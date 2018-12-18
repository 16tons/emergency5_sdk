// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/listener/TimeOfDayEditorListener.h"

#include <em5/environment/lighting/LightControlComponent.h>

#include <qsf_game/environment/time/TimeOfDayComponent.h>

#include <boost/bind.hpp>


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
		TimeOfDayEditorListener::TimeOfDayEditorListener(qsf::editor::EditorListenerManager* manager) :
			qsf::editor::EditorListener(manager)
		{
			mMessageProxy.registerAt(qsf::MessageConfiguration("qsf::editor::TimeOfDayUpdated"), boost::bind(&TimeOfDayEditorListener::onTimeOfDayUpdated, this, _1));
		}

		TimeOfDayEditorListener::~TimeOfDayEditorListener()
		{
			// Nothing to do here
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		void TimeOfDayEditorListener::onTimeOfDayUpdated(const qsf::MessageParameters& parameters)
		{
			// Update light state of all dynamic lighting
			LightControlComponent::updateAllLightStates(qsf::game::TimeOfDayComponent::getCurrentTimeOfDay(), true, true);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
