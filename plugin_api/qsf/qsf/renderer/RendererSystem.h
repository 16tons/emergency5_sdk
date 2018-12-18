// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/job/JobProxy.h"
#include "qsf/base/System.h"

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Root;
	class LogSystem;
	class LogManager;
	class GL3PlusPlugin;
	class RenderTarget;
	class D3D11Plugin;
	class GpuCommandBufferFlush;
	class ScriptCompilerListener;
	namespace v1
	{
		class ManualObjectFactory;
	}
}
namespace qsf
{
	class VrManager;
	class RenderWindow;
	class MaterialSystem;
	class ParameterGroup;
	class ParticleManager;
	class OgreResourceGroup;
	class QsfOgreEntityFactory;
	class RendererSettingsGroup;
	class TextureStreamingManager;
	class OgreResourceManagerHelper;
	class SceneManagerBaseFactory;
	class GridSceneManagerFactory;
	class MeshVertexFormatConverter;
	class OgreCompositorPassProvider;
	class CompositorWorkspaceConfiguratorManager;
	class SkeletonManager;
	namespace detail
	{
		class OgreLogRedirect;
		class OgreScriptCompilerListener;
	}
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
	*    Renderer system class
	*
	*  @note
	*    - The external library "Object-Oriented Graphics Rendering Engine" (OGRE) ( http://www.ogre3d.org/ ) is used
	*    - Warning: Do not access OGRE features unless you have to or you will create a lot of additional dependencies
	*      which might force you to add OGRE to your precompiled header and therefore enlarging it by around 200 MB. This
	*      creates an domino-effect and slows down the build process. You don't want that. Stay clean, stay fast.
	*    - The OGRE resource group "QSF_Renderer" is used
	*/
	class QSF_API_EXPORT RendererSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		RendererSystem();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RendererSystem();

		/**
		*  @brief
		*    Return whether or not the renderer system is up-and-running
		*
		*  @return
		*    "true" if the renderer system is up-and-running ("onStartup()" was called successfully), else "false"
		*/
		inline bool isRunning() const;

		/**
		*  @brief
		*    Ask the renderer system to flush as much memory as possible
		*
		*  @note
		*    - Don't call this frequently (performance)
		*/
		void flushMemory();

		/**
		*  @brief
		*    Check whether or not the given resolution is supported by the renderer
		*
		*  @param[in] width
		*    Width of the resolution
		*  @param[in] height
		*    Height of the resolution
		*
		*  @return
		*    "true" if the given resolution is supported, else "false"
		*/
		bool isResolutionSupported(uint32 width, uint32 height) const;

		/**
		*  @brief
		*    Return a list of supported resolutions
		*
		*  @param[out] supportedResolutions
		*    Receives the supported resolutions, list is not cleared before new entries are added, it's guaranteed that there are no duplicates and the list is sorted
		*/
		void getSupportedResolutions(std::vector<glm::ivec2>& supportedResolutions) const;

		/**
		*  @brief
		*    Return the size of the dedicated video memory
		*
		*  @return
		*    The size of the dedicated video memory (in bytes)
		*
		*  @note
		*    - Example: GeForce GTX 570 with 1280 MiB GDDR5 video RAM
		*/
		uint32 getDedicatedVideoMemorySize() const;

		/**
		*  @brief
		*    Return the friendly name of the renderer which can e.g. directly be used inside filenames
		*
		*  @return
		*    The friendly name of the renderer (examples: "opengl", "direct3d_11")
		*/
		std::string getFriendlyName() const;

		/**
		*  @brief
		*    Return whether or not the renderer has depth texture support
		*
		*  @return
		*    "true" if the renderer has depth texture support, else "false" (like the case on Mac OS X)
		*/
		bool hasDepthTextureSupport() const;

		/**
		*  @brief
		*    Explicitly disable renderer system asynchronous processing
		*
		*  @remarks
		*    Several aspects of the renderer system are using asynchronous processing in order to reduce hiccups during runtime:
		*    - Texture streaming
		*    - Mesh streaming
		*    - Shader compilation
		*
		*    There are situations like screenshot capturing or ground map generation were one needs everything at once. In
		*    those use-cases, you have to explicitly disable the asynchronous processing and reenable it when you're done.
		*
		*  @note
		*    - Call "qsf::RendererSystem::resetAsynchronousProcessing()" when you're done
		*/
		void disableAsynchronousProcessing();

		/**
		*  @brief
		*    Reset asynchronous renderer system processing to the state before it had been explicitly disabled
		*
		*  @see
		*    - "qsf::RendererSystem::disableAsynchronousProcessing()"
		*/
		void resetAsynchronousProcessing();

		//[-------------------------------------------------------]
		//[ System                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the material system instance
		*
		*  @return
		*    The material system instance, do no destroy the returned instance
		*/
		inline MaterialSystem& getMaterialSystem() const;

		//[-------------------------------------------------------]
		//[ Manager                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the texture streaming manager instance
		*
		*  @return
		*    The texture streaming manager instance, do no destroy the returned instance
		*/
		inline TextureStreamingManager& getTextureStreamingManager() const;

		/**
		*  @brief
		*    Return the particle manager instance
		*
		*  @return
		*    The particle manager instance, do no destroy the returned instance
		*/
		inline ParticleManager& getParticleManager() const;

		inline CompositorWorkspaceConfiguratorManager& getCompositorWorkspaceConfiguratorManager() const;
		VrManager& getVrManager();

		/**
		*  @brief
		*    Return the skeleton manager instance
		*
		*  @return
		*    The skeleton manager instance, do no destroy the returned instance
		*/
		inline SkeletonManager& getSkeletonManager() const;

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the ogre resource manager helper instance
		*
		*  @return
		*    The ogre resource manager helper instance, do no destroy the returned instance
		*/
		inline OgreResourceManagerHelper& getResourceManager() const;

		/**
		*  @brief
		*    Return the OGRE root instance
		*
		*  @return
		*    The OGRE root instance, can be a null pointer, do no destroy the returned instance
		*/
		inline Ogre::Root* getOgreRoot() const;

		/**
		*  @brief
		*    Return the mesh vertex format converter instance
		*
		*  @return
		*    The mesh vertex format converter instance, can be a null pointer, do no destroy the returned instance
		*
		*  @note
		*    - Don't access this if you don't have to
		*/
		inline MeshVertexFormatConverter* getMeshVertexFormatConverter() const;

		/**
		*  @brief
		*    Set whether or not the OGRE script compiler is muzzled
		*
		*  @param[in] muzzle
		*    "true" to muzzle the OGRE script compiler, else "false"
		*
		*  @remarks
		*    TODO(co) In OGRE 1.8
		*      "mCompiler->addError(CE_UNEXPECTEDTOKEN, impl->file, impl->line, "token class, " + impl->cls + ", unrecognized.");"
		*    was added inside the "Ogre::ScriptCompiler::AbstractTreeBuilder::visit()"-method inside "ScriptCompilerManager.cpp" resulting in tons of errors like
		*      "
		*      Compiler error: unknown error in (177): token class, renderer, unrecognized.
		*      Compiler error: unknown error in (186): token class, emission_rate, unrecognized.
		*      Compiler error: unknown error in (191): token class, angle, unrecognized.
		*      Compiler error: unknown error in (196): token class, time_to_live, unrecognized.
		*      "
		*   The command to ID mapping is initialized inside "Ogre::ScriptCompiler::initWordMap()" and can apparently not be extended.
		*   So we're always getting Particle Universe script errors by design. Other people noticed this issue as well
		*     http://ogre3d.org/forums/viewtopic.php?f=2&t=68698
		*     http://www.ogre3d.org/forums/viewtopic.php?f=2&t=68319&p=447769&hilit=+script+unrecognized+token#p447769
		*     http://www.ogre3d.org/forums/viewtopic.php?f=2&t=77014
		*   The only solution I found which does not involve manipulating the OGRE source codes is to add a script compiler listener.
		*/
		void setOgreScriptCompilerMuzzle(bool muzzle);

		/**
		*  @brief
		*    Return the internal dummy render window instance
		*
		*  @return
		*    The internal dummy render window instance, can be a null pointer, don't destroy the instance
		*
		*  @note
		*    - Don't use this method if you don't have to
		*/
		inline RenderWindow* getRenderWindowDummy() const;

		/**
		*  @brief
		*    Lock/unlock the renting of the render window dummy
		*
		*  @param[in] lock
		*    "true" to lock the renting of the render window dummy, else "false"
		*
		*  @note
		*    - If the render window dummy is locked, "qsf::RendererSystem::rentRenderWindowDummy()" will return a new instance
		*    - Locking is e.g. required when starting the game from inside the editor, if the user is closing the window inside the game by using
		*      the operation system close-button this window will automatically be destroyed internally, which isn't good if the renderer resources
		*      depend on this window
		*/
		inline void setRenderWindowDummyLock(bool lock);

		/**
		*  @brief
		*    Rent the internal dummy render window instance
		*
		*  @return
		*    The internal dummy render window instance, can be a null pointer, don't destroy the instance, be polite and return the rented instance by using "qsf::RendererSystem::returnRentedRenderWindowDummy()"
		*
		*  @note
		*    - The render window is hidden by default, show it when you dare
		*/
		RenderWindow* rentRenderWindowDummy();

		/**
		*  @brief
		*    Return the rented internal dummy render window instance
		*
		*  @param[in] renderWindow
		*    Render window to return
		*
		*  @see
		*    - "qsf::RendererSystem::rentRenderWindowDummy()"
		*/
		void returnRentedRenderWindowDummy(RenderWindow& renderWindow);

		/**
		*  @brief
		*    Render a frame using the given render window
		*
		*  @param[in] renderWindow
		*    Render window
		*/
		void renderFrame(const RenderWindow& renderWindow, Ogre::RenderTarget& ogreRenderTarget, bool isSecondEye);

		/**
		*  @brief
		*    Main update function that is passed to the job manager
		*
		*  @param[in] jobArguments
		*    Job arguments
		*
		*  @note
		*    - You may not want to call this method in case you're using e.g. Qt for your OS GUI
		*/
		void updateJob(const JobArguments& jobArguments);

		/**
		*  @brief
		*    Enforce an update
		*
		*  @note
		*    - Don't use this method if you don't have to
		*/
		void enforceUpdate();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the renderer system
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @remarks
		*    The implementation does the following tasks:
		*    - Initialise OGRE logging
		*    - Create the OGRE root instance
		*    - Load OGRE render system plugins
		*    - Set OGRE render system
		*    - Initialize OGRE
		*/
		virtual bool onStartup(bool serverMode) override;

		/**
		*  @brief
		*    Shutdown the renderer system
		*
		*  @remarks
		*    The implementation does the following tasks:
		*    - Destroy the OGRE root instance
		*    - Shutdown OGRE logging
		*
		*  @note
		*    - This method is only called case "onStartup()" returned successfully
		*/
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Creates a debug text output with resource manager usage statistics
		*/
		void displayResourceManagerUsageDebugOverlay();

		/**
		*  @brief
		*    Set OGRE resource manager budget
		*/
		void setOgreMemoryBudget(size_t numberOfBytes);

		/**
		*  @brief
		*    Update GPU command buffer flush configuration
		*/
		void updateGpuCommandBufferFlushConfiguration();

		void onSettingsPropertyChanged(const ParameterGroup& parameterGroup, uint32 property);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RendererSettingsGroup*				mRendererSettingsGroup;
		bool								mRunning;								///< "true" if the renderer system is up-and-running, else "false"
		Ogre::LogManager*					mOgreLogManager;						///< OGRE log manager, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		detail::OgreLogRedirect*			mOgreLogRedirect;						///< OGRE log direct instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		detail::OgreScriptCompilerListener*	mOgreScriptCompilerListener;			///< OGRE script compiler listener instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		MeshVertexFormatConverter*			mMeshVertexFormatConverter;				///< OGRE mesh vertex format converted instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		Ogre::ScriptCompilerListener*		mPreviousOgreScriptCompilerListener;	///< Previous OGRE script compiler listener instance, can be a null pointer, do not destroy the instance
		Ogre::Root*							mOgreRoot;								///< OGRE root instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		OgreResourceGroup*					mOgreResourceGroup;						///< QSF OGRE resource group, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		#if !defined(_USRDLL) || defined(__APPLE__)
			Ogre::GL3PlusPlugin*			mOgreGL3PlusPlugin;						///< OGRE OpenGL 3 plugin instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		#endif
		#ifdef WIN32
			#ifndef _USRDLL
				Ogre::D3D11Plugin*			mOgreD3D11Plugin;						///< OGRE Direct3D 11 plugin instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
			#endif
		#endif
		Ogre::v1::ManualObjectFactory*		mManualObjectFactory;
		OgreCompositorPassProvider*			mOgreCompositorPassProvider;			///< OGRE custom compositor pass provider, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		QsfOgreEntityFactory*				mQsfOgreEntityFactory;
		RenderWindow*						mRenderWindowDummy;						///< Dummy render window instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		bool								mRenderWindowDummyLock;
		bool								mRenderWindowDummyRented;				///< "true" if the render window dummy instance has been rented, else "false"

		// System
		MaterialSystem*							mMaterialSystem;
		// Manager
		TextureStreamingManager*				mTextureStreamingManager;
		ParticleManager*						mParticleManager;					///< Particle manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
		CompositorWorkspaceConfiguratorManager* mCompositorWorkspaceConfiguratorManager;
		VrManager*								mVrManager;
		SkeletonManager*						mSkeletonManager;

		JobProxy						mUpdateJobProxy;			///< Job proxy
		OgreResourceManagerHelper*		mOgreResourceManagerHelper;	///< OGRE resource manager helper instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		SceneManagerBaseFactory*		mSceneManagerBaseFactory;	///< QSF OGRE scene manager base factory instance, always valid, we're responsible for destroying the instance in case we no longer need it
		GridSceneManagerFactory*		mGridSceneManagerFactory;	///< QSF OGRE grid scene manager factory instance, always valid, we're responsible for destroying the instance in case we no longer need it
		Ogre::GpuCommandBufferFlush*	mGpuCommandBufferFlush;

		// For "qsf::RendererSystem::disableAsynchronousProcessing()" and "qsf::RendererSystem::resetAsynchronousProcessing()"
		bool mAsynchronousProcessingDisabled;
		// Texture streaming
		bool mIsFullTextureStreamInByDefaultEnabled;
		bool mIsChronologicCacheClearingEnabled;
		// Pipeline state compilation
		bool mAsynchronousPipelineStateCompilationEnabled;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/RendererSystem-inl.h"
