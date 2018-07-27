// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"
#include "qsf/asset/AssetSystemTypes.h"
#include "qsf/job/JobProxy.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace ParticleUniverse
{
	class ParticleUniversePlugin;
}
namespace qsf
{
	class FileCache;
	class AssetSystem;
	class RendererSystem;
	class ParameterGroup;
	class ParticlesComponent;
	class RendererDebugGroup;
	class RendererSettingsGroup;
}


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
	*    Particle manager class
	*
	*  @note
	*    - The external library "Particle Universe" (PU) ( http://www.fxpression.com/ ) is used
	*/
	class QSF_API_EXPORT ParticleManager : public Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererSystem;
		friend class ParticlesComponent;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline FileCache& getParticleFileCache() const;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<ParticlesComponent*> ParticlesComponents;

		enum JobManagerIndex
		{
			REALTIME_GUI,
			ANIMATION_PARTICLES,
			NUMBER_OF_JOBS
		};


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static void updateParticleUniverseParticleSystem(ParticlesComponents::iterator start, ParticlesComponents::iterator end, const JobArguments* jobArguments);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rendererSystem
		*    The owner renderer system, must stay valid as long as this particle manager exists
		*/
		explicit ParticleManager(RendererSystem& rendererSystem);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ParticleManager();

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the particle manager
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool onStartup();

		/**
		*  @brief
		*    Return whether or not the particle manager is up-and-running
		*
		*  @return
		*    "true" if the particle manager is up-and-running ("onStartup()" was called successfully), else "false"
		*/
		inline bool isRunning() const;

		/**
		*  @brief
		*    Shutdown the particle manager
		*
		*  @note
		*    - This method is only called case "onStartup()" returned successfully
		*/
		void onShutdown();

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		void onAssetsMounted(const Assets& assets);
		void onAssetsUnmounted(const Assets& assets);
		void onSettingsPropertyChanged(const ParameterGroup& parameterGroup, uint32 property);
		void prefetchParticleUniverseParticleTechniques();

		//[-------------------------------------------------------]
		//[ Update                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Register particles component to manager update
		*/
		void registerToManagerUpdate(ParticlesComponent& particlesComponent);

		/**
		*  @brief
		*    Unregister particles component from manager update
		*/
		void unregisterFromManagerUpdate(ParticlesComponent& particlesComponent);

		/**
		*  @brief
		*    Main update function that is passed to the job manager
		*
		*  @param[in] jobArguments
		*    Job arguments
		*  @param[in] jobManagerIndex
		*    Job manager index
		*/
		void updateJob(const JobArguments& jobArguments, JobManagerIndex jobManagerIndex);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AssetSystem&		   mAssetSystem;		///< Asset system instance, do not destroy the instance
		RendererSystem&		   mRendererSystem;		///< The owner renderer system, do not destroy the instance
		bool				   mRunning;			///< "true" if the particle manager is up-and-running ("onStartup()" was called successfully), else "false"
		RendererSettingsGroup* mRendererSettingsGroup;
		RendererDebugGroup*	   mRendererDebugGroup;
		#if !defined(_USRDLL) || defined(__APPLE__)
			ParticleUniverse::ParticleUniversePlugin* mParticleUniversePlugin;	///< OGRE Particle Universe plugin instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		#endif
		FileCache&			   mParticleFileCache;										///< Cache for PU files
		JobProxy			   mUpdateJobProxy[NUMBER_OF_JOBS];							///< Job proxy
		ParticlesComponents	   mSequentialParticlesComponentsUpdate[NUMBER_OF_JOBS];	///< Particles components to update sequential
		ParticlesComponents	   mParallelParticlesComponentsUpdate[NUMBER_OF_JOBS];		///< Particles components to update in parallel
		ParticlesComponents	   mParticlesComponentWorkingSet;							///< To reduce memory allocations/deallocations: "qsf::ParticlesComponent::updateParticleUniverseParticleSystem()" can unregister particles components from updating, so we need to work on a local copy while updating


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/particles/ParticleManager-inl.h"
