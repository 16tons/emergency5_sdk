// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/qaction/QActions.h"
#include "qsf_editor/renderer/gizmo/GizmoComponent.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QMenu;
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
		*    Transform QActions category
		*/
		class TransformQActions : public QActions
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::TransformQActions" unique pluginable QActions ID


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
			explicit TransformQActions(QActionsManager* qActionsManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~TransformQActions();

			//[-------------------------------------------------------]
			//[ QAction                                               ]
			//[-------------------------------------------------------]

			/** Return the Select Move Mode Action, can be a null pointer, do not destroy the returned instance */
			inline QAction* getSelectMoveModeQAction() const;

			/** Return the Select Rotate Mode Action, can be a null pointer, do not destroy the returned instance */
			inline QAction* getSelectRotateModeQAction() const;

			/** Return the Select Scale Mode Action, can be a null pointer, do not destroy the returned instance */
			inline QAction* getSelectScaleModeQAction() const;

			/** Return the Select x-axis Action, can be a null pointer, do not destroy the returned instance */
			inline QAction* getToggleXAxisQAction() const;

			/** Return the Select y-axis Action, can be a null pointer, do not destroy the returned instance */
			inline QAction* getToggleYAxisQAction() const;

			/** Return the Select z-axis Action, can be a null pointer, do not destroy the returned instance */
			inline QAction* getToggleZAxisQAction() const;

			// Flip axis
			inline QAction* getFlipAxisXQAction() const;
			inline QAction* getFlipAxisYQAction() const;
			inline QAction* getFlipAxisZQAction() const;

			inline QAction* getAlignAllNodesComponentsToCenterQAction() const;

			/** Return the Toggle Local Transformation Action, can be a null pointer, do not destroy the returned instance */
			inline QAction* getToggleLocalQAction() const;

			/** Return the Toggle Follow Ground Action, can be a null pointer, do not destroy the returned instance */
			inline QAction* getToggleFollowGroundQAction() const;

			inline QAction* getToggleGridVisibilityQAction() const;
			inline QAction* getChangeGridSizeQAction() const;

			/** Return the Toggle Snap to Grid Action, can be a null pointer, do not destroy the returned instance */
			inline QAction* getToggleSnapToGridQAction() const;

			/** Return the Snap to Ground Action, can be a null pointer, do not destroy the returned instance */
			inline QAction* getSnapToGroundQAction() const;

			/** Return the Snap to Object Action, can be a null pointer, do not destroy the returned instance */
			inline QAction* getSnapToObjectQAction() const;

			/** Return the Snap to Vertex Action, can be a null pointer, do not destroy the returned instance */
			inline QAction* getSnapToVertexQAction() const;

			/** Return the Snap to Current Camera Action, can be a null pointer, do not destroy the returned instance */
			inline QAction* getSnapToCurrentCameraQAction() const;

			/** Return the Reset rotation action, can be a null pointer, do not destroy the returned instance */
			inline QAction* getResetRotationQAction() const;

			//[-------------------------------------------------------]
			//[ Helper                                                ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Add Qt transform actions to the given Qt menu
			*
			*  @param[in] qMenu
			*    Qt menu were to add the Qt transform actions to
			*/
			void addQActionsToMenu(QMenu& qMenu) const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::QActions methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onTriggeredSwitchMovementMode();
			void onTriggeredSwitchRotateMode();
			void onTriggeredSwitchScaleMode();

			void onTriggeredToggleAxisX();
			void onTriggeredToggleAxisY();
			void onTriggeredToggleAxisZ();

			void onTriggeredFlipAxisX();
			void onTriggeredFlipAxisY();
			void onTriggeredFlipAxisZ();

			void onTriggeredAlignAllNodesComponentsToCenter();

			void onTriggeredSelectAxisX();
			void onTriggeredSelectAxisY();
			void onTriggeredSelectAxisZ();
			void onTriggeredSelectAxisXZ();
			void onTriggeredSelectAxisXYZ();

			void onTriggeredToggleLocal();
			void onTriggeredToggleFollowGround();
			void onTriggeredToggleGridVisibility();
			void onTriggeredToggleSnapToGrid();

			void onTriggeredShowHideGizmo();
			void onTriggeredSnapToCurrentCamera();
			void onTriggerResetRotation();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void changeTransformationMode(GizmoComponent::TransformationMode mode);
			void toggleTransformationAxis(GizmoComponent::TransformationAxis axis);
			void selectTransformationAxis(GizmoComponent::TransformationAxis axis);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			/** Qt action instances, can be null pointers, Qt will automatically destroy the instances */
			QAction* mSwitchMovementModeQAction;
			QAction* mSwitchRotateModeQAction;
			QAction* mSwitchScaleModeQAction;

			QAction* mToggleAxisXQAction;
			QAction* mToggleAxisYQAction;
			QAction* mToggleAxisZQAction;

			QAction* mFlipAxisXQAction;
			QAction* mFlipAxisYQAction;
			QAction* mFlipAxisZQAction;

			QAction* mAlignAllNodesComponentsToCenterQAction;

			QAction* mSelectAxisXQAction;
			QAction* mSelectAxisYQAction;
			QAction* mSelectAxisZQAction;
			QAction* mSelectAxisXZQAction;
			QAction* mSelectAxisXYZQAction;

			QAction* mToggleLocalQAction;
			QAction* mToggleFollowGroundQAction;
			QAction* mToggleGridVisibilityQAction;
			QAction* mChangeGridSizeQAction;
			QAction* mToggleSnapToGridQAction;

			QAction* mShowHideGizmoQAction;
			QAction* mSnapToGroundQAction;
			QAction* mSnapToObjectQAction;
			QAction* mSnapToVertexQAction;
			QAction* mSnapToCurrentCameraQAction;
			QAction* mResetRotationQAction;


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
#include "qsf_editor/qaction/edit/TransformQActions-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::TransformQActions)
