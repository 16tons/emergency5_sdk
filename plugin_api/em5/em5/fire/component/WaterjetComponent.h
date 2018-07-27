// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/component/Component.h>
#include <qsf/base/PropertyHelper.h>
#include <qsf/job/JobProxy.h>
#include <qsf/debug/DebugDrawProxy.h>

#include <glm/gtc/quaternion.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class JobArguments;
	class Transform;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 waterjet component class
	*/
	class EM5_API_EXPORT WaterjetComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;				///< "em5::WaterjetComponent" unique component ID
		static const uint32 TARGET_ENTITY_ID;
		static const uint32 ANGLE_IN_DEGREES;
		static const uint32 ANGLE_IN_RADIANS;
		static const uint32 START_RADIUS;
		static const uint32 END_RADIUS;
		static const uint32 END_RADIUS_Y_FACTOR;
		static const uint32 END_BOTTOM_LAPSE;
		static const uint32 BUILDUP_DISTANCE_PER_SECOND;
		static const uint32 DISABLE_ALL_PARTICLES;
		static const uint32 DISABLE_COLLISION_CHECKS;
		static const uint32 HIT_ENTITY_ID;
		static const uint32 HIT_POSITION;
		static const uint32 HIT_GROUND_POSITION;


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
		explicit WaterjetComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~WaterjetComponent();

		inline uint64 getTargetEntityId() const;
		inline void setTargetEntityId(uint64 entityId);

		qsf::Entity* getTargetEntity() const;
		void setTargetEntity(qsf::Entity* targetEntity);

		inline float getAngleInRadians() const;
		inline void setAngleInRadians(float angleInRadians);

		inline float getAngleInDegrees() const;
		inline void setAngleInDegrees(float angleInDegree);

		inline float getStartRadius() const;
		inline void setStartRadius(float startRadius);

		inline float getEndRadius() const;
		inline void setEndRadius(float endRadius);

		inline float getEndRadiusYFactor() const;
		inline void setEndRadiusYFactor(float endRadiusYFactor);

		inline float getEndBottomLapse() const;
		inline void setEndBottomLapse(float endBottomLapse);

		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(BuildupDistancePerSecond, float, mBuildupDistancePerSecond, BUILDUP_DISTANCE_PER_SECOND);

		inline bool getDisableAllParticles() const;
		inline void setDisableAllParticles(bool state);

		inline bool getDisableCollisionChecks() const;
		inline void setDisableCollisionChecks(bool state);

		inline uint64 getHitEntityId() const;

		inline const glm::vec3& getHitPosition() const;

		inline const glm::vec3& getHitGroundPosition() const;

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(CurrentDistance, float, mCurrentDistance);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetDebug(bool debug) override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateHit(uint64 entityId, const glm::vec3& hitPosition, const glm::vec3& hitGoundPosition);
		void updateAnimation(const qsf::JobArguments& jobArguments);
		void updateHitParticlesDead(const qsf::JobArguments& jobArguments);
		void updateMesh();
		void updateParticles();
		void destroyParticles();
		void destroyHitParticles();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const float PARTICLES_SCALE;				//< Scale down the particles, requested by the artists
		static const float PARTICLE_DISTANCE_FACTOR;	//< Factor that the distance between individual waterjet particles is multiplied with
		static const char PARTICLES_WATERJET_ASSET[];
		static const char PARTICLES_HIT_ASSET[];
		static const char PARTICLES_HIT_HACK_ASSET[];
		static const char PARTICLES_HIT_GROUND_ASSET[];

		struct JetParticle
		{
			float positionY;
			float positionZ;
			float scale;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Input - connected to CAMP
		uint64			mTargetEntityId;
		float			mAngleInRadians;
		float			mStartRadius;
		float			mEndRadius;
		float			mEndRadiusYFactor;
		float			mEndBottomLapse;
		float			mBuildupDistancePerSecond;
		bool			mDisableAllParticles;
		bool			mDisableCollisionChecks;

		// Output - connected to CAMP (read only)
		uint64			mHitEntityId;
		glm::vec3		mHitPosition;
		glm::vec3		mHitGroundPosition;

		// Internal - not connected to CAMP
		float					 mCurrentDistance;
		float					 mTargetDistance;
		float					 mTargetHeight;
		float					 mLastDistance;
		glm::quat				 mLastRotation;
		qsf::DebugDrawProxy		 mDebugDrawProxy;	///< Debug draw proxy
		std::vector<uint64>		 mWaterjetParticlesEntityIds;
		uint64					 mHitParticlesEntityId;
		uint64					 mHitHackParticlesEntityId;	// TODO(co) Remove this as soon as the artists integrated the hit particles into the hit & drop particles effect, we don't want to have too
		uint64					 mHitGroundParticlesEntityId;
		std::vector<JetParticle> mParticlePoints;

		// Job proxies for regular update
		qsf::JobProxy mAnimationJobProxy;	///< Regular animation job proxy; for updates once a frame
		qsf::JobProxy mHitParticlesDeadJobProxy;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/fire/component/WaterjetComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::WaterjetComponent)
