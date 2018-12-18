// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/audio/AudioProxy.h"

#include <qsf/logic/action/Action.h>
#include <qsf/reflection/type/CampGlmQuat.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FireComponent;
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
	*    EMERGENCY 5 area extinguish action
	*/
	class EM5_API_EXPORT AreaExtinguishAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::AreaExtinguishAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_FLY_TO_POSITION,
			STATE_WATER_IMPACT_DELAY,
			STATE_EXTINGUISH,
			STATE_FLY_AWAY
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		AreaExtinguishAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AreaExtinguishAction();

		void init(glm::vec3 targetPosition, qsf::Time durration, float radius, float extinguishPower, float coolingPowerPerSecond);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void createAirPlaneEntity();
		void createGroundEffectMarker();
		void createAirParticleEffect();
		void createGroundParticleEffect();

		void updateAirplanePosition(qsf::Time elapsedTime);
		void updateGroundParticle(qsf::Time elapsedTime);

		void deactivateEffectParticle();
		void deactivateEffectParticleAir();
		void deleteParticleEffects();
		void closeHatch();
		void damagePersonsInArea(float timePassedInSeconds);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<FireComponent*>				mExtinguishFireComponents;
		State									mState;
		glm::vec3								mTargetPosition;
		qsf::WeakPtr<qsf::Entity>				mAirplaneEntity;
		qsf::WeakPtr<qsf::Entity>				mGroundMarkerEntity;
		qsf::WeakPtr<qsf::Entity>				mRadarEntity;			///< The commando car has a radar on the back
		qsf::WeakPtr<qsf::Entity>				mEffectParticleAir;		///< The particle for the extinguishing progress, sticks on the airplane
		std::vector<qsf::WeakPtr<qsf::Entity>>	mEffectParticlesGround;	///< The particle for the extinguishing progress, hovers over the ground
		bool									mCanPlayPersonDamageHint;
		glm::quat								mInitialRadarRotation;

		// Configuration
		qsf::Time	mDurration;			///< The time the effect currently is active
		qsf::Time	mDurrationAirEffect;
		qsf::Time	mTotalDuration;		///< The time the extinguish takes
		float		mExtinguishPowerPerSecond;
		float		mCoolingPowerPerSecond;
		float		mRadius;

		// Particle handling
		glm::vec3	mEffectStartPosition;
		qsf::Time	mTimeLastGroundEffect;

		// Audio
		AudioProxy	mAudioProxyAirplane;
		AudioProxy	mAudioProxyEffectStart;
		AudioProxy	mAudioProxyEffectImpact;

		AudioProxy	mAudioProxyRadarNoise;
		AudioProxy	mAudioProxyUseAirplaneFeedback;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::AreaExtinguishAction)
