// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/Dialog.h"
#include "qsf_editor/dialog/sequence/SequenceMapBackup.h"
#include "qsf_editor/dialog/sequence/SequenceEditorState.h"

#include <qsf/job/JobProxy.h>
#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class SequenceDialog;
}
namespace camp
{
	class UserObject;
}
namespace qsf
{
	class Entity;
	class Component;
	class AssetProxy;
	class ClassSystem;
	namespace editor
	{
		namespace base
		{
			class Operation;
		}
		class AssetEditHelper;
		class QTrackTreeWidget;
		class EntityPreviewManager;
		class PropertiesFilterModel;
		class SequenceEditorWrapper;
	}
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
		*    QSF editor sequence dialog
		*/
		class SequenceDialog : public Dialog
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class SequenceEditorState;


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::SequenceDialog" unique pluginable dialog ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] dialogManager
			*    The dialog manager this dialog instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this dialog instance)
			*/
			SequenceDialog(DialogManager* dialogManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~SequenceDialog();


		//[-------------------------------------------------------]
		//[ Public virtual QDialog methods                        ]
		//[-------------------------------------------------------]
		public:
			virtual void reject() override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Dialog methods         ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;
			virtual void changeVisibility(bool visible) override;


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void closeEvent(QCloseEvent* qCloseEvent) override;
			virtual void paintEvent(QPaintEvent*) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			bool load();

			bool save();
			void saveAs();
			void exportSequence();

			void onTriggeredExit();

			void newSequence();

			void addSelectedEntitiesToSequence();
			void onTimeSelectionBeginOrEndChanged();

			void onTimeSelectionChanged(float timeSelectionBegin, float timeSelectionEnd);
			void setSequenceTimeRange(float newBeginTime, float newDuration);

			void onBeginTimeChanged();
			void onEndTimeChanged();
			void onPlayheadTimeOfInputBoxChanged();
			void onPlayheadTimeChanged(float newTime);
			void onPlaybackStateChanged(SequenceEditorState::PlaybackState newPlaybackState);

			void onStartForwardPlayback();
			void onStartBackwardPlayback();
			void onStopPlayback();
			void onTogglePlayback();
			void onToggleCaptureSequence();

			void onSelectNextKey();
			void onSelectPreviousKey();

			void onSelectionChanged();

			void setActiveSequence(SequenceEditorWrapper* sequence);

			void keyPropertyValueChanged(const camp::UserObject* campUserObject, const camp::Property* campProperty, const std::string& oldPropertyValue, const std::string& newPropertyValue);
			void editingFinishedKeyTimeValue();
			void editingFinishedKeyFrameNumberValue();
			void sequencePropertyValueChanged(const camp::UserObject* campUserObject, const camp::Property* campProperty, const std::string& oldPropertyValue, const std::string& newPropertyValue);

			void addKeysAtCurrentTimePosition();

			void updateDialogTitle();

			void selectedEntityChanged(Entity* newSelectedEntity);
			void onEntityComponentPropertyChanged(const Component& component, uint32 propertyId);

			// Listenes to undo stack clean state changes
			void onSequenceCleanChanged(bool isClean);

			// This method listens to the main editor operations, not the sequence editor operations
			void mainEditorUndoOrRedoOperationExecuted(const base::Operation& operation);

			void setTimeSelectionFromWidget(float beginTime, float endTime, bool isFinal);

			void setPlayheadTimeFromWidget(float oldTime, float newTime, bool isFinal);

			void onTriggeredCopy();
			void onTriggeredPaste();
			void onTriggeredDelete();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Set whether or not to listen to main editor undo/redo operations
			*
			*  @param[in] active
			*    "true" to listen on main editor undo/redo operations, else "false"
			*/
			void setMainEditorUndoOrRedoOperationListeningActive(bool active);

			/**
			*  @brief
			*    Hook into timely updates from the engine core
			*/
			void timeUpdateJob(const JobArguments& jobArguments);

			/**
			*  @brief
			*    Create a new sequence asset and begins editing it
			*
			*  @param[in] copyFromActive
			*    Copies sequence data from the currently active sequence for the new sequence instead of starting with a blank sequence
			*/
			bool createNewSequenceAssetAndBeginEditing(bool copyToFromActive);

			/**
			*  @brief
			*    Bring up an dialog which asks the user to ( (save or discard) and continue the action ) or to (cancel the action)
			*
			*  @returns
			*    Will return true in case the action is accepted. In this case the sequence is or is not saved depending on the "Save" or "Discard" option chosen
			*    Will return false if the action should be canceled. In this case the sequence is left as it is and should be kept so.
			*
			*  @note
			*    The long weird name comes from the side effect that it reads nicely in an "if" conditional
			*/
			bool userChosesToContinueSequenceDiscardingAction();

			/**
			*  @brief
			*    Extension of userChosesToContinueSequenceDiscardingAction() - will check if data is clean and if it is not,
			*    it will ask the user what to do. If the data is clean anyway, no user interaction is needed.
			*
			*  @returns
			*    true if the caller should continue the potentially discarding action and false if the caller should stop
			*/
			bool shouldContinueDiscardingAction();

			/**
			*  @brief
			*    Change the currently edited sequence in the sequencer
			*/
			void changeEditedSequence(const AssetProxy& previousSequenceAsset, std::shared_ptr<AssetEditHelper> assetEditHelper = nullptr);

			/**
			*  @brief
			*    Makes a backup of the current map state and decouples from the server.
			*
			*  @note
			*    - Stolen from: @see SimulationMode::backupMap
			*/
			void backupMap();

			/**
			*  @brief
			*    Restore the previously made map backup
			*/
			void restoreMap();

			/**
			*  @brief
			*    Called as soon as submitting the new sequence asset is done
			*/
			void onNewSequenceAssetSubmitDone(std::shared_ptr<AssetEditHelper> newAssetEditHelper, GlobalAssetId globalAssetId);

			void onSaveSequenceAssetSubmitDone();

			void onChangeSequenceAssetReady(GlobalAssetId globalAssetId);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			const ClassSystem&		mClassSystem;
			EntityPreviewManager*	mEntityPreviewManager;			///< Always valid while the sequencer dialog is shown, do not destroy the instance
			Ui::SequenceDialog*		mUiSequenceDialog;				///< UI sequence dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			std::string				mCurrentEditedSequenceFileName;	///< Filename of the currently editing sequence. TODO(oi) Should the reference be an asset?
			QTrackTreeWidget*		mTrackTreeWidget;				///< Manually constructed track tree widget
			SequenceEditorState*	mSequenceEditorState;
			SequenceEditorWrapper*	mActiveSequence;
			SequenceEditorWrapper*	mPreviouslyActiveSequence;
			JobProxy				mUpdateJobProxy;				///< Job proxy for time-dependent updates
			PropertiesFilterModel*	mPropertiesFilterModel;
			Entity*					mCurrentSelectedEntity;
			SequenceMapBackup		mSequenceMapBackup;				///< Storage for the sequence map backup during sequence editing
			bool					mIsChangingSelection;			///< Simple semaphore to prevent different selection parts to interfere and produce ping-pongs (editor vs. sequencer)
			std::shared_ptr<AssetEditHelper>		mAssetEditHelper;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::SequenceDialog)
