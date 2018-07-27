// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/plugin/Plugin.h"
#include "game_plugin/action/CheerAction.h"
#include "game_plugin/component/IndicatorComponent.h"
#include "game_plugin/command/CheerCommand.h"
#include "game_plugin/event/MedicalCheerEventFactory.h"
#include "game_plugin/event/MedicalCheerEvent.h"
#include "game_plugin/game/GameListener.h"
#include "game_plugin/logic/observer/CheeringObserver.h"

#include <em5/network/multiplayer/datacache/DataCacheItemFactory.h>
#include <em5/plugin/version/PluginVersion.h>
#include <em5/reflection/CampDefines.h>

#include <qsf_game/command/CommandManager.h>		// This is needed for commands; without this include, the build will work, but creation of commands fail

#include <qsf/map/Entity.h>							// Needed to avoid "error C2664: 'user::IndicatorComponent::IndicatorComponent(const user::IndicatorComponent &)' : cannot convert argument 1 from 'qsf::Entity *' to 'qsf::Prototype *' (game_plugin\plugin\Plugin.cpp)"
#include <qsf/localization/LocalizationSystem.h>	// Defines the "QT_TR_NOOP()"-macro


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
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


			//[-------------------------------------------------------]
			//[ Component                                             ]
			//[-------------------------------------------------------]
			QSF_CAMP_START_ENUM_EXPORT(user::IndicatorComponent::Color)
				QSF_CAMP_ENUM_VALUE(WHITE)
				QSF_CAMP_ENUM_VALUE(RED)
				QSF_CAMP_ENUM_VALUE(YELLOW)
				QSF_CAMP_ENUM_VALUE(GREEN)
				QSF_CAMP_ENUM_VALUE(BLUE)
			QSF_CAMP_END_ENUM_EXPORT

			QSF_START_CAMP_CLASS_EXPORT(user::IndicatorComponent, QT_TR_NOOP("ID_USER_COMPONENT_INDICATOR_NAME"), QT_TR_NOOP("ID_USER_COMPONENT_INDICATOR_DESCRIPTION"))
				QSF_CAMP_IS_COMPONENT
				QSF_ADD_CAMP_PROPERTY(Color, IndicatorComponent::getColor, IndicatorComponent::setColor, QT_TR_NOOP("ID_USER_COMPONENT_INDICATOR_COLOR_DESCRIPTION"), IndicatorComponent::Color::WHITE)
				QSF_ADD_CAMP_PROPERTY(IsAnimated, IndicatorComponent::isAnimated, IndicatorComponent::setIsAnimated, QT_TR_NOOP("ID_USER_COMPONENT_INDICATOR_ISANIMATED_DESCRIPTION"), false)
			QSF_END_CAMP_CLASS_EXPORT


			//[-------------------------------------------------------]
			//[ Command                                               ]
			//[-------------------------------------------------------]
			QSF_START_CAMP_CLASS_EXPORT(user::CheerCommand, "CheerCommand", "Cheer command")
				QSF_GAME_CAMP_IS_COMMAND
			QSF_END_CAMP_CLASS_EXPORT


			//[-------------------------------------------------------]
			//[ Action                                                ]
			//[-------------------------------------------------------]
			QSF_START_CAMP_CLASS_EXPORT(user::CheerAction, "CheerAction", "Cheer action")
				QSF_CAMP_IS_ACTION
			QSF_END_CAMP_CLASS_EXPORT


			//[-------------------------------------------------------]
			//[ Observer                                              ]
			//[-------------------------------------------------------]
			QSF_START_CAMP_CLASS_EXPORT(user::CheeringObserver, "CheeringObserver", "Cheering observer")
				QSF_CAMP_IS_GAME_LOGIC
			QSF_END_CAMP_CLASS_EXPORT


			//[-------------------------------------------------------]
			// Event                                                  ]
			//[-------------------------------------------------------]
			QSF_START_CAMP_CLASS_EXPORT(user::MedicalCheerEventFactory, "MedicalCheerEventFactory", "Medical cheer event factory")
				EM5_CAMP_IS_FREEPLAY_EVENT_FACTORY
				QSF_ADD_CAMP_PROPERTY(Age, user::MedicalCheerEventFactory::getAgeAsString, user::MedicalCheerEventFactory::setAgeAsString, "Age", "all")
				QSF_ADD_CAMP_PROPERTY(Weather, user::MedicalCheerEventFactory::getWeatherAsString, user::MedicalCheerEventFactory::setWeatherAsString,"Weather", "all")
				QSF_ADD_CAMP_PROPERTY(Inside, user::MedicalCheerEventFactory::getInsideBuilding, user::MedicalCheerEventFactory::setInsideBuilding, "Inside", false)
			QSF_END_CAMP_CLASS_EXPORT

			QSF_START_CAMP_CLASS_EXPORT(user::MedicalCheerEvent, "MedicalCheerEvent", "Medical cheer event")
				EM5_CAMP_IS_FREEPLAY_EVENT
				QSF_ADD_CAMP_PROPERTY(Injury, user::MedicalCheerEvent::getInjuryName, user::MedicalCheerEvent::setInjuryName, "Injury", "")
				QSF_ADD_CAMP_PROPERTY(RunningDelay, user::MedicalCheerEvent::getRunningDelay, user::MedicalCheerEvent::setRunningDelay, "RunningDelay", false)
				QSF_ADD_CAMP_PROPERTY(RescueWithHelicopter, user::MedicalCheerEvent::getRescueWithHelicopter, user::MedicalCheerEvent::setRescueWithHelicopter, "REwscueWithHElicopter", false)
				QSF_ADD_CAMP_PROPERTY(EventLayer, user::MedicalCheerEvent::getEventLayer, user::MedicalCheerEvent::setEventLayer, "EventLayer", "")
				QSF_ADD_CAMP_PROPERTY(SecondEventTag, user::MedicalCheerEvent::getSecondEventTag, user::MedicalCheerEvent::setSecondEventTag, "SecondEventTag", "")
			QSF_END_CAMP_CLASS_EXPORT

			// DoneBroken_Arm
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
		// Use this as entry point for all modding activities
		// Feel free to add more system relevant managers or systems here
		// It is also possible to add input handler to create a unique input behavior

		mGameListener = new GameListener();
		mGameListener->init();

		// Create and register a "DataCacheItemFactory" listener
		//  -> This is needed to sync the indicator component in multiplayer games
		em5::multiplayer::DataCacheItemFactory::addListener(mDataCacheItemFactoryListener);

		// Done
		return true;
	}

	void Plugin::onShutdown()
	{
		// Unregister our "DataCacheItemFactory" listener
		em5::multiplayer::DataCacheItemFactory::removeListener(mDataCacheItemFactoryListener);

		// Use this as to shutdown all modding relevant classes
		QSF_SAFE_DELETE(mGameListener);
	}

	void Plugin::onUninstall()
	{
		// Removing classes is not possible within the CAMP reflection system

		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user
