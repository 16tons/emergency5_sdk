// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/logic/action/Action.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class DlkLadderComponent;
}
namespace qsf
{
	class GameLogicComponent;
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
	*    EMERGENCY 5 DLK save person action
	*/
	class EM5_API_EXPORT DLKSavePersonAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::DLKSavePersonAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_EXTENDING_LEGS,
			STATE_LADDER_ALIGNMENT,
			STATE_WAIT_1,
			STATE_RESCUE_FROM_BUILDING,
			STATE_WAIT_2,
			STATE_FOLD_LADDER,
			STATE_WAIT_3,
			STATE_RESCUE_FROM_DLK,
			STATE_HIDE_LEGS,
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
		DLKSavePersonAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DLKSavePersonAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(uint64 targetEntityId);

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
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void fadeOutPerson();
		void rescuePersonFromBuilding();
		void rescuePersonFromDlk();
		void updateGameLogicComponent();
		void putEntityOnDlk(qsf::Entity* entity);
		glm::vec3 getTargetPosition(qsf::Entity& targetEntity);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64						mTargetEntityId;	///< Entity that the ladder should target
		State						mCurrentState;
		float						mSecondsPassed;
		qsf::GameLogicComponent*	mGameLogicComponent;
		bool						mInjurePersonAfterRescue;
		bool						mActivateFireReceiverAfterRescue;
		qsf::WeakPtr<DlkLadderComponent> mDlkLadderComponent;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::DLKSavePersonAction)
