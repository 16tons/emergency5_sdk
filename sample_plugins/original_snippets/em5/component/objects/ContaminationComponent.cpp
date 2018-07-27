// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/ContaminationComponent.h"
#include "em5/component/objects/ContaminationCloudComponent.h"

#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/serialization/binary/BasicTypeSerialization.h>
#include <qsf/serialization/binary/StlTypeSerialization.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ContaminationComponent::COMPONENT_ID = qsf::StringHash("em5::ContaminationComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void ContaminationComponent::startSimulation()
	{
		if (isActive())
		{
			std::vector<ContaminationCloudComponent*> components;
			getContaminationCloudComponents(components);

			for (ContaminationCloudComponent* cloudComponent : components)
			{
				cloudComponent->startSimulation();
			}

			mContaminatedPersons = 0;
		}
	}

	void ContaminationComponent::stopSimulation()
	{
		// No check if component is active here

		std::vector<ContaminationCloudComponent*> components;
		getContaminationCloudComponents(components);

		for (ContaminationCloudComponent* cloudComponent : components)
		{
			cloudComponent->stopSimulation();
		}

		mContaminatedPersons = 0;
	}

	void ContaminationComponent::increaseContaminatedPersons()
	{
		// TODO(ca) For how long has this dead code been lying around?
		//   => If this is still commented after the release of em2017, this should be removed, because existing events in em5 and em2017 in their current state would break upon reactivation of this code.
		//   -> If you deactivate a feature like this, please make sure that the relevant property is removed from the component at once and later reintroduced with a default value that retains the current (disabled) behaviour for old maps/events.
		++mContaminatedPersons;
		if (mContaminationLimit == mContaminatedPersons)
		{
			// (mk) Outcommented, game design don't want this feature anymore. I keep it here to be able to fast react for additional design changes
			//stopSimulation();
		}
	}

	void ContaminationComponent::getContaminationCloudComponents(std::vector<ContaminationCloudComponent*>& components, bool showError) const
	{
		const qsf::Map& map = getEntity().getMap();
		for (uint64 entityId : mContaminationCloudIds)
		{
			const qsf::Entity* cloudEntity = map.getEntityById(entityId);
			if (nullptr != cloudEntity)
			{
				ContaminationCloudComponent* cloudComponent = cloudEntity->getComponent<ContaminationCloudComponent>();
				if (nullptr != cloudComponent)
				{
					components.push_back(cloudEntity->getComponent<ContaminationCloudComponent>());
				}
				else if (showError)
				{
					QSF_ERROR("The entity " << entityId << " is no contamination cloud entity", QSF_REACT_NONE);
				}
			}
			else if (showError)
			{
				QSF_ERROR("Contamination cloud entity " << entityId << " not found", QSF_REACT_NONE);
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void ContaminationComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint32>(mContaminationType);
		serializer.serialize(mContaminationCloudIds);
	}

	void ContaminationComponent::onSetDebug(bool debug)
	{
		std::vector<ContaminationCloudComponent*> components;
		getContaminationCloudComponents(components, false);

		for (ContaminationCloudComponent* cloudComponent : components)
		{
			cloudComponent->setDebug(debug);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
