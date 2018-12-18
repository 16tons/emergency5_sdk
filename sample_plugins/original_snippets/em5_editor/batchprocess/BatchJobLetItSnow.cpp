// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/batchprocess/BatchJobLetItSnow.h"

#include <em5/activity/ActivityComponent.h>
#include <em5/component/vehicle/BoatComponent.h>

#include <qsf_editor/EditorHelper.h>
#include <qsf_editor/selection/entity/EntitySelectionManager.h>

#include <qsf_editor_base/operation/CompoundOperation.h>
#include <qsf_editor_base/operation/component/CreateComponentOperation.h>
#include <qsf_editor_base/operation/component/SetComponentPropertyOperation.h>
#include <qsf_editor_base/operation/entity/SelectEntityHiddenOperation.h>
#include <qsf_editor_base/operation/entity/DeselectEntityHiddenOperation.h>

#include <qsf_game/component/base/DecorationComponent.h>
#include <qsf_game/component/person/PersonComponent.h>
#include <qsf_game/component/health/HealthComponent.h>
#include <qsf_game/environment/weather/SnowComponent.h>

#include <qsf/QsfHelper.h>
#include <qsf/math/Random.h>
#include <qsf/map/Entity.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/localization/LocalizationSystem.h>
#include <qsf/renderer/mesh/MeshComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		// Qt translation needs always an context/scope for the text which should be translatable
		// For the QT_TR_NOOP macro the lupdate tool checks if the class which contains the macro call has an Q_OBJECT definition (if so the class name is used as the scope)
		// With the QT_TRANSLATE_NOOP macro the needed scope is specified to the macro call -> the containing scope/class doesn't need specify/contains the Q_OBJECT macro
		const char BatchJobLetItSnow::JOB_TEXT[] = QT_TRANSLATE_NOOP("em5::editor", "ID_BATCH_JOB_LET_IT_SNOW");


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		BatchJobLetItSnow::BatchJobLetItSnow(qsf::editor::BatchJobManager* manager) :
			qsf::editor::BatchJob(manager)
		{
			// Nothing to do in here
		}

		BatchJobLetItSnow::~BatchJobLetItSnow()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::BatchJobBase methods      ]
		//[-------------------------------------------------------]
		std::string BatchJobLetItSnow::getText() const
		{
			return QSF_TRANSLATE_CONTEXTSTRING("em5::editor", JOB_TEXT);
		}

		void BatchJobLetItSnow::getProgress(uint32& current, uint32& total) const
		{
			current = 0;
			total = 0;
		}

		bool BatchJobLetItSnow::configure()
		{
			// Make sure we have a clean state
			cleanup();
			return true;
		}

		bool BatchJobLetItSnow::work()
		{
			// Find all mesh components
			const auto& meshComponents = qsf::ComponentMapQuery(QSF_MAINMAP).getAllInstances<qsf::MeshComponent>();
			if (!meshComponents.empty())
			{
				// Collect all entities to perform the snow operation on
				std::unordered_set<qsf::Entity*> entities;
				for (const qsf::MeshComponent* meshComponent : meshComponents)
				{
					// Do only take top-level entities into account
					qsf::Entity& entity = static_cast<qsf::Entity&>(qsf::LinkComponent::getTopmostAncestorPrototype(meshComponent->getEntity(), qsf::LinkComponent::SELECT_PARENT));

					// Ignore persons and decoration
					if (nullptr == entity.getComponent<qsf::game::PersonComponent>() &&
						nullptr == entity.getComponent<qsf::game::HealthComponent>() &&
						nullptr == entity.getComponent<qsf::game::DecorationComponent>() &&
						nullptr == entity.getComponent<BoatComponent>() &&
						nullptr == entity.getComponent<ActivityComponent>())
					{
						entities.insert(&entity);
					}
				}

				// Apply snow to entities
				if (!entities.empty())
				{
					// Use a single compound operation to perform all the required changes
					qsf::editor::base::CompoundOperation* compoundOperation = new qsf::editor::base::CompoundOperation();
					compoundOperation->setText("Let it snow batch conversion");

					qsf::editor::EntitySelectionManager& entitySelectionManager = QSF_EDITOR_SELECTION_SYSTEM.getSafe<qsf::editor::EntitySelectionManager>();
					for (qsf::Entity* entity : entities)
					{
						// TODO(co) I'm sure that the hidden entity selection is not optimal, yet (e.g. situations were another user has the lock and hence we can't convert the entity)
						// For the online editor server, we need to request entity selections, else our operations will be rejected
						const uint64 entityId = entity->getId();
						qsf::editor::base::SelectEntityHiddenOperation* selectEntityHiddenOperation = entitySelectionManager.createSelectEntityHiddenOperation(entityId);
						QSF_ASSERT(nullptr != selectEntityHiddenOperation, "Could not lock selected entity", continue);
						compoundOperation->pushBackOperation(selectEntityHiddenOperation);

						// Some random snow amount
						const float snowAmount = qsf::Random::getRandomFloat(0.75f, 0.99f);

						// Push the operation
						if (nullptr == entity->getComponent<qsf::game::SnowComponent>())
						{
							compoundOperation->pushBackOperation(new qsf::editor::base::CreateComponentOperation(entityId, qsf::game::SnowComponent::COMPONENT_ID));
						}
						compoundOperation->pushBackOperation(new qsf::editor::base::SetComponentPropertyOperation(entityId, qsf::game::SnowComponent::COMPONENT_ID, qsf::game::SnowComponent::ACTIVE, true));
						compoundOperation->pushBackOperation(new qsf::editor::base::SetComponentPropertyOperation(entityId, qsf::game::SnowComponent::COMPONENT_ID, qsf::game::SnowComponent::SNOW_AMOUNT, snowAmount));

						// Unlock the selected entity
						compoundOperation->pushBackOperation(entitySelectionManager.createDeselectEntityHiddenOperation(entityId));
					}

					// Commit the operation
					QSF_EDITOR_OPERATION.push(compoundOperation);
				}
			}

			return false;
		}

		void BatchJobLetItSnow::cleanup()
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
