// Copyright (C) 2012-2015 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/action/gangster/EscapeAction.h"


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
	*    EMERGENCY 5 escape foot short action
	*/
	class EscapeFootShortAction : public EscapeAction
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::EscapeFootShortAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		EscapeFootShortAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~EscapeFootShortAction();

		/**
		*  @brief
		*    Initialization
		*/
		void init(const std::string& escapeTargetTag);

		/**
		*  @brief
		*    This entity got catched
		*/
		virtual void catched() override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:

		uint32 getMovementMode(uint32 movementModeId);
		bool checkAllowUpdate();
		bool checkPositionStuck();
		glm::vec3 getClosestTargetPoint(bool onlyMapExits);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Internal
		bool mIsCivilPerson;

		qsf::Time mStuckTime;
		qsf::Time mElapsedTime;
		qsf::Time mPhaseEndTime;
		glm::vec3 mTargetPosition;
		glm::vec3 mOldPosition;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::EscapeFootShortAction)
