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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FireReceiverComponent;
	class FireComponent;
	class WaterCannonComponent;
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
	*    EMERGENCY 5 extinguish fire action
	*/
	class EM5_API_EXPORT ExtinguishFireAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::ExtinguishFireAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_START_EFFECT,
			STATE_EXTINGUISH
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ExtinguishFireAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ExtinguishFireAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(const qsf::Entity& targetEntity, float extinguishPower, float coolingPower);

		uint64 getTargetEntityId() const;


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
		void startExtinguishEffect();
		void stopExtinguishEffect();
		qsf::Entity* createExtinguisherEffectEntity() const;
		qsf::Entity* createWaterJetEffectEntity();
		void linkEffectEntity(qsf::Entity& effectEntity);
		FireReceiverComponent* getFireComponentFromTarget();
		FireComponent* searchNewTargetInRange();
		uint64 getTargetIdForLinkingEffectEntity();
		void pushExtinguishAction(FireComponent& fireTarget) const;
		WaterCannonComponent* getWaterCannonComponent() const;
		bool isTargetInExtinguishRange(qsf::Entity& targetEntity, const glm::vec3& callerPosition) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		uint64	mTargetEntityId;		///< Target entity of the action
		float	mExtinguishPower;		///< Extinguish power in fire energy delta per second; should be a positive value
		float	mCoolingPower;			///< Cooling power in fire energy delta per second; should be a negative value
		bool	mTargetIsBuring;
		uint64	mEffectEntityId;		///< ID of the entity with the fire extinguish particle effect. This is created by us and we are responsible for deleting it.
		uint64	mOpticalTargetEntityId;	///< ID of the entity how is target for the effect (mostly an entity with fire particle, rarely an entity with fire component)
		State	mState;
		float	mMaxExtinguishRange;

		// Internal
		AudioProxy mAudioProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::ExtinguishFireAction)
