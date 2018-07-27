// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "editor_plugin/PrecompiledHeader.h"
#include "editor_plugin/plugin/Plugin.h"
#include "editor_plugin/view/indicator/IndicatorView.h"

#include <em5/plugin/version/PluginVersion.h>
#include <em5/reflection/CampDefines.h>

#include <qsf_editor/reflection/CampQWidget.h>		// Required, else CAMP will not be able to create instances because the CAMP type "QWidget" will be unknown
#include <qsf_editor/editmode/EditModeManager.h>	// Required, else CAMP will not be able to create instances because the CAMP type "qsf::editor::EditModeManager" will be unknown
#include <qsf_editor/view/ViewManager.h>			// Required, else CAMP will not be able to create instances because the CAMP type "qsf::editor::ViewManager" will be unknown
#include <qsf_editor/tool/ToolManager.h>			// Required, else CAMP will not be able to create instances because the CAMP type "qsf::editor::ToolManager" will be unknown
#include <qsf_editor/map/test/MapTestManager.h>		// Required, else CAMP will not be able to create instances because the CAMP type "qsf::editor::MapTestManager" will be unknown
#include <qsf_editor/delegate/DynamicEnumManager.h>	// Required, else CAMP will not be able to create instances because the CAMP type "qsf::editor::DynamicEnumManager" will be unknown
#include <qsf_editor/listener/EditorListenerManager.h>	// Required, else CAMP will not be able to create instances because the CAMP type "qsf::editor::EditorListenerManager" will be unknown
#include <qsf_editor/filter/object/ObjectBrowserFilterManager.h>	// Required, else CAMP will not be able to create instances because the CAMP type "qsf::editor::ObjectBrowserFilterManager" will be unknown


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		Plugin::Plugin() :
			qsf::Plugin(new em5::PluginVersion())
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Plugin methods                 ]
		//[-------------------------------------------------------]
		bool Plugin::onInstall()
		{
			try
			{
				// Declare CAMP reflection system classes
				// -> Use Qt's "QT_TR_NOOP()"-macro in order to enable Qt's "lupdate"-program to find the internationalization texts
				
				addCampClass(
					camp::Class::declare<IndicatorView>()
					.tag("Name", QT_TR_NOOP("ID_USEREDITOR_VIEW_INDICATORVIEW_NAME"))			// Text: "Indicator Browser"
					.tag("Description", QT_TR_NOOP("ID_USEREDITOR_VIEW_INDICATORVIEW_DESCRIPTION"))	// Text: "Indicator browser"
					.tag("Shortcut", "")															// Internal, no translation required
					.base<qsf::editor::View>()
					.constructor2<qsf::editor::ViewManager*, QWidget*>()
					.getClass()
				);

				// Done
				return true;
			}
			catch (const std::exception& e)
			{
				// Error!
				QSF_ERROR("Failed to install the plugin '" << getName() << "'. Exception caught: " << e.what(), QSF_REACT_NONE);
				return false;
			}
		}

		bool Plugin::onStartup()
		{
			// Nothing to do in here

			// Done
			return true;
		}

		void Plugin::onShutdown()
		{
			// Nothing to do in here
		}

		void Plugin::onUninstall()
		{
			// Removing classes is not possible within the CAMP reflection system

			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // user
