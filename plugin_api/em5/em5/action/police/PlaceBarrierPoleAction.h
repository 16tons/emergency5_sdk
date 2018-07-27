// Copyright (C) 2012-2017 Promotion Software GmbH


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
//[ Forward Declaration                                   ]
//[-------------------------------------------------------]
namespace em5
{
	class BarrierTapeComponent;
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
	*    EMERGENCY 5 action to place a pole for a barrier tape
	*/
	class EM5_API_EXPORT PlaceBarrierPoleAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::PlaceBarrierPoleAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_MOVE_TO_POLE0,
			STATE_DROP_POLE0,
			STATE_MOVE_TO_POLE1,
			STATE_DROP_POLE1,
			STATE_FINISH
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PlaceBarrierPoleAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PlaceBarrierPoleAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(BarrierTapeComponent* barrierTapeComponent);

		/**
		*  @brief
		*    Give back the state of this action
		*/
		State getActionState() const;


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
		bool validateUserAndTarget(const glm::vec3& targetPosition) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		State	mState;

		// Configuration
		qsf::WeakPtr<BarrierTapeComponent>	mBarrierTapeComponent;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::PlaceBarrierPoleAction)
