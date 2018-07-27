// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/logic/observer/CheeringObserver.h"

#include <em5/plugin/Messages.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CheeringObserver::GAMELOGIC_TYPE_ID = qsf::StringHash("user::CheeringObserver");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CheeringObserver::CheeringObserver() :
		em5::Observer(GAMELOGIC_TYPE_ID),
		mObjectiveId(qsf::getUninitialized<uint32>())
	{
		// Nothing here
	}

	void CheeringObserver::connectToObjective(em5::Objective& objective)
	{
		mObjectiveId = objective.getId();

		mMessageProxy.registerAt(qsf::MessageConfiguration("user::CheerActionFinished"), boost::bind(&CheeringObserver::onCheering, this, _1));
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Observer methods                  ]
	//[-------------------------------------------------------]
	void CheeringObserver::serialize(qsf::BinarySerializer& serializer)
	{
		Observer::serialize(serializer);
		serializer.serialize(mObjectiveId);

		if (serializer.isReading())
		{
			mMessageProxy.unregister();

			mMessageProxy.registerAt(qsf::MessageConfiguration("user::CheerActionFinished"), boost::bind(&CheeringObserver::onCheering, this, _1));
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void CheeringObserver::onCheering(const qsf::MessageParameters& parameters)
	{
		em5::Objective* objective = em5::Objective::getObjectiveById(mObjectiveId);
		if (nullptr != objective)
		{
			objective->increaseCurrentNumber();
		}

		// This observer is not needed any more now, so remove it
		removeThis();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user
