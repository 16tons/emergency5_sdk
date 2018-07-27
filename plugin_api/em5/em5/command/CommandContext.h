// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf_game/command/CommandContext.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class Player;
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
	*    EMERGENCY 5 command context class
	*/
	class EM5_API_EXPORT CommandContext : public qsf::game::CommandContext
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum EvaluationReason
		{
			EVALUATIONREASON_UNKNOWN,
			EVALUATIONREASON_SHOWCOMMAND_CURSOR,		// Show an info which command can be executed on an entity under the mouse cursor (changes the mouse cursor)
			EVALUATIONREASON_SHOWCOMMAND_ABOVE_ENTITY	// Show an info which command can be executed on an entity (an icon is shown "above" the entity)
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline CommandContext();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~CommandContext();


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		Player*				mPlayer;			///< Player issuing the command; always valid, do not destroy
		bool				mIsAutomatism;		///< "true" if command is pushed by automatism, not by player direct
		EvaluationReason	mEvaluationReason;	///< Indicates the reason for an evaluation of an command


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/command/CommandContext-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(em5::CommandContext)
