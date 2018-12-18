// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/BarrierTape/PlaceBarrierTapePole2Command.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/police/PlaceBarrierPoleAction.h"
#include "em5/command/CommandContext.h"
#include "em5/component/objects/BarrierTapeComponent.h"
#include "em5/specs/LogicSpecsGroup.h"

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PlaceBarrierTapePole2Command::PLUGINABLE_ID = qsf::StringHash("em5::PlaceBarrierTapePole2Command");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PlaceBarrierTapePole2Command::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlaceBarrierTapePole2Command::PlaceBarrierTapePole2Command(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// No mouse cursor, no button - command is pushed implicitly by the command mode
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool PlaceBarrierTapePole2Command::checkAvailable()
	{
		// This command will only be pushed explicitly and should never be chosen by some automatism
		return false;
	}

	bool PlaceBarrierTapePole2Command::checkCaller(qsf::Entity& caller)
	{
		// This command will only be pushed explicitly and should thus always have a valid caller
		return true;
	}

	bool PlaceBarrierTapePole2Command::checkContext(const qsf::game::CommandContext& context)
	{
		// This command will only be pushed explicitly and should thus always have a valid context
		return true;
	}

	void PlaceBarrierTapePole2Command::execute(const qsf::game::CommandContext& context)
	{
		// This command is only meant to store the position for the first pole into the caller on the host, so the PlaceBarrierTapePole2Command can then access this information needed to push the actual actions
		// For this to work we have to repeat, what already happened on the client:

		// Sanity check
		QSF_CHECK(nullptr != context.mCaller, "Place barrier tape pole 2 command was called without a caller", QSF_REACT_THROW);

		// TODO(ca) Using entities as temporary datastore is both, expensive and error-prone - we should try to avoid this whenever possible

		// Find the linked temporary barrier tape component on the linked child entity created by the place barrier tape pole 1 command
		qsf::LinkComponent& callerLinkComponent = context.mCaller->getOrCreateComponentSafe<qsf::LinkComponent>();
		BarrierTapeComponent* barrierTapeComponent = callerLinkComponent.getComponentFromEntityOrLinkedChild<BarrierTapeComponent>();
		if (nullptr != barrierTapeComponent)
		{
			// We'll take ownership of this entity now - it no longer needs to be linked to the caller
			barrierTapeComponent->getEntity().getComponentSafe<qsf::LinkComponent>().unlinkFromParent();

			const float transparency = LogicSpecsGroup::getInstanceSafe().getPoliceBarrierTransparency();

			// Initialize barrier tape component
			barrierTapeComponent->createPole(1);
			barrierTapeComponent->setPoleTransparency(1, transparency);
			barrierTapeComponent->movePole(1, context.getTargetPosition());

			// Push action (this transfers ownership of the barrier tape entity to the place barrier pole action, which will either release the entity into the wild if construction succeeds or delete it if not)
			qsf::ActionComponent& actionComponent = context.mCaller->getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.clearPlan();
			actionComponent.pushAction<PlaceBarrierPoleAction>(ACTION_PRIORITY).init(barrierTapeComponent);
		}
		else
		{
			QSF_ERROR("Failed to find barrier tape entity", QSF_REACT_NONE);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
