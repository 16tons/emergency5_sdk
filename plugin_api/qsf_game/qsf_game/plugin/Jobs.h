// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/job/Jobs.h>
#include <qsf/base/NamedIdentifier.h>


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
		*    Static QSF game plugin jobs collection class
		*/
		class QSF_GAME_API_EXPORT Jobs : public qsf::Jobs
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class Plugin;


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			// Realtime
			static const NamedIdentifier REALTIME_GAMECLOCK;	///< Job manager "Realtime.GameClock"

			// Simulation
			static const NamedIdentifier SIMULATION_TIMEOFDAY;	///< Job manager "Simulation.TimeOfDay"

			// Animation
			static const NamedIdentifier ANIMATION_MARKER;		///< Job manager "Animation.Marker"


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Default constructor
			*/
			Jobs();

			/**
			*  @brief
			*    Destructor
			*
			*  @brief
			*    - Not virtual by intent
			*/
			~Jobs();

			/**
			*  @brief
			*    Register the job managers
			*/
			static void registerJobManagers();

			/**
			*  @brief
			*    Unregister the job managers
			*/
			static void unregisterJobManagers();


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
