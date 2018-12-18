// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/police/SEKSpecialistLogic.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 SEKSpecialistLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::SEKSpecialistLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SEKSpecialistLogic::SEKSpecialistLogic() :
		PolicemanLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool SEKSpecialistLogic::onStartup()
	{
		return PolicemanLogic::onStartup();
	}

	void SEKSpecialistLogic::onShutdown()
	{
		PolicemanLogic::onShutdown();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
