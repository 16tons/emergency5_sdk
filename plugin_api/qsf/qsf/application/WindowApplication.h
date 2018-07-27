// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/application/TimedApplication.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class RenderWindow;
	class ParameterGroup;
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
	*    Application implementation with a single window
	*/
	class QSF_API_EXPORT WindowApplication : public TimedApplication
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 APPLICATION_ID;	///< "qsf::WindowApplication" unique ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~WindowApplication();

		/**
		*  @brief
		*    Return the QSF render window instance
		*
		*  @return
		*    The QSF render window instance, can be a null pointer in case "onStartup()" failed or was not called yet, do no destroy the returned instance
		*/
		inline RenderWindow* getRenderWindow() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::WindowApplication methods      ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Get the window size and fullscreen flag
		*
		*  @note
		*    The default implementation returns the default window size and fullscreen mode
		*/
		virtual void getWindowSize(bool& fullscreen, uint32& width, uint32& height);

		/**
		*  @brief
		*    This method is called when the render window is about to be shown for the first time
		*
		*  @note
		*    - The default implementation is empty
		*/
		virtual void onRenderWindowFirstPreShow();

		/**
		*  @brief
		*    Return if the visibility of the system mouse cursor should be modified on fullscreen toggle
		*
		*  @return
		*    "true" if the modifying the system mouse cursor visibility on fullscreen toggle is allowed, otherwise false
		*/
		virtual bool getModifySystemCursorVisibilityOnFullscreenToggleAllowed() const;

		/**
		*  @brief
		*    Set a default window icon
		*
		*  @remarks
		*    The default implementation tries to find a default icon for the window in the following order:
		*    - First try: Try to use the default icon of the shared library this application instance is in
		*    - Second try: Use the default process icon... if there's one...
		*    - Third try: If there's no default process icon, we're using the standard QSF icon
		*    MS Windows: As default windows icon resource ID we have chosen "101".
		*/
		virtual void setDefaultWindowIcon();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Application methods               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Ask the application politely to shut down as soon as possible
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Close the render window
		*/
		virtual void exit() override;

		virtual uint32 getId() override;

		/**
		*  @brief
		*    Set the window settings to user user settings
		*/
		virtual void setWindowToUserSettings();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Application methods            ]
	//[-------------------------------------------------------]
	protected:
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the application
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Create the render window, it stays valid until it's destroyed within "qsf::WindowApplication::onShutdown()"
		*/
		virtual bool onStartup() override;

		/**
		*  @brief
		*    Run the application
		*
		*  @return
		*    Exit code of the application (usually 0 means no error)
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Loop until the render window gets closed, updates a realtime clock internally
		*
		*  @note
		*    - Only called in case "onStartup()" returned successfully
		*/
		virtual int onRun() override;

		/**
		*  @brief
		*    Shutdown the application
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Destroy the render window
		*
		*  @note
		*    - Only called in case "onStartup()" returned successfully
		*/
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		inline WindowApplication();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called as soon as there's a property change inside the settings group manager
		*
		*  @param[in] parameterGroup
		*    Parameter group instance the changed property is in
		*  @param[in] propertyId
		*    Unique class property ID the component property which values was changed
		*/
		void onSettingsGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);

		/**
		*  @brief
		*    Called as soon as there's a property change inside the debug group manager
		*
		*  @param[in] parameterGroup
		*    Parameter group instance the changed property is in
		*  @param[in] propertyId
		*    Unique class property ID the component property which values was changed
		*/
		void onDebugGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RenderWindow* mRenderWindow;	///< QSF render window instance, can be a null pointer in case "onStartup()" failed or was not called yet, we're responsible for destroying the instance in case we no longer need it


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/application/WindowApplication-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::WindowApplication)
