// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/component/equipment/weapon/Weapon.h"

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
	*    EMERGENCY 5 be thrown weapon action
	*
	*  @note
	*    - This action is to be inserted into the action plan of the thrown weapon
	*    - It deals with collision and whatever shall happen when other objects are hit
	*/
	class EM5_API_EXPORT BeThrownWeaponAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::BeThrownWeaponAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BeThrownWeaponAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BeThrownWeaponAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(uint64 throwerEntityId, uint64 targetEntityId, weapon::Weapon thrownWeapon, bool throwToGround = false);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual void onShutdown() override;
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum State
		{
			STATE_INIT,
			STATE_FLYING
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onCollision(qsf::Entity* hitTarget);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float			mAccumulatedTime;
		uint64			mThrowerEntityId;	///< Who threw?
		uint64			mTargetEntityId;
		glm::vec3		mInitialPosition;	///< The position the object is thrown from
		glm::vec3		mTargetPosition;	///< The position (world coordinates) the object shall fly to
		State			mCurrentState;		///< Current action progress state
		weapon::Weapon	mThrownWeapon;		///< What shall be thrown?
		bool			mThrowToGround;		///< If set, throw to pivot of target, else to bounding box center
		bool			mCanPlayAudio;		///< If true, its possible to play an impact audio


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::BeThrownWeaponAction)
