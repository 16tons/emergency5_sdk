// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/base/valueconverter/ValueConverterProvider.h"

#include <em5/component/door/DoorComponent.h>

#include <qsf_editor/mimedata/EntitiesMimeData.h>

#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/reflection/CampHelper.h>
#include <qsf/reflection/ClassSystem.h>
#include <qsf/reflection/type/CampGlmVec3.h>
#include <qsf/QsfHelper.h>

#include <QtCore/qmimedata.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		ValueConverterProvider::ValueConverterProvider(qsf::editor::ValueConverterManager* valueConverterManager) :
			qsf::editor::ValueConverterProvider(valueConverterManager)
		{
			// Nothing here
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::ValueConverterProvider methods ]
		//[-------------------------------------------------------]
		bool ValueConverterProvider::convertQtMimeDataToPropertyValueAsString(const qsf::Object& targetObject, int targetPropertyIndex, const QMimeData& qMimeData, std::string& valueAsString) const
		{
			// Check for entity
			if (qMimeData.hasFormat(qsf::editor::EntitiesMimeData::MIME_TYPE))
			{
				qsf::editor::EntitySelectionManager::IdSet idSet;
				static_cast<const qsf::editor::EntitiesMimeData&>(qMimeData).getEntityIdSet(idSet);
				if (!idSet.empty())
				{
					// Get entity transform
					const qsf::Entity* entity = QSF_MAINMAP.getEntityById(*idSet.begin());
					if (nullptr != entity)
					{
						const qsf::TransformComponent* transformComponent = entity->getComponent<qsf::TransformComponent>();
						if (nullptr != transformComponent)
						{
							// Is the given object an door component instance?
							static const camp::Class& doorComponentCampClass = camp::classByType<DoorComponent>();
							if (QSF_CLASS.isCampClassEqualOrDerived(targetObject.getCampClass(), doorComponentCampClass))
							{
								// Check for "DoorOffset" or "DoorRotation" property
								const uint32 propertyIdBy = targetObject.getPropertyIdByIndex(targetPropertyIndex);
								if (DoorComponent::DOOR_OFFSET == propertyIdBy || DoorComponent::DOOR_ROTATION == propertyIdBy)
								{
									// Get transform
									qsf::Transform originalLocalToWorld;
									static_cast<const DoorComponent&>(targetObject).getLocalToWorldTransform(originalLocalToWorld);

									// Get value as string
									if (DoorComponent::DOOR_OFFSET == propertyIdBy)
									{
										valueAsString = qsf::CampHelper::campValueToString(originalLocalToWorld.vec3PositionWorldToLocal(transformComponent->getPosition()));

										// The mapping was successful
										return true;
									}
									else if (DoorComponent::DOOR_ROTATION == propertyIdBy)
									{
										valueAsString = qsf::CampHelper::campValueToString(originalLocalToWorld.quatRotationWorldToLocal(transformComponent->getRotation()));

										// The mapping was successful
										return true;
									}
								}
							}
						}
					}
				}
			}

			// The mapping wasn't successful
			return false;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
