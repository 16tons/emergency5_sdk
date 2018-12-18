// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/equipment/weapon/WeaponConfiguration.h"

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
	*    EMERGENCY 5 shoot action
	*/
	class EM5_API_EXPORT ShootAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::ShootAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_AIM_AT_TARGET,
			STATE_FINISH_SHOOT,
			STATE_DONE
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ShootAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ShootAction();

		/**
		*  @brief
		*    Initialize action with target and weapon configuration
		*/
		void init(const qsf::Entity& targetEntity, const weapon::WeaponConfiguration& weaponConfiguration);

		uint64 getTargetEntityId() const;

		float getShootRange() const;


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
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool validateUserAndTarget() const;
		void showMuzzleFlash(bool visible);
		void injureTarget();
		void fleeCivilPerson(qsf::Entity& civilEntity);
		void unitShootBackAutomatisim(qsf::Entity& targetEntity);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		uint64		mTargetEntityId;	///< Target entity of the action
		weapon::WeaponConfiguration mWeaponConfig;

		// State
		State		mCurrentState;		///< Current action progress state
		qsf::Time	mWaitTimeToAim;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::ShootAction)
