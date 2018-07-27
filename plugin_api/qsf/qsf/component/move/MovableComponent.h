// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/component/move/TickInterpolationHelper.h"
#include "qsf/link/LinkProxy.h"
#include "qsf/math/BitFlagSet.h"
#include "qsf/math/Transform.h"
#include "qsf/time/Time.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Clock;
	class LinkComponent;
	class ContainerLink;
	class PlacementComponent;
	class TransformComponent;
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
	*    Movable component base class
	*
	*  @remarks
	*    For movable entities, there are two actual transformations:
	*     - The "simulation" transformation is used by any "higher-level" or game logic.
	*       It is only updated by the simulation itself, inside of a simulation tick - i.e. with a fixed update rate, independent from the frame rate.
	*     - The "animation" transformation is what the user actually sees on the screen, and should in general be updated for each renderer frame by
	*       interpolating the simulation transformations. It may also contain a transformation offset (think of hovering of helicopters as a visual effect).
	*
	*    To handle both types of transformation, we use two different components, namely movable and transform component.
	*     - In case the entity can't move (or at least does not need to move smoothly) there is no movable component used and no differentiation is necessary,
	*       and the transform component represents both types of transformations.
	*     - If the entity is able to move, it has both components: The transform component stores the simulation transformation, while the movable holds
	*       (among other data) the interpolated animation transformation. It is possible to use the movable component to access both types of transformations,
	*       and it should in general preferred to be used for all write access.
	*/
	class QSF_API_EXPORT MovableComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class LinkComponent;		// Needs to call "setIsLinkedChild()"


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Component
		static const uint32 COMPONENT_ID;			///< "qsf::MovableComponent" unique component ID
		static const uint32 CONTAINER;				///< "Container" unique class property ID inside the class
		static const uint32 GROUND_SPEED_CATEGORY;	///< "GroundSpeedCategory" unique class property ID inside the class


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
		explicit MovableComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MovableComponent();

		//[-------------------------------------------------------]
		//[ Transformation getters                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return current position
		*
		*  @return
		*    The current simulation position
		*
		*  @note
		*    - The position is taken from the transform component
		*/
		const glm::vec3& getPosition() const;

		/**
		*  @brief
		*    Return current rotation
		*
		*  @return
		*    The current simulation rotation
		*
		*  @note
		*  ´ - The rotation is taken from the transform component
		*/
		const glm::quat& getRotation() const;

		//[-------------------------------------------------------]
		//[ Smooth movement                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Move to the given position
		*
		*  @param[in] position
		*    The position where to move the entity to
		*
		*  @remarks
		*    The moveTo methods are thought for smooth continuous movement of entities.
		*    If you want to perform a hard position/rotation change, use warpToPosition instead.
		*/
		void moveToPosition(const glm::vec3& position);

		/**
		*  @brief
		*    Change the entity's rotation
		*
		*  @param[in] rotation
		*    The rotation to set
		*
		*  @remarks
		*    The moveTo methods are thought for smooth continuous movement of entities.
		*    If you want to perform a hard position/rotation change, use warpToPositionAndRotation instead.
		*/
		void moveToRotation(const glm::quat& rotation);

		/**
		*  @brief
		*    Move to the given position and rotation
		*
		*  @param[in] position
		*    The position where to move the entity to
		*  @param[in] rotation
		*    The rotation to set
		*
		*  @remarks
		*    The moveTo methods are thought for smooth continuous movement of entities.
		*    If you want to perform a hard position/rotation change, use warpToPositionAndRotation instead.
		*/
		void moveToPositionAndRotation(const glm::vec3& position, const glm::quat& rotation);

		//[-------------------------------------------------------]
		//[ Warping                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Warp to the given position
		*
		*  @param[in] position
		*    The position where to move the entity to
		*
		*  @remarks
		*    Use this method for hard position changes to avoid automatically induced effects like interpolation between game ticks.
		*    For smooth continuous movement, prefer the respective moveTo method.
		*/
		void warpToPosition(const glm::vec3& position);

		/**
		*  @brief
		*    Warp to the given position and rotation
		*
		*  @param[in] position
		*    The position where to move the entity to
		*  @param[in] rotation
		*    The rotation to set
		*
		*  @remarks
		*    Use this method for hard position changes to avoid automatically induced effects like interpolation between game ticks.
		*    For smooth continuous movement, prefer the respective moveTo method.
		*/
		void warpToPositionAndRotation(const glm::vec3& position, const glm::quat& rotation);

		//[-------------------------------------------------------]
		//[ Moving flag                                           ]
		//[-------------------------------------------------------]
		inline bool isMoving() const;

		//[-------------------------------------------------------]
		//[ Velocity & acceleration getters                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the currently set velocity
		*
		*  @return
		*    Velocity vector in world space, given in units per second
		*/
		inline const glm::vec3& getVelocity() const;

		/**
		*  @brief
		*    Return the currently set rotation velocity quaternion
		*
		*  @param[in] referenceTime
		*    Time the rotation change applies to
		*
		*  @return
		*    Angular rotation velocity quaternion in world space, given as rotation per second
		*/
		glm::quat getAngularVelocity(const qsf::Time& referenceTime = qsf::Time::fromSeconds(1.0f)) const;

		/**
		*  @brief
		*    Return the currently set acceleration vector
		*
		*  @return
		*    Linear acceleration vector in world space, given in units per second^2
		*/
		inline const glm::vec3& getAcceleration() const;

		/**
		*  @brief
		*    Return the currently set rotation acceleration quaternion
		*
		*  @return
		*    Angular rotation acceleration quaternion in world space, given as rotation per second^2
		*/
		inline const glm::quat& getAngularAcceleration() const;

		/**
		*  @brief
		*    Return scalar speed value
		*
		*  @return
		*    Current movement speed along front vector, in world units per second
		*/
		float getSpeed() const;

		float getForwardAcceleration() const;

		//[-------------------------------------------------------]
		//[ Velocity & acceleration setters                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set the current velocity
		*
		*  @param[in] velocity
		*    Velocity vector, expressed in units per second
		*
		*  @remarks
		*    The velocity is used as a hint for interpolation and can be used by other components (e.g. for calculating physics forces).
		*/
		void setVelocity(const glm::vec3& velocity);

		/**
		*  @brief
		*    Set the current angular rotation velocity
		*
		*  @param[in] angularVelocity
		*    Angular velocity quaternion, describes current local rotation change per reference time
		*  @param[in] referenceTime
		*    Time the rotation change applies to
		*
		*  @remarks
		*    Angular velocity is used as a hint for interpolation and can be used by other components as well.
		*/
		void setAngularVelocity(const glm::quat& angularVelocity, const qsf::Time& referenceTime = qsf::Time::fromSeconds(1.0f));

		/**
		*  @brief
		*    Set the current acceleration
		*
		*  @param[in] acceleration
		*    Linear acceleration vector, expressed in units per second^2
		*
		*  @remarks
		*    Acceleration is used as a hint for interpolation and can be used by other components as well.
		*/
		void setAcceleration(const glm::vec3& acceleration);

		/**
		*  @brief
		*    Set the current angular rotation acceleration
		*
		*  @param[in] angularAcceleration
		*    Angular acceleration vector, describes change in angular rotation velocity per second
		*
		*  @remarks
		*    Rotation acceleration is used as a hint for interpolation and can be used by other components as well.
		*/
		void setAngularAcceleration(const glm::quat& angularAcceleration);

		//[-------------------------------------------------------]
		//[ Transform bias                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return transform bias
		*
		*  @return
		*    The bias transformation that is automatically applied to the interpolated transformation
		*/
		inline const Transform& getTransformBias() const;

		/**
		*  @brief
		*    Disable the transform bias
		*/
		void resetTransformBias();

		/**
		*  @brief
		*    Set transform bias
		*
		*  @param[in] transformBias
		*    The bias transformation that is automatically applied to the interpolated transformation
		*/
		void setTransformBias(const Transform& transformBias);

		//[-------------------------------------------------------]
		//[ Container referencing                                 ]
		//[-------------------------------------------------------]
		inline bool hasContainer() const;
		inline uint64 getContainerEntityId() const;
		Entity* getContainerEntity() const;
		const ContainerLink* getContainerLink() const;
		void setContainerId(uint64 id);
		void setContainer(const Prototype* prototype);

		//[-------------------------------------------------------]
		//[ Regular update calls                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Must be called when a simulation tick starts
		*/
		void onTickStart(const Clock& simulationClock);

		/**
		*  @brief
		*    Must be called when a simulation tick ends
		*/
		void onTickEnd();

		/**
		*  @brief
		*    Apply entity interpolation
		*
		*  @return
		*    true if the interpolation values has changed (the property changed propagation is needed) otherwise false
		*/
		bool applyInterpolation(float normalizedTimeOffset);

		/**
		*  @brief
		*    Copy position and rotation to the entity's transform component
		*/
		void applyMovementWithoutInterpolation();

		/**
		*		// TODO(fw): This description is not correct any more
		*  @brief
		*    Lets the component send the promotePropertyChange "event" with INTERPOLATED_TRANSFORM as parameter
		*
		*  @note
		*    - This method is only used from EntityInterpolation class and is needed for the parallelized version
		*/
		void propagateInterpolationChange();

		//[-------------------------------------------------------]
		//[ Ground speed                                          ]
		//[-------------------------------------------------------]
		inline const std::string& getGroundSpeedCategory() const;
		inline void setGroundSpeedCategory(const std::string& groundSpeedCategory);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onSetSimulating(bool simulating) override;
		virtual void onSetActive(bool active) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setIsLinkedChild(bool isLinkedChild);
		void internalActivate(bool activate);

		void calculateLinearVelocity() const;
		void calculateAngularVelocity() const;
		void calculateLinearAcceleration() const;
		void calculateAngularAcceleration() const;

		void applyTransformationBias(glm::vec3& interpolatedPosition, glm::quat& interpolatedRotation);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum InternalFlags
		{
			INTERPOLATION_ACTIVE = 1<<0,	///< If set, entity interpolation should be applied
			LINKED_CHILD		 = 1<<1,	///< If set, entity is a child entity
			WARPED				 = 1<<2,	///< This flag is set in case the position and rotation were warped in the last simulation update
			IN_TICK_UPDATE		 = 1<<3,	///< Set during the tick update, i.e. in between "onTickStart" and "onTickEnd"
			MOVING				 = 1<<4,	///< If set, entity is moving (or at least preparing movement)
			HAS_TRANSFORM_BIAS	 = 1<<5,	///< This flag is set in case the transformation bias is set and not trivial
		};

		typedef BitFlagSet<unsigned short, InternalFlags> InternalFlagsSet;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Direct component references for efficient access
		WeakPtr<TransformComponent> mTransformComponent;
		WeakPtr<PlacementComponent> mPlacementComponent;

		// Backup of state before simulation update
		glm::vec3 mFormerPosition;			///< Position of former simulation update
		glm::quat mFormerRotation;			///< Rotation of former simulation update
		glm::vec3 mFormerVelocity;			///< Linear velocity of former simulation update
		glm::quat mFormerAngularVelocity;	///< Angular rotation velocity of former simulation update

		// Velocities and accelerations
		mutable glm::vec3 mVelocity;				///< Current linear velocity vector, in units per second
		mutable glm::quat mAngularVelocity;			///< Current rotation velocity, as rotation quaternion change per reference time (not a second, this would be too much to handle fast rotation)
		mutable glm::vec3 mAcceleration;			///< Current linear acceleration vector, in units per second^2
		mutable glm::quat mAngularAcceleration;		///< Current rotation acceleration, as rotation quaternion change per sequred reference time (not a second, this would be too much to handle fast rotation)
		float mLastTickLengthInSeconds;				///< Length of the last simulation tick in seconds; needed to be able to do automatic velocity and acceleration updates

		// Flags
		mutable InternalFlagsSet mInternalFlags;	///< Flag bitmask for the internal flags

		// Tick interpolation helper
		TickInterpolationHelper mTickInterpolationHelper;

		// Transformation bias
		Transform mTransformBias;

		// Link proxy pointing to the container
		LinkProxy mContainerLinkProxy;

		std::string mGroundSpeedCategory;	///< Ground speed category


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
#include "qsf/component/move/MovableComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::MovableComponent)
