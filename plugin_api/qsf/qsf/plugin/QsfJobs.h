// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/job/Jobs.h"
#include "qsf/base/NamedIdentifier.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Static QSF plugin jobs collection class
	*/
	class QSF_API_EXPORT QsfJobs : public Jobs
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class QsfPlugin;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Manually called
		static const NamedIdentifier OCCASION;							///< Job manager "Occasion"

		// Realtime
		static const NamedIdentifier REALTIME;							///< Job manager "Realtime"
			static const NamedIdentifier REALTIME_GENERAL;				///< Job manager "Realtime.General"
			static const NamedIdentifier REALTIME_DEBUGDRAW;			///< Job manager "Realtime.DebugDraw"
			static const NamedIdentifier REALTIME_RENDERER;				///< Job manager "Realtime.Renderer"
			static const NamedIdentifier REALTIME_EFFECTS;				///< Job manager "Realtime.Effects"
			static const NamedIdentifier REALTIME_CAMERA;				///< Job manager "Realtime.Camera"
			static const NamedIdentifier REALTIME_INPUT;				///< Job manager "Realtime.Input"
			static const NamedIdentifier REALTIME_GUI;					///< Job manager "Realtime.Gui"
			static const NamedIdentifier REALTIME_NETWORK;				///< Job manager "Realtime.Network"
				static const NamedIdentifier REALTIME_NETWORK_SERVER;	///< Job manager "Realtime.Network.Server"
				static const NamedIdentifier REALTIME_NETWORK_CLIENT;	///< Job manager "Realtime.Network.Client"
			static const NamedIdentifier REALTIME_AUDIO;				///< Job manager "Realtime.Audio"

		// Animation
		static const NamedIdentifier ANIMATION;							///< Job manager "Animation"
			static const NamedIdentifier ANIMATION_GENERAL;				///< Job manager "Animation.General"
			static const NamedIdentifier ANIMATION_MESH;				///< Job manager "Animation.Mesh"
			static const NamedIdentifier ANIMATION_PARTICLES;			///< Job manager "Animation.Particles"

		// Simulation
		static const NamedIdentifier SIMULATION;						///< Job manager "Simulation"
			static const NamedIdentifier SIMULATION_GENERAL;			///< Job manager "Simulation.General"
			static const NamedIdentifier SIMULATION_ACTION;				///< Job manager "Simulation.Action"
			static const NamedIdentifier SIMULATION_ANIMATION;			///< Job manager "Simulation.Animation"
			static const NamedIdentifier SIMULATION_PHYSICS;			///< Job manager "Simulation.Physics"
			static const NamedIdentifier SIMULATION_SCRIPT;				///< Job manager "Simulation.Script"


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		QsfJobs();

		/**
		*  @brief
		*    Destructor
		*
		*  @brief
		*    - Not virtual by intent
		*/
		~QsfJobs();

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
} // qsf
