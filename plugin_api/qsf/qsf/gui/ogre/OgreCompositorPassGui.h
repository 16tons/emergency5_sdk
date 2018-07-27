// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <OGRE/Compositor/Pass/OgreCompositorPass.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class GuiContext;
	class RendererSettingsGroup;
	class OgreCompositorPassDefGui;
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
	*    GUI OGRE compositor pass
	*/
	class OgreCompositorPassGui : public Ogre::CompositorPass
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		OgreCompositorPassGui(const OgreCompositorPassDefGui* definition, Ogre::CompositorNode* parentNode, const Ogre::CompositorChannel& target, Ogre::SceneManager* sceneManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OgreCompositorPassGui();

		/**
		*  @brief
		*    Return the used GUI context instance
		*
		*  @return
		*    The used GUI context instance, can be a null pointer, do no destroy the returned instance
		*/
		GuiContext* getGuiContext() const;

		/**
		*  @brief
		*    Set the used GUI context instance
		*
		*  @param[in] guiContext
		*    The used GUI context instance, null pointer to set no GUI context
		*/
		void setGuiContext(GuiContext* guiContext);


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::CompositorPass methods           ]
	//[-------------------------------------------------------]
	public:
		virtual void execute(const Ogre::Camera* lodCamera) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const OgreCompositorPassDefGui*	mDefinition;
		Ogre::SceneManager*				mSceneManager;
		GuiContext*						mGuiContext;			///< GUI context, can be a null pointer, don't destroy the instance
		RendererSettingsGroup&			mRendererSettingsGroup;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
