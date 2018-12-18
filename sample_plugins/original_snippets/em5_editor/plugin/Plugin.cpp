// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/plugin/Plugin.h"
#include "em5_editor/dialog/AboutDialogEM5.h"
#include "em5_editor/view/FireMaterialsView.h"
#include "em5_editor/view/EventPoolsView.h"
#include "em5_editor/tool/FireSimulationTool.h"
#include "em5_editor/map/test/ConsistencyMapTest.h"
#include "em5_editor/map/test/CollisionMapTest.h"
#include "em5_editor/delegate/provider/FireMaterialDynamicEnumProvider.h"
#include "em5_editor/delegate/provider/InjuryDynamicEnumProvider.h"
#include "em5_editor/delegate/provider/MoverTypeDynamicEnumProvider.h"
#include "em5_editor/delegate/provider/GroundTypeDynamicEnumProvider.h"
#include "em5_editor/base/valueconverter/ValueConverterProvider.h"
#include "em5_editor/listener/TimeOfDayEditorListener.h"
#include "em5_editor/listener/WeatherCameraEditorListener.h"
#include "em5_editor/editmode/FireComponentEditMode.h"
#include "em5_editor/filter/object/EventTagObjectBrowserFilter.h"
#include "em5_editor/batchprocess/BatchJobLetItSnow.h"
#include "em5_editor/batchprocess/BatchJobGroundSpeedConverter.h"	// TODO(co) Remove this batch job when possible, was only added for editing migration
#include "em5_editor/batchprocess/BatchJobMigrateSkeletonCoordinateSystem.h"

#include <em5/modding/ModSystem.h>
#include <em5/plugin/version/PluginVersion.h>
#include <em5/EM5Helper.h>

#include <qsf_editor/reflection/CampQWidget.h>		// Required, else CAMP will not be able to create instances because the CAMP type "QWidget" will be unknown
#include <qsf_editor/editmode/EditModeManager.h>	// Required, else CAMP will not be able to create instances because the CAMP type "qsf::editor::EditModeManager" will be unknown
#include <qsf_editor/view/ViewManager.h>			// Required, else CAMP will not be able to create instances because the CAMP type "qsf::editor::ViewManager" will be unknown
#include <qsf_editor/tool/ToolManager.h>			// Required, else CAMP will not be able to create instances because the CAMP type "qsf::editor::ToolManager" will be unknown
#include <qsf_editor/map/test/MapTestManager.h>		// Required, else CAMP will not be able to create instances because the CAMP type "qsf::editor::MapTestManager" will be unknown
#include <qsf_editor/delegate/DynamicEnumManager.h>	// Required, else CAMP will not be able to create instances because the CAMP type "qsf::editor::DynamicEnumManager" will be unknown
#include <qsf_editor/listener/EditorListenerManager.h>	// Required, else CAMP will not be able to create instances because the CAMP type "qsf::editor::EditorListenerManager" will be unknown
#include <qsf_editor/filter/object/ObjectBrowserFilterManager.h>	// Required, else CAMP will not be able to create instances because the CAMP type "qsf::editor::ObjectBrowserFilterManager" will be unknown
#include <qsf_editor/plugin/Messages.h>

#include <qsf_editor_base/operation/CompoundOperation.h>
#include <qsf_editor_base/operation/component/SetComponentPropertyOperation.h>

#include <qsf_game/environment/time/TimeOfDayComponent.h>

#include <qsf/renderer/compositor/DefaultCompositingComponent.h>
#include <qsf/localization/SetBoostUtf8Locale.h>

