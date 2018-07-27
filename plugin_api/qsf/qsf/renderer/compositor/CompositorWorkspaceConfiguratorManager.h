// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/plugin/pluginable/PluginableManager.h"

#include <OGRE/OgrePrerequisites.h>	// TODO(co) OGRE 2.0, "OGRE/OgreIdString.h" lacks of this include
#include <OGRE/OgreIdString.h>
#undef TRANSPARENT	// OGRE and it's Windows OS includes...

#include <boost/noncopyable.hpp>

#include <map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class RenderTarget;
	class CompositorWorkspace;
}
namespace qsf
{
	class GuiContext;
	class CameraComponent;
	namespace detail
	{
		class OgreCompositorWorkspaceListener;
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
	*    Compositor workspace configurator pluginable manager
	*
	*  @note
	*    - A compositor workspace configurator will compose the OGRE workspace definitions during runtime according to the current requirements
	*/
	class QSF_API_EXPORT CompositorWorkspaceConfiguratorManager : public PluginableManager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererSystem;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct RenderTarget
		{


			//[-------------------------------------------------------]
			//[ Public data                                           ]
			//[-------------------------------------------------------]
			// Given from the outside
			Ogre::RenderTarget& ogreFinalRenderTarget;
			CameraComponent*	cameraComponent;
			GuiContext*			guiContext;
			// Internal
			Ogre::IdString			   workspaceDefinitionName;	// TODO(co) Sadly, OGRE 2.0 gives us no chance to access "Ogre::Ogre::CompositorWorkspace::mDefinition" (we might want to change this)
			Ogre::CompositorWorkspace* ogreCompositorWorkspace;
			bool					   mGuiRenderingOnly;


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			RenderTarget(Ogre::RenderTarget& ogreFinalRenderTargetInput, CameraComponent* cameraComponentInput, GuiContext* guiContextInput) :
				ogreFinalRenderTarget(ogreFinalRenderTargetInput),
				cameraComponent(cameraComponentInput),
				guiContext(guiContextInput),
				ogreCompositorWorkspace(nullptr),
				mGuiRenderingOnly(false)
			{
				// Nothing here
			}


		};

		typedef std::map<Ogre::RenderTarget*, RenderTarget> RenderTargets;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CompositorWorkspaceConfiguratorManager();

		// TODO(co) Comment
		bool addCompositorWorkspace(Ogre::RenderTarget& ogreFinalRenderTarget, CameraComponent* cameraComponent = nullptr, GuiContext* guiContext = nullptr);
		bool updateCompositorWorkspace(Ogre::RenderTarget& ogreFinalRenderTarget, CameraComponent* cameraComponent = nullptr, GuiContext* guiContext = nullptr);
		bool renderCompositorWorkspace(Ogre::RenderTarget& ogreFinalRenderTarget);
		void renderCompositorWorkspace(Ogre::CompositorWorkspace& ogreCompositorWorkspace);
		void swapFinalCompositorWorkspace(Ogre::RenderTarget& ogreFinalRenderTarget);
		void swapFinalCompositorWorkspace(Ogre::CompositorWorkspace& ogreCompositorWorkspace);
		void removeCompositorWorkspace(Ogre::RenderTarget& ogreFinalRenderTarget);
		Ogre::CompositorWorkspace* getCompositorWorkspace(Ogre::RenderTarget& ogreFinalRenderTarget) const;
		void recreateCompositorWorkspace(Ogre::RenderTarget& ogreRenderTarget);
		void recreateCompositorWorkspace(const Ogre::IdString compositorWorkspaceDefinitionName);
		const RenderTarget* getRenderTargetByCompositorWorkspace(Ogre::CompositorWorkspace& ogreCompositorWorkspace) const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::PluginableManager methods      ]
	//[-------------------------------------------------------]
	protected:
		virtual void classAdded(const camp::Class& added) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		CompositorWorkspaceConfiguratorManager();

		// TODO(co) Comment
		bool updateCompositorWorkspace(RenderTarget& renderTarget, CameraComponent* cameraComponent, GuiContext* guiContext) const;
		void setOgreCompositorWorkspaceGuiContext(Ogre::CompositorWorkspace& ogreCompositorWorkspace, GuiContext* guiContext) const;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		void onStartup();
		void onShutdown();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		detail::OgreCompositorWorkspaceListener* mOgreCompositorWorkspaceListener;	///< OGRE compositor workspace listener, always valid, we're responsible for destroying the instance
		RenderTargets							 mRenderTargets;
		bool									 mRunning;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::CompositorWorkspaceConfiguratorManager)
