// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/qaction/QActions.h"


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
		*    Online QActions category
		*/
		class OnlineQActions : public QActions
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::OnlineQActions" unique pluginable QActions ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] qActionsManager
			*    The QtActions manager this QtActions instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit OnlineQActions(QActionsManager* qActionsManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~OnlineQActions();

			//[-------------------------------------------------------]
			//[ QAction                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the flush undo stack Qt action
			*
			*  @return
			*    The flush undo stack Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getFlushUndoStackQAction() const;

			/**
			*  @brief
			*    Return the server management Qt action
			*
			*  @return
			*    The server management Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getServerManagementQAction() const;

			/**
			*  @brief
			*    Return the select project map Qt action
			*
			*  @return
			*    The select project map Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getSelectProjectMapQAction() const;

			/**
			*  @brief
			*    Return the sign out Qt action
			*
			*  @return
			*    The sign out Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getSignOutQAction() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::QActions methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onTriggeredFlushUndoStack();
			void onTriggeredServerManagement();
			void onTriggeredSelectProjectMap();
			void onTriggeredSignOut();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			QAction* mFlushUndoStackQAction;	///< Flush undo stack Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mServerManagementQAction;	///< Server management Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mSelectProjectMapQAction;	///< Select project map Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mSignOutQAction;			///< Sign out Qt action instance, can be a null pointer, Qt will automatically destroy the instance


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/qaction/base/OnlineQActions-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::OnlineQActions)
