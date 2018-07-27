// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/job/JobProxy.h"
#include "qsf/job/JobArguments.h"
#include "qsf/component/Component.h"
#include "qsf/reflection/type/CampGlmVec3.h"

#include <boost/container/flat_map.hpp>

#include <vector>
#include <atomic>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class btRigidBody;
class btDynamicsWorld;
class btCollisionObject;
struct ContactResultCallback;
namespace qsf
{
	class PhysicsDebugDraw;
	class CollisionComponent;
	class PhysicsMotionState;
	class BulletCollisionComponent;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Component managing a bullet dynamic collision world.
	*    Currently it is optimized to the specific AI needs.
	*    That means to optimize performance static collisions are registered only at editing time while dynamic collisions are only registered at runtime.
	*    The AI needs to know about the static collisions to create its maps inside the editor and the dynamic obstacles to evade at runtime.
	*    We need to decide whether to move this logic to the AI library again if we keep the semantic this special.
	*    It can't be used as a classical physics engine at the moment for these reasons.
	*/
	class QSF_API_EXPORT PhysicsWorldComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class GeneralDebugGroup;	// Must be able to call "qsf::PhysicsWorldComponent::setShowCollision()"


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::PhysicsWorldComponent" unique component ID
		static const uint32 GRAVITY;		///< "Gravity" unique class property ID inside the class

		// Type definitions
		typedef std::pair<const btCollisionObject*, const btCollisionObject*> CollisionPairType;
		typedef boost::container::flat_multimap<const btCollisionObject*, const btCollisionObject*> CollidingComponentsMultimap;	///< Map containing collisions sorted by collider

		// Signals
		boost::signals2::signal<void(CollisionComponent&)> CollisionComponentAdded;
		boost::signals2::signal<void(CollisionComponent&)> CollisionComponentRemoved;

		// Contains information about an hit of an ray cast
		struct HitResult
		{
			Entity*   entity;					///< The entity which was hit by the ray
			glm::vec3 hitPosition;				///< The position (in world coordinates) where the collision object was hit by the ray
			glm::vec3 hitNormal;				///< The normal vector of the hit
			glm::vec3 hitPositionFromFraction;	///< The position (in world coordinates) where the collision object was hit by the ray. This value is calculated from the ray start position and the distance (called in bullet fraction) in direction of the ray
			float	  hitFraction;				///< Hit fraction along the ray
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		explicit PhysicsWorldComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PhysicsWorldComponent();

		/**
		*  @brief
		*    Return the gravity vector
		*
		*  @return
		*    The gravity vector, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec3& getGravity() const;

		/**
		*  @brief
		*    Set the gravity vector
		*
		*  @param[in] gravity
		*    The gravity vector to set
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setGravity(const glm::vec3& gravity);

		/**
		*  @brief
		*    Set the target frames per second
		*
		*  @param[in] targetFramesPerSecond
		*    The target frames per second, a value of 60 commonly used, 0 to disable fixed timesteps
		*/
		inline void setTargetFramesPerSecond(float targetFramesPerSecond);

		//[-------------------------------------------------------]
		//[ Collision component registration                      ]
		//[-------------------------------------------------------]
		inline void registerCollisionComponent(BulletCollisionComponent& collisionComponent);
		inline void unregisterCollisionComponent(BulletCollisionComponent& collisionComponent);

		//[-------------------------------------------------------]
		//[ Intersection                                          ]
		//[-------------------------------------------------------]
		/**
		* See the bullet documentation for the collision flags that are used to select among the results.
		* A flag value of -1 does not filter at all (called ALL_FILTER inside bullet) while a value of 1 only selects "default" collisions.
		* The rationale behind the current default values is as follows. We want to select the collisions that define themselves as default (their mask contains 1, so our filter group also contains 1).
		* But we don't want to filter among these objects based on with which types these usually want to collide at all (so we negate their filter group by making our mask std::numeric_limits<short>::max())
		*/
		//@{
		bool rayTestFirstHit(const glm::vec3& origin, const Entity& targetEntity, glm::vec3& hitLocation, short collisionFilterGroup = 1, short collisionMask = -1) const;
		bool rayTestFirstHit(const glm::vec3& origin, const glm::vec3& end, const Entity& targetEntity, glm::vec3& hitLocation, short collisionFilterGroup = 1, short collisionMask = -1) const;
		Entity* rayTestFetchFirstHitEntity(const glm::vec3& begin, const glm::vec3& end, glm::vec3* hitLocation = nullptr, short collisionFilterGroup = 1, short collisionMask = -1);
		bool rayTestFetchHitEntities(const glm::vec3& begin, const glm::vec3& end, std::list<Entity*>& entityList, short collisionFilterGroup = 1, short collisionMask = -1);
		// The entities in the entityList are sorted by the distance to the ray origin (nearest one is first in the list)
		bool rayTestFetchHitEntitiesSorted(const glm::vec3& begin, const glm::vec3& end, std::vector<HitResult>& entityList, short collisionFilterGroup = 1, short collisionMask = -1);
		//@}

