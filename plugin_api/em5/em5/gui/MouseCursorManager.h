// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class GuiContext;
}
namespace Rocket
{
	namespace Core
	{
		class ElementDocument;
	}
}


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
	*    Manager for handling mouse cursors via libRocket
	*/
	class EM5_API_EXPORT MouseCursorManager : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] guiContext
		*    The GUI context for which the cursors should be managed
		*/
		explicit MouseCursorManager(qsf::GuiContext& guiContext);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MouseCursorManager();

		/**
		*  @brief
		*    Doing initialization (loading the mouse cursor document)
		*/
		void startup();

		/**
		*  @brief
		*    Sets the cursor identified by the name as the new current mouse cursor
		*/
		void setMouseCursor(const std::string& cursorName);

		/**
		*  @brief
		*    Resets the mouse cursor to its default
		*/
		void resetMouseCursor();

		/**
		*  @brief
		*    Show hide the mouse cursor
		*
		*  @note
		*    - For ease-of-use: The method is using reference counting, meaning if you make the cursor two times invisible, you need to make it two times visible in order to see the mouse cursor
		*/
		void showCursor(bool show);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool loadMouseCursor(const std::string& cursorName);
		void setMouseCursorCssClass(const std::string& mouseCursorCssClassName);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::GuiContext&				mGuiContext;				///< QSF GUI context instance
		Rocket::Core::ElementDocument*	mCursorDocument;
		std::string						mCurrentMouseCursorName;	///< The name of the current mouse cursor
		int								mCursorShowCalls;			///< Number of times the mouse cursor was requested to be shown


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
