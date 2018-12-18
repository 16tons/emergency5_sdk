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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
}


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
		*    Edit QActions category
		*/
		class EditQActions : public QActions
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::EditQActions" unique pluginable QActions ID


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
			explicit EditQActions(QActionsManager* qActionsManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~EditQActions();

			//[-------------------------------------------------------]
			//[ Undo/redo QAction                                     ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the undo Qt action
			*
			*  @return
			*    The undo Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getUndoQAction() const;

			/**
			*  @brief
			*    Return the redo Qt action
			*
			*  @return
			*    The redo Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getRedoQAction() const;

			//[-------------------------------------------------------]
			//[ Basic QAction                                         ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the cut Qt action
			*
			*  @return
			*    The cut Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getCutQAction() const;

			/**
			*  @brief
			*    Return the copy Qt action
			*
			*  @return
			*    The copy Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getCopyQAction() const;

			/**
			*  @brief
			*    Return the paste Qt action
			*
			*  @return
			*    The paste Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getPasteQAction() const;

			/**
			*  @brief
			*    Update the state of the paste Qt action
			*
			*  @todo
			*    - TODO(co) I'm sorry, I don't understand what this method is doing, looks like a hack we don't want to have
			*/
			void updatePasteQAction() const;

			/**
			*  @brief
			*    Return the delete Qt action
			*
			*  @return
			*    The delete Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getDeleteQAction() const;

			/**
			*  @brief
			*    Return the rename Qt action
			*
			*  @return
			*    The rename Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getRenameQAction() const;

			//[-------------------------------------------------------]
			//[ Dialog QAction                                        ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the map properties dialog Qt action
			*
			*  @return
			*    The map properties dialog Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getMapPropertiesDialogQAction() const;

			/**
			*  @brief
			*    Return the material browser dialog Qt action
			*
			*  @return
			*    The material browser dialog Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getMaterialBrowserQAction() const;

			/**
			*  @brief
			*    Return the sequence dialog Qt action
			*
			*  @return
			*    The sequence dialog Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getSequenceQAction() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::QActions methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup() override;
			virtual void retranslateUi() override;
			virtual void onShutdown() override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			// Slots for our own QAction instances
			void onTriggeredMapPropertiesDialog();
			void onTriggeredMaterialBrowserDialog();
			void onTriggeredSequenceDialog();
			// Slots for other signals
			void onQClipboardDataChanged();
			// qsf::editor::EntitySelectionManager
			void onChangeEntity(Entity* entity);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Undo/redo
			QAction* mUndoQAction;					///< Undo Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mRedoQAction;					///< Redo Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			// Basic
			QAction* mCutQAction;					///< Cut Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mCopyQAction;					///< Copy Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mPasteQAction;					///< Paste Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mDeleteQAction;				///< Delete Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mRenameQAction;				///< Rename Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			// Dialogs
			QAction* mMapPropertiesDialogQAction;	///< Map properties dialog Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mMaterialBrowserDialogQAction;	///< Material browser dialog Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mSequenceDialogQAction;		///< Sequence dialog Qt action instance, can be a null pointer, Qt will automatically destroy the instance


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
#include "qsf_editor/qaction/edit/EditQActions-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::EditQActions)
