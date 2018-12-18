// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Rocket
{
	namespace Core
	{
		class Context;
		class Plugin;
		class EventListener;
	}
}
namespace qsf
{
	class GuiContext;
	class RocketFileInterface;
	class RocketSystemInterface;
	class OgreRocketRenderInterface;
	class FileCache;
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
	*    Ingame Graphical User Interface (GUI) system class
	*
	*  @note
	*    - The external library "libRocket" ( http://librocket.com/ ) is used
	*    - For ingame GUI only, does not deal with operation system GUI topics (e.g. within the editor we're using Qt for this task)
	*    - Depends on the render system
	*/
	class QSF_API_EXPORT GuiSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline GuiSystem();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~GuiSystem();

		/**
		*  @brief
		*    Create a GUI context instance
		*
		*  @return
		*    The GUI context instance, null pointer on error (e.g. GUI system is not up-and-running due to server mode), destroy the instance in case you no longer need it
		*/
		GuiContext* createGuiContext() const;

		/**
		*  @brief
		*    Return the GUI context used by the libRocket debugger
		*
		*  @return
		*    The GUI context used by the libRocket debugger, null pointer case the libRocket debugger is disabled
		*
		*  @note
		*    - Don't use the libRocket debugger functions directly in order to avoid inconsistent states
		*/
		inline GuiContext* getDebuggerGuiContext() const;

		/**
		*  @brief
		*    Set the GUI context used by the libRocket debugger
		*
		*  @param[in] guiContext
		*    The GUI context used by the libRocket debugger, null pointer to disable the libRocket debugger, must stay valid as long as it's set as the current debugger GUI context
		*
		*  @see
		*    - "qsf::GuiSystem::getDebuggerGuiContext()"
		*/
		void setDebuggerGuiContext(GuiContext* guiContext);

		/**
		*  @brief
		*    Register a libRocket plugin
		*
		*  @param[in] plugin
		*    The plugin to register, this GUI system takes over the memory ownership
		*
		*  @note
		*    - Plugins can not be unregistered manually, libRocket will handle the cleanup when "Rocket::Core::Shutdown()" is called
		*/
		void registerLibrocketPlugin(Rocket::Core::Plugin& plugin);

		/**
		*  @brief
		*    Create an event listener
		*/
		Rocket::Core::EventListener* createEventListener(const std::string& eventKey);

		/**
		*  @brief
		*    Return the used character set
		*/
		const std::string& getCharacterSet() const;

		/**
		*  @brief
		*    Set the character set which shall use
		*
		*  @param[in] characterSet
		*    Character set which shall use as string for example "U+0020-00FF".
		*    You can set more character sets by separating them with comma, for example "U+0020-00FF,U+0400-04FF"
		*/
		void setCharacterSet(const std::string& characterSet);

		/**
		*  @brief
		*    Reset the "pixels per inch" conversion value, used for units like "pt", "in", "cm", back to its default value
		*/
		void resetPixelsPerInch();

		/**
		*  @brief
		*    Set the "pixels per inch" conversion value, used for units like "pt", "in", "cm"
		*
		*  @param[in] ppi
		*    Pixels per inch, default is 100.0f
		*/
		void setPixelsPerInch(float ppi);

		/**
		*  @brief
		*    Get the GUI file cache instance
		*/
		FileCache* getGuiFileCache() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup(bool serverMode) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool								mRunning;					///< "true" if the GUI system is up-and-running, else "false"
		RocketSystemInterface*				mRocketSystemInterface;		///< libRocket system interface instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		RocketFileInterface*				mRocketFileInterface;		///< libRocket file interface instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		OgreRocketRenderInterface*			mOgreRocketRenderInterface;	///< libRocket OGRE render interface instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		GuiContext*							mDebuggerGuiContext;		///< The GUI context used by the libRocket debugger, null pointer case the libRocket debugger is disabled, don't destroy the instance
		Rocket::Core::Context*				mRocketCoreContextDebugger;	///< libRocket debugger context, can be a null pointer, don't forget to remove the reference in case we no longer need it
		std::vector<Rocket::Core::Plugin*>	mRocketPlugin;				///< List of libRocket plugins so we can support multiple different plugins, we're responsible for destroying the instance and deleting the content in case we no longer need it
		std::string							mCharacterSet;				///< Holds the used character set for example. U+0020-00FF; if it is empty we use the character set used in the rml or rcss.


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/gui/GuiSystem-inl.h"
