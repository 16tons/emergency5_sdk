// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/gangsters/eventspecific/GangsterDemoExtremist.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterDemoExtremist::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterDemoExtremist");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterDemoExtremist::GangsterDemoExtremist() :
		GangsterBaseLogic(GAMELOGIC_TYPE_ID),
		mState(STATE_INIT)
	{
		// These are just fallback values, in case the gangster type JSON does not define something else
		addAutoTargetEventTag("Policeman", 5);
		addAutoTargetEventTag("PoliceSekman", 5);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GangsterBaseLogic methods      ]
	//[-------------------------------------------------------]
	void GangsterDemoExtremist::onSimulationUpdate(const qsf::JobArguments& jobArguments)
	{
		// Nothing special to do in here, all gangster behavior is handled in "onIdle"
	}

	void GangsterDemoExtremist::onIdle()
	{
		qsf::Entity& entity = *getEntity();
		qsf::ActionComponent& actionComponent = mActionComponent.getSafe();

		switch (mState)
		{
			case STATE_INIT:
			{
				break;
			}
		}
	}

	void GangsterDemoExtremist::onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits)
	{
		// Nothing here
	}

	void GangsterDemoExtremist::onNoUnitsSpotted()
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
