// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/batchprocess/BatchJobGroundSpeedConverter.h"

#include <qsf_editor/EditorHelper.h>
#include <qsf_editor/selection/entity/EntitySelectionManager.h>

#include <qsf_editor_base/operation/CompoundOperation.h>
#include <qsf_editor_base/operation/component/SetComponentPropertyOperation.h>
#include <qsf_editor_base/operation/entity/SelectEntityHiddenOperation.h>
#include <qsf_editor_base/operation/entity/DeselectEntityHiddenOperation.h>

#include <qsf/QsfHelper.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/localization/LocalizationSystem.h>
#include <qsf/physics/PhysicsSystem.h>
#include <qsf/physics/collision/BulletCollisionComponent.h>
#include <qsf/physics/collision/CollisionTypeManager.h>


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
		const char BatchJobGroundSpeedConverter::JOB_TEXT[] = QT_TRANSLATE_NOOP("em5::editor", "ID_BATCH_JOB_GROUND_SPEED_CONVERTER");


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		BatchJobGroundSpeedConverter::BatchJobGroundSpeedConverter(qsf::editor::BatchJobManager* manager) :
			BatchJob(manager)
		{
			// Nothing to do in here
		}

		BatchJobGroundSpeedConverter::~BatchJobGroundSpeedConverter()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::BatchJobBase methods      ]
		//[-------------------------------------------------------]
		std::string BatchJobGroundSpeedConverter::getText() const
		{
			return QSF_TRANSLATE_CONTEXTSTRING("em5::editor", JOB_TEXT);
		}

		void BatchJobGroundSpeedConverter::getProgress(uint32& current, uint32& total) const
		{
			current = 0;
			total = 0;
		}

		bool BatchJobGroundSpeedConverter::configure()
		{
			// Make sure we have a clean state
			cleanup();
			return true;
		}

		bool BatchJobGroundSpeedConverter::work()
		{
			const auto& bulletCollisionComponents = qsf::ComponentMapQuery(QSF_MAINMAP).getAllInstances<qsf::BulletCollisionComponent>();
			if (!bulletCollisionComponents.empty())
			{
				// Use a single compound operation to perform all the required changes
				qsf::editor::base::CompoundOperation* compoundOperation = new qsf::editor::base::CompoundOperation();
				compoundOperation->setText("Ground speed batch conversion");

				// Collect operations to perform
				qsf::editor::EntitySelectionManager& entitySelectionManager = QSF_EDITOR_SELECTION_SYSTEM.getSafe<qsf::editor::EntitySelectionManager>();
				for (qsf::BulletCollisionComponent* bulletCollisionComponent : bulletCollisionComponents)
				{
					// TODO(co) I'm sure that the hidden entity selection is not optimal, yet (e.g. situations were another user has the lock and hence we can't convert the entity)
					// For the online editor server, we need to request entity selections, else our operations will be rejected
					qsf::editor::base::SelectEntityHiddenOperation* selectEntityHiddenOperation = entitySelectionManager.createSelectEntityHiddenOperation(bulletCollisionComponent->getEntityId());
					QSF_ASSERT(nullptr != selectEntityHiddenOperation, "Could not lock selected entity", continue);
					compoundOperation->pushBackOperation(selectEntityHiddenOperation);

					// See https://xwiki.promotion-software.de/xwiki/bin/view/EM5/Collision+Flags for EMERGENCY 5 collision editing information
					const short collisionFilterGroup = bulletCollisionComponent->getCollisionFilterGroup();
					const short collisionMask = bulletCollisionComponent->getCollisionMask();

					// TODO(co) Is it possible to map other editing as well? (please note that "Barricade" is the default ground speed setting of collision components)
					std::string groundSpeed = "Barricade";

					// Shallow water plane
					if (6144 == collisionFilterGroup && 64 == collisionMask)
					{
						groundSpeed = "Shallow Water";
					}

					// Deep water plane
					else if (6144 == collisionFilterGroup && 448 == collisionMask)
					{
						groundSpeed = "Deep Water";
					}

					{ // Set collision type ID
						// Loop through all collision type instances
						const qsf::CollisionTypeManager::ElementMap& elements = QSF_PHYSICS.getCollisionTypeManager().getElements();
						for (const auto& collisionTypeElement : elements)
						{
							const qsf::CollisionType* collisionType = collisionTypeElement.second;
							if (collisionType->getCollisionFilterGroup() == collisionFilterGroup && collisionType->getCollisionMask() == collisionMask)
							{
								// Push the operation
								compoundOperation->pushBackOperation(new qsf::editor::base::SetComponentPropertyOperation(*bulletCollisionComponent, qsf::BulletCollisionComponent::COLLISION_TYPE_ID, collisionType->getId()));

								// Get us out of the loop
								break;
							}
						}
					}

					// Push the operation
					compoundOperation->pushBackOperation(new qsf::editor::base::SetComponentPropertyOperation(*bulletCollisionComponent, qsf::CollisionComponent::GROUND_SPEED, groundSpeed));

					// Unlock the selected entity
					compoundOperation->pushBackOperation(entitySelectionManager.createDeselectEntityHiddenOperation(bulletCollisionComponent->getEntityId()));
				}

				// Commit the operation
				QSF_EDITOR_OPERATION.push(compoundOperation);
			}

			return false;
		}

		void BatchJobGroundSpeedConverter::cleanup()
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
