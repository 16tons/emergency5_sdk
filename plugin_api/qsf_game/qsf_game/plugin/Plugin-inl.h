// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		inline Plugin* Plugin::instance()
		{
			return mGlobalPluginInstance;
		}

		inline Plugin& Plugin::getInstanceSafe()
		{
			QSF_CHECK(mGlobalPluginInstance, "Accessing uninitialized plugin instance", QSF_REACT_THROW);
			return *mGlobalPluginInstance;
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Plugin methods                    ]
		//[-------------------------------------------------------]
		inline CommandSystem& Plugin::getCommandSystem() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mCommandSystem, "The command system instance is invalid", QSF_REACT_THROW);
			return *mCommandSystem;
		}

		inline WeatherSystem& Plugin::getWeatherSystem() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mWeatherSystem, "The weather system instance is invalid", QSF_REACT_THROW);
			return *mWeatherSystem;
		}

		inline EquipmentSystem& Plugin::getEquipmentSystem() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mEquipmentSystem, "The equipment system instance is invalid", QSF_REACT_THROW);
			return *mEquipmentSystem;
		}

		inline CharacterAnimationSystem& Plugin::getCharacterAnimationSystem() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mCharacterAnimationSystem, "The character animation system instance is invalid", QSF_REACT_THROW);
			return *mCharacterAnimationSystem;
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Plugin methods                    ]
		//[-------------------------------------------------------]
		inline const char* Plugin::getName() const
		{
			return "QSF Game";
		}

		inline const char* Plugin::getVendor() const
		{
			return "Promotion Software GmbH";
		}

		inline const char* Plugin::getDescription() const
		{
			return "Quadriga Simulation Framework (QSF) game, contains high level functionality which is shared between multiple game projects";
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::SystemContainer methods           ]
		//[-------------------------------------------------------]
		const char* Plugin::getSystemContainerName() const
		{
			return getName();
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
