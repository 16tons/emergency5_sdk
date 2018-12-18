// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/base/manager/Manager.h>

#include <QtCore/qpoint.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Cursor manager class
		*/
		class QSF_EDITOR_API_EXPORT CursorManager : public Manager
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			CursorManager();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~CursorManager();

			/**
			*  @brief
			*    Return whether or not the cursor is currently visible
			*
			*  @return
			*    "true" if the cursor is currently visible, else "false"
			*/
			bool isCursorVisible() const;

			/**
			*  @brief
			*    Set the cursor visibility
			*
			*  @param[in] cursorVisible
			*    "true" if the cursor should be visible, else "false"
			*
			*  @note
			*    - Also backups/restores the cursor position
			*/
			void setCursorVisible(bool cursorVisible);

			/**
			*  @brief
			*    Return the cursor position at the point in time where it was last visible
			*
			*  @return
			*    Cursor position at the point in time where it was last visible
			*/
			QPoint getPreviousVisibleCursorPosition() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool   mCursorVisible;					///< Cursor visible?
			QPoint mPreviousVisibleCursorPosition;	///< Cursor position at the point in time where it was last visible


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
