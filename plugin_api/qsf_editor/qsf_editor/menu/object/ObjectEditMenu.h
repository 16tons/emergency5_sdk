// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtWidgets/qmenu.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QAction;
QT_END_NAMESPACE


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
		*    Object edit menu
		*
		*  @note
		*    - This menu is more complex and dynamic, so it's justified to have a special menu class for this task
		*/
		class ObjectEditMenu : public QMenu
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			ObjectEditMenu();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ObjectEditMenu();

			/**
			*  @brief
			*    Return the report bug Qt action
			*
			*  @return
			*    Report bug Qt action, do not destroy the instance
			*/
			QAction& getReportBugQAction() const;

			/**
			*  @brief
			*    Return the add comment Qt action
			*
			*  @return
			*    Add comment Qt action, do not destroy the instance
			*/
			QAction& getAddCommentQAction() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			QAction* mQActionReportBug;		///< Report bug Qt action, always valid, Qt will destroy the instances automatically
			QAction* mQActionAddComment;	///< Add comment Qt action, always valid, Qt will destroy the instances automatically


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
