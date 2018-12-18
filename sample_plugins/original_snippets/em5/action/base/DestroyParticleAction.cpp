// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/DestroyParticleAction.h"
#include "em5/map/MapHelper.h"

#include <qsf/renderer/particles/ParticlesComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier DestroyParticleAction::ACTION_ID = "em5::DestroyParticleAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DestroyParticleAction::DestroyParticleAction() :
		Action(ACTION_ID)
	{
		// Nothing here
	}

	DestroyParticleAction::~DestroyParticleAction()
	{
		// Nothing here
	}

	void DestroyParticleAction::init()
	{
		// TODO(co) Anything to do in here?
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void DestroyParticleAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(co) Anything to do in here?
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result DestroyParticleAction::updateAction(const qsf::Clock&)
	{
		qsf::ParticlesComponent * particle = getEntity().getComponent<qsf::ParticlesComponent>();
		if (particle == nullptr				// Lost it's particle component for some reason
			|| particle->isFinished()		// Finished normally
			)
		{
			// Destroy the entity
			MapHelper::destroyEntity(getEntity());

			return qsf::action::RESULT_DONE;
		}

		return qsf::action::RESULT_CONTINUE;
	}

	bool DestroyParticleAction::onStartup()
	{
		const qsf::ParticlesComponent* particlesComponent = getEntity().getComponent<qsf::ParticlesComponent>();
		if (particlesComponent == nullptr)
			return false; // Only for particles

		// Done
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