#include <camp/classbuilder.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Private static data                                   ]
		//[-------------------------------------------------------]
		Plugin* Plugin::mGlobalPluginInstance = nullptr;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		Plugin::Plugin() :
			qsf::Plugin(new PluginVersion())
		{
			// Set Boost UTF-8 locale
			qsf::SetBoostUtf8Locale();
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Plugin methods                 ]
		//[-------------------------------------------------------]
		bool Plugin::onInstall()
		{
			// There should only exist one EM5 editor plugin instance at one and the same time
			QSF_CHECK(nullptr == mGlobalPluginInstance, "There should only exist one EM5 editor plugin instance at one and the same time",
				return true);

			// Make this EM5 editor plugin instance the global plugin instance
			mGlobalPluginInstance = this;

			try
			{
				// Declare CAMP reflection system classes
				// -> Use Qt's "QT_TR_NOOP()"-macro in order to enable Qt's "lupdate"-program to find the internationalization texts


				//[-------------------------------------------------------]
				//[ Dialog                                                ]
				//[-------------------------------------------------------]
				addCampClass(
					camp::Class::declare<AboutDialogEM5>()
						.tag("Name",		QT_TR_NOOP("ID_EM5EDITOR_DIALOG_ABOUT_NAME"))			// Text: "About"
						.tag("Description",	QT_TR_NOOP("ID_EM5EDITOR_DIALOG_ABOUT_DESCRIPTION"))	// Text: "About dialog"
						.base<qsf::editor::AboutDialog>()
						.constructor2<qsf::editor::DialogManager*, QWidget*>()
						.getClass()
					);


				//[-------------------------------------------------------]
				//[ View                                                  ]
				//[-------------------------------------------------------]
				addCampClass(
					camp::Class::declare<FireMaterialsView>()
						.tag("Name",		QT_TR_NOOP("ID_EM5EDITOR_VIEW_FIREMATERIALS_NAME"))			// Text: "EM5 Fire Materials"
						.tag("Description",	QT_TR_NOOP("ID_EM5EDITOR_VIEW_FIREMATERIALS_DESCRIPTION"))	// Text: "EMERGENCY 5 fire materials view"
						.tag("Shortcut",	"")															// Internal, no translation required
						.base<qsf::editor::View>()
						.constructor2<qsf::editor::ViewManager*, QWidget*>()
						.getClass()
					);

				// TODO(co) The EM5 event pools view will not be part of the first public EM5 editor release. Maybe there's no need for this view at all and we can remove it because events are edited via JSON files.
				/*
				addCampClass(
					camp::Class::declare<EventPoolsView>()
						.tag("Name",		QT_TR_NOOP("ID_EM5EDITOR_VIEW_EVENTPOOLS_NAME"))		// Text: "EM5 Event Pools (TODO)" // TODO(co) Implement EM5 event pools view
						.tag("Description",	QT_TR_NOOP("ID_EM5EDITOR_VIEW_EVENTPOOLS_DESCRIPTION"))	// Text: "EMERGENCY 5 event pools view"
						.tag("Shortcut",	"")														// Internal, no translation required
						.base<qsf::editor::View>()
						.constructor2<qsf::editor::ViewManager*, QWidget*>()
						.getClass()
					);
				*/


				//[-------------------------------------------------------]
				//[ Tool                                                  ]
				//[-------------------------------------------------------]
				addCampClass(
					camp::Class::declare<FireSimulationTool>()
						.tag("Name",		QT_TR_NOOP("ID_EM5EDITOR_TOOL_FIRESIMULATION_NAME"))		// Text: "EM5 Fire Simulation"
						.tag("Description",	QT_TR_NOOP("ID_EM5EDITOR_TOOL_FIRESIMULATION_DESCRIPTION"))	// Text: "EMERGENCY 5 fire simulation tool"
						.tag("Shortcut",	"")															// Internal, no translation required
						.base<qsf::editor::Tool>()
						.constructor1<qsf::editor::ToolManager*>()
						.getClass()
					);


				//[-------------------------------------------------------]
				//[ Map test                                              ]
				//[-------------------------------------------------------]
				addCampClass(
					camp::Class::declare<ConsistencyMapTest>()
						.tag("Name",		QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_CONSISTENCY_NAME"))		// Text: "EMERGENCY 5 Consistency"
						.tag("Description",	QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_CONSISTENCY_DESCRIPTION"))	// Text: "Map test checking the EMERGENCY 5 consistency inside the map"
						.base<qsf::editor::MapTest>()
						.constructor1<qsf::editor::MapTestManager*>()
						.getClass()
					);

				addCampClass(
					camp::Class::declare<CollisionMapTest>()
						.tag("Name",		QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_COLLISION_NAME"))			// Text: "EMERGENCY 5 Collision"
						.tag("Description",	QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_COLLISION_DESCRIPTION"))	// Text: "Map test checking the EMERGENCY 5 collision inside the map"
						.base<qsf::editor::MapTest>()
						.constructor1<qsf::editor::MapTestManager*>()
						.getClass()
					);


				//[-------------------------------------------------------]
				//[ Dynamic Enum Provider                                 ]
				//[-------------------------------------------------------]
				addCampClass(
					camp::Class::declare<FireMaterialDynamicEnumProvider>()
						.tag("Name",		QT_TR_NOOP("ID_EM5EDITOR_DYNAMICENUMPROVIDER_FIREMATERIAL_NAME"))			// Text: "Fire Material"
						.tag("Description", QT_TR_NOOP("ID_EM5EDITOR_DYNAMICENUMPROVIDER_FIREMATERIAL_DESCRIPTION"))	// Text: "Fire material name"
						.base<qsf::editor::DynamicEnumProvider>()
						.constructor1<qsf::editor::DynamicEnumManager*>()
						.getClass()
					);

				addCampClass(
					camp::Class::declare<InjuryDynamicEnumProvider>()
						.tag("Name",		QT_TR_NOOP("ID_EM5EDITOR_DYNAMICENUMPROVIDER_INJURY_NAME"))			// Text: "Injury"
						.tag("Description", QT_TR_NOOP("ID_EM5EDITOR_DYNAMICENUMPROVIDER_INJURY_DESCRIPTION"))	// Text: "Injury name"
						.base<qsf::editor::DynamicEnumProvider>()
						.constructor1<qsf::editor::DynamicEnumManager*>()
						.getClass()
					);

				addCampClass(
					camp::Class::declare<MoverTypeDynamicEnumProvider>()
						.tag("Name",		QT_TR_NOOP("ID_EM5EDITOR_DYNAMICENUMPROVIDER_MOVERTYPE_NAME"))			// Text: "MoverType"
						.tag("Description", QT_TR_NOOP("ID_EM5EDITOR_DYNAMICENUMPROVIDER_MOVERTYPE_DESCRIPTION"))	// Text: "MoverType name"
						.base<qsf::editor::DynamicEnumProvider>()
						.constructor1<qsf::editor::DynamicEnumManager*>()
						.getClass()
					);

				addCampClass(
					camp::Class::declare<GroundTypeDynamicEnumProvider>()
						.tag("Name",		QT_TR_NOOP("ID_EM5EDITOR_DYNAMICENUMPROVIDER_GROUNDTYPE_NAME"))			// Text: "GroundType"
						.tag("Description", QT_TR_NOOP("ID_EM5EDITOR_DYNAMICENUMPROVIDER_GROUNDTYPE_DESCRIPTION"))	// Text: "GroundType name"
						.base<qsf::editor::DynamicEnumProvider>()
						.constructor1<qsf::editor::DynamicEnumManager*>()
						.getClass()
					);


				//[-------------------------------------------------------]
				//[ Value Converter Provider                              ]
				//[-------------------------------------------------------]
				addCampClass(
					camp::Class::declare<ValueConverterProvider>()
						.tag("Name",		"Value Converter")	// Internal, no localization required
						.tag("Description", "Value converter")	// Internal, no localization required
						.base<qsf::editor::ValueConverterProvider>()
						.constructor1<qsf::editor::ValueConverterManager*>()
						.getClass()
					);


				//[-------------------------------------------------------]
				//[ Editor listener                                       ]
				//[-------------------------------------------------------]
				addCampClass(
					camp::Class::declare<TimeOfDayEditorListener>()
						.base<qsf::editor::EditorListener>()
						.constructor1<qsf::editor::EditorListenerManager*>()
						.getClass()
					);

				addCampClass(
					camp::Class::declare<WeatherCameraEditorListener>()
						.base<qsf::editor::EditorListener>()
						.constructor1<qsf::editor::EditorListenerManager*>()
						.getClass()
					);


				//[-------------------------------------------------------]
				//[ Edit modes                                            ]
				//[-------------------------------------------------------]
				addCampClass(
					camp::Class::declare<FireComponentEditMode>()
						.tag("Name",		QT_TR_NOOP("ID_EM5EDITOR_EDITMODE_FIRECOMPONENT_NAME"))			// Text: "Fire entity"
						.tag("Description",	QT_TR_NOOP("ID_EM5EDITOR_EDITMODE_FIRECOMPONENT_DESCRIPTION"))	// Text: "Fire entity edit mode"
						.base<qsf::editor::EditMode>()
						.constructor1<qsf::editor::EditModeManager*>()
						.getClass()
					);


				//[-------------------------------------------------------]
				//[ Object browser filter                                 ]
				//[-------------------------------------------------------]
				addCampClass(
					camp::Class::declare<EventTagObjectBrowserFilter>()
						.tag("Name",		"Event Tag Object Browser Filter")	// Internal, no localization required
						.tag("Description",	"Event tag object browser filter")	// Internal, no localization required
						.base<qsf::editor::ObjectBrowserFilter>()
						.constructor1<qsf::editor::ObjectBrowserFilterManager*>()
						.getClass()
					);


				//[-------------------------------------------------------]
				//[ Batch processing                                      ]
				//[-------------------------------------------------------]
				addCampClass(
					camp::Class::declare<BatchJobMigrateSkeletonCoordinateSystem>()
						.base<qsf::editor::BatchJob>()
						.constructor1<qsf::editor::BatchJobManager*>()
						.getClass()
					);

				addCampClass(
					camp::Class::declare<BatchJobMigrateSkeletonCoordinateSystemInverse>()
						.base<qsf::editor::BatchJob>()
						.constructor1<qsf::editor::BatchJobManager*>()
						.getClass()
					);

				addCampClass(
					camp::Class::declare<BatchJobLetItSnow>()
						.base<qsf::editor::BatchJob>()
						.constructor1<qsf::editor::BatchJobManager*>()
						.getClass()
					);

				// TODO(co) Remove this batch job when possible, was only added for editing migration
				addCampClass(
					camp::Class::declare<BatchJobGroundSpeedConverter>()
						.base<qsf::editor::BatchJob>()
						.constructor1<qsf::editor::BatchJobManager*>()
						.getClass()
					);


				// Done
				return true;
			}
			catch (const std::exception& e)
			{
				// Error!
				QSF_ERROR("Failed to install the EMERGENCY 5 editor plugins. Exception caught: " << e.what(), QSF_REACT_NONE);
				return false;
			}
		}

		bool Plugin::onStartup()
		{
			// TODO(fw): There may be better places to startup the mod projects
			//  -> Moving the mod system to qsf::game could make things easier here, the mounting should be done inside the editor application then
			EM5_MOD.mountEnabledMods();

			// Register message proxy
			mOnPreNewEmptyMapMessageProxy.registerAt(qsf::MessageConfiguration(qsf::editor::Messages::PRE_NEW_EMPTY_MAP), boost::bind(&Plugin::onPreNewEmptyMap, this, _1));

			// Done
			return true;
		}

		void Plugin::onShutdown()
		{
			// Unregister message proxy
			mOnPreNewEmptyMapMessageProxy.unregister();

			// TODO(fw): See comment in onStartup
			{
				ModSystem& modSystem = EM5_MOD;
				modSystem.gatherModStateFromProjects();
				modSystem.saveModUserSettings();
			}
		}

		void Plugin::onUninstall()
		{
			// Removing classes is not possible within the CAMP reflection system

			// In case this plugin instance is the global EM5 editor plugin instance (which should always be the case),
			// set the global EM5 editor plugin instance to a null pointer
			if (this == mGlobalPluginInstance)
			{
				mGlobalPluginInstance = nullptr;
			}
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		void Plugin::onPreNewEmptyMap(const qsf::MessageParameters& parameters)
		{
			// Adjust new empty maps according to EMERGENCY 5 standards
			qsf::editor::base::CompoundOperation* compoundOperation = nullptr;
			if (parameters.getParameter("compoundOperation", compoundOperation) && nullptr != compoundOperation)
			{
				// Time of day
				compoundOperation->pushBackOperation(new qsf::editor::base::SetComponentPropertyOperation(0, qsf::game::TimeOfDayComponent::COMPONENT_ID, qsf::game::TimeOfDayComponent::SEQUENCE, qsf::AssetProxy("em5/sequence/timeofday/timeofday_default").getGlobalAssetId()));

				{ // Map border
					// Define helper macros
					#define SET_PROPERTY(propertyId, propertyValue)	compoundOperation->pushBackOperation(new qsf::editor::base::SetComponentPropertyOperation(0, qsf::compositing::DefaultCompositingComponent::COMPONENT_ID, qsf::compositing::DefaultCompositingComponent::propertyId, propertyValue));

					// Set properties
					SET_PROPERTY(BORDER_FADE_SPEED,					 0.023f)
					SET_PROPERTY(BORDER_LINE_THICKNESS,				 0.1f);
					SET_PROPERTY(BORDER_LINE_COLOR,					 qsf::Color4(0.047f, 0.8431f, 1.0f, 1.0f));
					SET_PROPERTY(BORDER_LINE_EMISSIVE_INTENSITY,	 5.0f);
					SET_PROPERTY(BORDER_OUTER_COLOR,				 qsf::Color4(0.0f, 0.0f, 0.0f, 0.5f));
					SET_PROPERTY(BORDER_TEXTURE,					 qsf::AssetProxy("em5/texture/default/em5_mapborder01_c").getGlobalAssetId());
					SET_PROPERTY(BORDER_TEXTURE_SCALING,			 20.0f);
					SET_PROPERTY(BORDER_TEXTURE_SCROLL_SPEEDX,		 0.05f);
					SET_PROPERTY(BORDER_TEXTURE_BLEND_COLOR,		 qsf::Color4(0.0f, 0.0f, 0.0f, 0.0f));
					SET_PROPERTY(BORDER_TEXTURE_INTENSITY,			 0.2);
					SET_PROPERTY(BORDER_TEXTURE_EMISSIVE_INTENSITY,	 20.0f);
					SET_PROPERTY(BORDER_TEXTURE2,					 qsf::AssetProxy("em5/texture/default/em5_mapborder02_c").getGlobalAssetId());
					SET_PROPERTY(BORDER_TEXTURE2_SCALING,			 70.0f);
					SET_PROPERTY(BORDER_TEXTURE2_SCROLL_SPEEDX,		 0.0f);
					SET_PROPERTY(BORDER_TEXTURE2_SCROLL_SPEEDY,		 0.07f);
					SET_PROPERTY(BORDER_TEXTURE2_BLEND_COLOR,		 qsf::Color4(0.0f, 0.0f, 0.0f, 0.0f));
					SET_PROPERTY(BORDER_TEXTURE2_INTENSITY,			 1.0f);
					SET_PROPERTY(BORDER_TEXTURE2_EMISSIVE_INTENSITY, 2.0f);

					// Undefine helper macros
					#undef SET_PROPERTY
				}
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
