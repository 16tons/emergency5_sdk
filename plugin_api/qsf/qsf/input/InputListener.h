// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Window;
	class InputEvent;
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
	*    Input listener class
	*
	*  @note
	*    - The default implementation of "eventReceived()" is empty
	*/
	class QSF_API_EXPORT InputListener
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct Response
		{
			bool mBlockOtherListeners;	///< Set to "true" to block other input listeners
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline InputListener();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~InputListener();

		/**
		*  @brief
		*    Return the focus window
		*/
		inline Window* getFocusWindow() const;

		/**
		*  @brief
		*    Set focus window
		*
		*  @param[in] focusWindow
		*    Only if this window has currently the focus, this input listener will receive events
		*/
		inline void setFocusWindow(Window* focusWindow);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputListener methods             ]
	//[-------------------------------------------------------]
	public:
		inline virtual void eventReceived(const InputEvent& inputEvent, Response& response);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Window* mFocusWindow;	///< Focus window instance, can be a null pointer; only if this window has currently the focus, this input listener will receive events


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/input/InputListener-inl.h"
