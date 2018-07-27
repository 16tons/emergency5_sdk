// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/application/WindowApplication.h>

#include <boost/property_tree/ptree.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Game application base class
		*/
		class QSF_GAME_API_EXPORT GameApplication : public WindowApplication
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			GameApplication();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~GameApplication();

			/**
			*  @brief
			*    Ask the application politely to shut down as soon as possible
			*/
			inline void exit();

			/**
			*  @brief
			*    Get installation version string, e.g. "1.0.0"
			*/
			const std::string& getInstallationVersion() const;

			/**
			*  @brief
			*    Save all settings
			*/
			void saveSettings();

			/**
			*  @brief
			*    Call the updateRealTimeClock function
			*/
			void realTimeClockUpdate();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::game::GameApplication methods  ]
		//[-------------------------------------------------------]
		protected:
			virtual void onVersionJsonLoaded(const boost::property_tree::ptree& pTree);


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			std::string getAbsoluteSettingsPrefix() const;

			void initInstallationVersion(const std::string& applicationName, const std::string& binariesVersion);
			void loadApplicationProperties();
			bool loadDeveloperParameters(const std::string& filename, boost::property_tree::ptree& parametersPTree) const;


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			bool		mExit;					///< If "true", shut down the application as soon as possible, else keep on running
			std::string	mInstallationVersion;	///< Installed version of either application or content - whatever is newer


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/application/GameApplication-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::GameApplication)
