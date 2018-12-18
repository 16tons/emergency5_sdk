// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"
#include "qsf/asset/AssetSystemTypes.h"

#include <glm/glm.hpp>

#include <boost/container/flat_map.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace ParticleUniverse
{
	class ParticleSystem;
	class ParticleSystemManager;
}
namespace Ogre
{
	class SceneManager;
}
namespace qsf
{
	class AssetProxy;
	class AssetSystem;
	class RendererSystem;
	class ParameterGroup;
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
	*    Particle Universe particle manager instance manager class
	*
	*  @remarks
	*    Sadly, Particle Universe is not ready for massive particles usage. So we have to take care of a more clever particles manager instance
	*    management on our own. If we don't do such a management, we quickly get several GiB of memory consumption when having more than a handful
	*    of particle component instances, loading time will also explode due to the massive instance creation.
	*
	*  @note
	*    - The external library "Particle Universe" (PU) ( http://www.fxpression.com/ ) is used
	*/
	class QSF_API_EXPORT ParticleUniverseParticleSystemInstanceManager : public Manager
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		class Listener
		{


		//[-------------------------------------------------------]
		//[ Public virtual qsf::ParticleUniverseParticleSystemInstanceManager::Listener methods ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Called just before "qsf::ParticleUniverseParticleSystemInstanceManager::rentParticleUniverseParticleSystem()" will return
			*/
			virtual void onRentParticleUniverseParticleSystem(const AssetProxy& particlesAssetProxy, ParticleUniverse::ParticleSystem& particleUniverseParticleSystem) = 0;

			/**
			*  @brief
			*    Called directly as first inside "qsf::ParticleUniverseParticleSystemInstanceManager::returnRentedParticleUniverseParticleSystem()"
			*/
			virtual void onReturnRentedParticleUniverseParticleSystem(ParticleUniverse::ParticleSystem& particleUniverseParticleSystem) = 0;


		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Load a Particle Universe script
		*
		*  @param[in] globalAssetIdAsString
		*    The script asset's global asset ID in string form
		*  @param[in] rendererSystem
		*    The "qsf::RendererSystem" instance to use
		*  @param[in] logError
		*    If "true", log errors
		*/
		static bool loadParticleUniverseFile(const std::string& globalAssetIdAsString, RendererSystem& rendererSystem, bool logError);

		/**
		*  @brief
		*    Load or reload a Particle Universe script and update particle system instances if required
		*
		*  @param[in] globalAssetId
		*    The script asset's global asset ID
		*  @param[in] rendererSystem
		*    The "qsf::RendererSystem" instance to use
		*  @param[in] logError
		*    If "true", log errors
		*/
		static bool loadOrReloadParticleUniverseFile(GlobalAssetId globalAssetId, RendererSystem& rendererSystem, bool logError);

		/**
		*  @brief
		*    Apply particles LOD setting to the given Particle Universe template
		*
		*  @param[in] templateName
		*    Name of the Particle Universe template to apply the particles LOD setting to
		*/
		static void applyLodSetting(const std::string& templateName);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] ogreSceneManager
		*    Owner OGRE scene manager
		*/
		explicit ParticleUniverseParticleSystemInstanceManager(Ogre::SceneManager& ogreSceneManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ParticleUniverseParticleSystemInstanceManager();

		/**
		*  @brief
		*    Set Particle Universe particle system instance manager; listener instance will not be destroyed internally
		*/
		void setListener(Listener* listener);

		/**
		*  @brief
		*    Check whether or not a Particle Universe particle system is considered to be looping
		*
		*  @param[in] particlesAssetProxy
		*    The particles asset to check (e.g. "sample/particles/fire/campfire")
		*
		*  @return
		*    "true" if the Particle Universe particle system is considered to be looping, else "false"
		*/
		bool isLoopingParticleUniverseParticleSystem(const AssetProxy& particlesAssetProxy);

		/**
		*  @brief
		*    Create Particle Universe particle system instances to reduce the risk of renting causing runtime hiccups
		*/
		void heatRentableStock();

		/**
		*  @brief
		*    Rent a Particle Universe particle system instance from the particle manager
		*
		*  @param[in] particlesAssetProxy
		*    The particles asset to use (e.g. "sample/particles/fire/campfire")
		*  @param[out] looping
		*    Receives information about whether or not the particles effect is considered to be looping, do only use this information in case you received a valid rented Particle Universe particle system instance
		*  @param[in] enforceCreation
		*    Enforce creation of particles system instance in case there's nothing left in the free-pool? During runtime it's usually no good idea to enforce it due to runtime hiccups.
		*
		*  @return
		*    Rented Particle Universe particle system instance, null pointer on error, be polite and return the borrowed instance instead of destroying it
		*/
		ParticleUniverse::ParticleSystem* rentParticleUniverseParticleSystem(const AssetProxy& particlesAssetProxy, bool& looping, bool enforceCreation = false);

		/**
		*  @brief
		*    Return a rented Particle Universe particle system instance to the particle manager
		*
		*  @param[in] particleUniverseParticleSystem
		*    Rented Particle Universe particle system instance you wish to return in order to get your bail back (just kidding, there's no bail back)
		*/
		void returnRentedParticleUniverseParticleSystem(ParticleUniverse::ParticleSystem& particleUniverseParticleSystem);

		/**
		*  @brief
		*    Destroy unused Particle Universe particle system instances using a given global asset ID
		*
		*  @note
		*    - Don't use this method if you don't have to
		*/
		void destroyUnusedParticleUniverseParticleSystemsByGlobalAssetId(GlobalAssetId globalAssetId);

		/**
		*  @brief
		*    Destroy all unused Particle Universe particle system instances
		*
		*  @note
		*    - Don't use this method if you don't have to
		*/
		void destroyAllUnusedParticleUniverseParticleSystems();

		/**
		*  @brief
		*    Return whether or not particle system reusage is enabled
		*
		*  @return
		*    "true" if particle system reusage is enabled, else "false"
		*
		*  @note
		*    - Due to major memory constraints on x86, we only support particle reusage on x64
		*/
		bool isParticleSystemReusageRenabled() const;

		/**
		*  @brief
		*    Set whether or not particle system reusage is enabled
		*
		*  @param[in] enabled
		*    "true" if particle system reusage is enabled, else "false"
		*
		*  @see
		*    - "isParticleSystemReusageRenabled()"
		*/
		void setParticleSystemReusageRenabled(bool enabled);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onSettingsPropertyChanged(const ParameterGroup& parameterGroup, uint32 property);


	//[-------------------------------------------------------]
	//[ Private declarations                                  ]
	//[-------------------------------------------------------]
	private:
		struct ParticleUniverseParticleSystemBackup
		{
			ParticleUniverse::ParticleSystem* particleUniverseParticleSystem;	///< Particle Universe particle system instance, can be a null pointer, do not destroy the instance
			float							  originalScaleVelocity;			///< Original scale velocity from the Particle Universe particle system instance
			glm::vec3						  originalScale;					///< Original scale from the Particle Universe particle system instance
			bool							  looping;							///< "true" if the particles effect is looping, else "false"
		};
		typedef std::vector<ParticleUniverse::ParticleSystem*>									ParticleUniverseParticleSystems;
		typedef boost::container::flat_map<GlobalAssetId, ParticleUniverseParticleSystems>		UnusedParticleUniverseParticleSystems;
		typedef boost::container::flat_map<GlobalAssetId, ParticleUniverseParticleSystemBackup> ParticleUniverseParticleSystemBackups;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AssetSystem&							 mAssetSystem;
		RendererSystem&							 mRendererSystem;
		Ogre::SceneManager&						 mOgreSceneManager;							///< Owner OGRE scene manager
		ParticleUniverse::ParticleSystemManager& mParticleUniverseParticleSystemManager;
		UnusedParticleUniverseParticleSystems	 mUnusedParticleUniverseParticleSystems;
		ParticleUniverseParticleSystemBackups	 mParticleUniverseParticleSystemBackups;
		bool									 mParticleSystemReusageRenabled;			///< "true" if particle system reusage is enabled, else "false"
		RendererSettingsGroup&					 mRendererSettingsGroup;
		Listener*								 mListener;	///< Listener to use, always valid, instance is only referenced and not destroyed after it's no longer used
		unsigned long							 mNextFrameNumber;
		uint32									 mNumberOfRentedParticlesSystemInLastFrame;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
