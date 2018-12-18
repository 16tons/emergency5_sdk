// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/filter/object/EventTagObjectBrowserFilter.h"

#include <qsf_game/component/event/EventTagComponent.h>

#include <qsf_editor/filter/object/ObjectBrowserFilterManager.h>
#include <qsf_editor/base/FilterConfiguration.h>

#include <qsf/map/Entity.h>


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
		const uint32 EventTagObjectBrowserFilter::PLUGINABLE_ID = qsf::StringHash("em5::editor::EventTagObjectBrowserFilter");


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		EventTagObjectBrowserFilter::EventTagObjectBrowserFilter(qsf::editor::ObjectBrowserFilterManager* objectBrowserFilterManager) :
			ObjectBrowserFilter(objectBrowserFilterManager)
		{
			// Nothing to do in here
		}

		EventTagObjectBrowserFilter::~EventTagObjectBrowserFilter()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::ObjectBrowserFilter methods    ]
		//[-------------------------------------------------------]
		bool EventTagObjectBrowserFilter::acceptsEntity(const QRegExp& filter, const qsf::Entity& entity)
		{
			// TODO(fw): Check filter configuration (needs to be extended for this...)
			//const qsf::editor::FilterConfiguration& filterConfiguration = getPluginableManager<qsf::editor::ObjectBrowserFilterManager>().getFilterConfiguration();

			const qsf::game::EventTagComponent* eventTagComponent = entity.getComponent<qsf::game::EventTagComponent>();
			if (nullptr != eventTagComponent)
			{
				// Check event tags
				if (QString::fromUtf8(eventTagComponent->getEventTagsAsString().c_str()).contains(filter))
				{
					// Show entity
					return true;
				}
			}

			// Did not accept entity
			return false;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
