// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/equipment/weapon/WeaponConfiguration.h"
#include "em5/audio/AudioProxy.h"

#include <qsf/logic/action/Action.h>


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
	*    EMERGENCY 5 arrest gangster action
	*/
	class EM5_API_EXPORT ArrestGangsterAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::ArrestGangsterAction" unique action identifier

		static const uint64 FIGHT_ROUNDS;				///< number of fight rounds before a gangster is arrested


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void setArrestedParameters(qsf::Entity& arrestedEntity, bool arrested);

		// Offset between player and target
		// Usually this is in the target points, but arresting has none so far, so I had to put it here
		static glm::vec3 getArrestOffset();
		static glm::vec3 getFightingOffset();
		static glm::vec3 getFightingLongRangeOffset();


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ArrestGangsterAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ArrestGangsterAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(const qsf::Entity& targetEntity);

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
		weapon::Weapon getWeaponFromGangster() const;
		void ensureRelativePositions(bool arrestSituation);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum State
		{
			STATE_INIT,					///< The arresting starts here
			STATE_FIGHT,				///< If a fist fight occurs, the action will be in this state
			STATE_ARRESTING,
			STATE_WAIT_FOR_ANIMATIONS,	///< The actual arresting is taking place, i.e. the "handcuffing"
			STATE_DONE					///< When the fight is done, this state is reached and the target can now be arrested
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		uint64					  mTargetEntityId;	///< Target entity ID of the action
		qsf::WeakPtr<qsf::Entity> mTargetEntity;	///< Target entity of the action

		State		mCurrentState;	///< The state of the action
		AudioProxy	mAudioProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::ArrestGangsterAction)
