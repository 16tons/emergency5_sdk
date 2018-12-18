// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/UseObjectLogic.h"

#include <qsf/base/StringHash.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 UseObjectLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::UseObjectLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UseObjectLogic::UseObjectLogic(uint32 gameLogicId) :
		qsf::GameLogic(gameLogicId)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::UseObjectLogic methods            ]
	//[-------------------------------------------------------]
	void UseObjectLogic::useObject()
	{
		// Default implementation: do nothing
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
