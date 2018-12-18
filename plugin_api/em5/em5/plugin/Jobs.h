// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/job/Jobs.h>
#include <qsf/base/NamedIdentifier.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Static EMERGENCY 5 plugin jobs collection class
	*/
	class EM5_API_EXPORT Jobs : public qsf::Jobs
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
		static const qsf::NamedIdentifier REALTIME_GUI_DEBUG;				///< Job manager "Realtime.Gui.Debug"
		static const qsf::NamedIdentifier REALTIME_GUI_MESSAGEBOX;			///< Job manager "Realtime.Gui.MessageBox"
		static const qsf::NamedIdentifier REALTIME_INVIDEOSYNC;				///< Job manager "Realtime.InVideoSync"

		// Animation
		static const qsf::NamedIdentifier ANIMATION_VEHICLE;				///< Job manager "Animation.Vehicle"
		static const qsf::NamedIdentifier ANIMATION_PERSON;					///< Job manager "Animation.Person"
		static const qsf::NamedIdentifier ANIMATION_TREE;					///< Job manager "Animation.Tree"
		static const qsf::NamedIdentifier ANIMATION_WATERJET;				///< Job manager "Animation.Waterjet"
		static const qsf::NamedIdentifier ANIMATION_FIREHOSE;				///< Job manager "Animation.Firehose"
		static const qsf::NamedIdentifier ANIMATION_HIGHLIGHT;				///< Job manager "Animation.Highlight"

		// Simulation
		static const qsf::NamedIdentifier SIMULATION_VEHICLE;				///< Job manager "Simulation.Vehicle"
		static const qsf::NamedIdentifier SIMULATION_HIGHLIGHT;				///< Job manager "Simulation.Highlight"
		static const qsf::NamedIdentifier SIMULATION_HEALTH;				///< Job manager "Simulation.Health"
		static const qsf::NamedIdentifier SIMULATION_FIRE;					///< Job manager "Simulation.Fire"
		static const qsf::NamedIdentifier SIMULATION_LIGHTING;				///< Job manager "Simulation.Lighting"
		static const qsf::NamedIdentifier SIMULATION_LOGIC;					///< Job manager "Simulation.Logic"
		static const qsf::NamedIdentifier SIMULATION_SPAWN_PERSONS;			///< Job manager "Simulation.Spawn.Persons"
		static const qsf::NamedIdentifier SIMULATION_SPAWN_VEHICLES;		///< Job manager "Simulation.Spawn.Vehicles"
		static const qsf::NamedIdentifier SIMULATION_CLEANUP;				///< Job manager "Simulation.Cleanup"
		static const qsf::NamedIdentifier SIMULATION_MULTIPLAYER_HOST;		///< Job manager "Simulation.MultiplayerHost"
		static const qsf::NamedIdentifier SIMULATION_MULTIPLAYER_CLIENT;	///< Job manager "Simulation.MultiplayerClient"

		// All job managers must be sorted into either REALTIME, ANIMATION or SIMULATION - choose wisely


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
} // em5
