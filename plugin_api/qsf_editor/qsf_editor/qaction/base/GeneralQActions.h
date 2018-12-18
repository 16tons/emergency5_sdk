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
		*    General QActions category
		*/
		class GeneralQActions : public QActions
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::GeneralQActions" unique pluginable QActions ID


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
			explicit GeneralQActions(QActionsManager* qActionsManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~GeneralQActions();

			//[-------------------------------------------------------]
			//[ QAction                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the asset browser Qt action
			*
			*  @return
			*    The asset browser Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getAssetBrowserQAction() const;

			/**
			*  @brief
			*    Return the asset maintenance Qt action
			*
			*  @return
			*    The asset maintenance Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getAssetMaintenanceQAction() const;

			/**
			*  @brief
			*    Return the check for conflicts Qt action
			*
			*  @return
			*    The check for conflicts Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getCheckForConflictsQAction() const;

			/**
			*  @brief
			*    Return the disconnect Qt action
			*
			*  @return
			*    The disconnect Qt action, can be a null pointer, do not destroy the returned instance
			*
			*  @remarks
			*    This action is inside this general class by intent. It must be possible to disconnect from an "offline server"
			*    in order to be able to connect to an online server. So don't move this action into e.g. online editor only.
			*/
			inline QAction* getDisconnectQAction() const;

			/**
			*  @brief
			*    Return the exit Qt action
			*
			*  @return
			*    The exit Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getExitQAction() const;

			/**
			*  @brief
			*    Return the context sensitive Qt action
			*
			*  @return
			*    The context sensitive Qt action, can be a null pointer, do not destroy the returned instance
			*
			*  @remarks
			*    This Qt action is context sensitive. For instance, if an entity with a street section is selected and the
			*    action is triggerd, the editor will change into the street section edit mode.
			*/
			inline QAction* getContextSensitiveQAction() const;

			/**
			*  @brief
			*    Add batch job Qt actions to the given Qt menu
			*
			*  @param[in] qMenu
			*    Qt menu to add the batch job Qt actions to
			*/
			void addBatchJobQActionsToQMenu(QMenu& qMenu) const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::QActions methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onTriggeredAssetBrowser();
			void onTriggeredAssetMaintenance();
			void onTriggeredCheckForConflicts();
			void onTriggeredDisconnect();
			void onTriggeredExit();
			void onTriggeredBatchJob();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			QAction* mAssetBrowserQAction;		///< Asset browser Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mAssetMaintenanceQAction;	///< Asset maintenance Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mBatchProcessingQAction;	///< Batch processing Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mCheckForConflictsQAction;	///< Check for conflicts Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mDisconnectQAction;		///< Disconnect Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mExitQAction;				///< Exit Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mContextSensitiveQAction;	///< Context sensitive Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			std::vector<QAction*> mBatchJobQActions;


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
#include "qsf_editor/qaction/base/GeneralQActions-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::GeneralQActions)
