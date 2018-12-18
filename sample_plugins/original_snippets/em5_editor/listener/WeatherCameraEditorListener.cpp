// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/listener/WeatherCameraEditorListener.h"

#include <qsf_game/environment/weather/WeatherComponent.h>

#include <qsf/renderer/component/CameraComponent.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/QsfHelper.h>

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
		WeatherCameraEditorListener::WeatherCameraEditorListener(qsf::editor::EditorListenerManager* manager) :
			qsf::editor::EditorListener(manager)
		{
			mCameraChangedMessageProxy.registerAt(qsf::MessageConfiguration("qsf::editor::CameraChanged"), boost::bind(&WeatherCameraEditorListener::onCameraChanged, this, _1));
		}

		WeatherCameraEditorListener::~WeatherCameraEditorListener()
		{
			// Nothing to do here
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		void WeatherCameraEditorListener::onCameraChanged(const qsf::MessageParameters& parameters)
		{
			// Inform the weather component which needs the correct camera
			qsf::game::WeatherComponent* weatherComponent = QSF_MAINMAP.getCoreEntity().getComponent<qsf::game::WeatherComponent>();
			if (nullptr != weatherComponent)
			{
				qsf::CameraComponent* cameraComponent = nullptr;
				if (parameters.getParameter("CameraComponent", cameraComponent))
				{
					weatherComponent->setCameraComponent(cameraComponent);
				}
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