		//[-------------------------------------------------------]
		//[ Thread save encapsulation                             ]
		//[-------------------------------------------------------]
		void addBulletCollisionObject(btCollisionObject* collisionObject, short int collisionFilterGroup, short int collisionFilterMask);
		void removeBulletCollisionObject(btCollisionObject* collisionObject);
		void addBulletRigidBody(btRigidBody* body, short group, short mask);
		void removeBulletRigidBody(btRigidBody* body);

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the Bullet dynamics world
		*
		*  @return
		*    The Bullet dynamics world, can be a null pointer, do not destroy the instance
		*
		*  @note
		*    - If the method call was successful, call "qsf::PhysicsWorldComponent::releaseBulletDynamicsWorld()" when done
		*    - Don't use Bullet directly if you don't have to
		*/
		btDynamicsWorld* acquireBulletDynamicsWorld();
		void releaseAcquiredBulletDynamicsWorld();

		/**
		*  @brief
		*    Return a multimap of all Bullet components that collided the last game tick
		*
		*  @return
		*    A std::multimap of collided BulletCollisionComponents
		*    All real collisions (between bullet objects having a user pointer pointing to a BulletCollisionComponent
		*    are included symmetrically. Furthermore, collisions between a virtual query object (user pointer is nullptr)
		*    and real objects are included asymmetrically, virtual collides with real, but not vice versa.
		*
		*  @note
		*    - Do not insert null pointers
		*/
		inline const CollidingComponentsMultimap& getCurrentCollisions() const;

		/**
		*  @brief
		*    Main update function that is passed to the job manager
		*
		*  @param[in] jobArguments
		*    Job arguments, in case no time has past just a collision detection will be performed without performing a physics simulation step
		*
		*  @note
		*    - Don't call this method directly if you don't have to
		*/
		void updateJob(const JobArguments& jobArguments);

		inline bool hasEnqueuedUpdates() const;
		void performEnqueuedUpdate();
		inline void enqueueBulletWorldTransformChange(PhysicsMotionState& physicsMotionState);

		/**
		*  @brief
		*    Inject the world transform changes made by Bullet during the simulation step back into QSF
		*/
		void dispatchBulletWorldTransformChanges();

		void setBulletCollisionObjectDebugState(const btCollisionObject& bulletCollisionObject, bool debug);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual void onSetSimulating(bool simulating) override;
		inline virtual void onSetActive(bool active) override;
		inline virtual void onSetDebug(bool debug) override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateJobProxyRegistration();
		inline void enqueuePhysicsUpdate(const JobArguments& jobArguments);
		void setPhysicsDebugDrawActive(bool active);
		void setShowCollision(bool showCollision);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		class BulletPhysicsData;	// Nested class
		typedef std::vector<PhysicsMotionState*> EnqueuedBulletWorldTransformChanges;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		glm::vec3 mGravity;	///< Gravity vector
		// Internal
		JobProxy							mUpdateJobProxy;				///< Job proxy
		BulletPhysicsData*					mPImpl;							///< (partial) PImpl idiom
		CollidingComponentsMultimap			mCurrentlyCollidingEntities;	///< Only references components; do not delete pointers; holds currently colliding bullet components
		std::vector<CollisionPairType>		mTemporaryUnsortedCollisions;
		float								mTargetFramesPerSecond;			///< We are calculating the fixed timestep in bullet from this value, choose 0 to disable fixed timesteps
		std::deque<JobArguments>			mEnqueuedUpdates;
		EnqueuedBulletWorldTransformChanges	mEnqueuedBulletWorldTransformChanges;
		int									mShowCollisionRequests;
		PhysicsDebugDraw*					mPhysicsDebugDraw;
		int									mPhysicsDebugDrawActiveRequests;
		bool								mBulletDynamicsWorldAcquired;

	public:
		std::atomic_bool mPhysicsUpdateInProgress;	///< Actually only for internal builds (additional check to find erroneous behavior), but we need to make sure end-user SDK is the same with or without ENDUSER define


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/physics/PhysicsWorldComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PhysicsWorldComponent)
