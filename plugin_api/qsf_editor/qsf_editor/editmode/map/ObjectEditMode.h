// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/EditMode.h"
#include "qsf_editor/selection/entity/EntitySelectionManager.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Component;
	class Transform;
	class EntitySelectionRectangle;
	namespace editor
	{
		class EditorSettingsGroup;
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
		*    Object edit mode class
		*/
		class QSF_EDITOR_API_EXPORT ObjectEditMode : public EditMode
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::ObjectEditMode" unique pluginable edit mode ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] editModeManager
			*    The edit mode manager this edit mode instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit ObjectEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ObjectEditMode();

			/**
			*  @brief
			*    Update the gizmo
			*
			*  @note
			*    - This is public so systems which are not going over the operations can update the gizmo when they
			*      know the transformation of the selected object might have changed (e.g. the Sequencer)
			*/
			void updateGizmo();


		//[-------------------------------------------------------]
		//[ Public virtual QObject methods                        ]
		//[-------------------------------------------------------]
		public:
			virtual bool eventFilter(QObject* qObject, QEvent* qEvent) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::EditMode methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(EditMode* previousEditMode) override;
			virtual void onShutdown(EditMode* nextEditMode) override;
			virtual void mousePressEvent(QMouseEvent& qMouseEvent) override;
			virtual void mouseReleaseEvent(QMouseEvent& qMouseEvent) override;
			virtual void mouseMoveEvent(QMouseEvent& qMouseEvent) override;
			virtual void focusInEvent(QFocusEvent& qFocusEvent) override;
			virtual void focusOutEvent(QFocusEvent& qFocusEvent) override;


		//[-------------------------------------------------------]
		//[ Protected Qt slots (MOC)                              ]
		//[-------------------------------------------------------]
		protected Q_SLOTS:
			void onCustomContextMenuRequested(const QPoint& qPoint);
			void onTriggeredContextSensitive();
			void onTriggeredCut();
			void onTriggeredCopy();
			void onTriggeredPaste();
			void onTriggeredDelete();
			void onTriggeredSnapToGround();
			void onTriggeredSnapToObject();
			void onTriggeredSnapToVertex();
			void onQClipboardDataChanged();
			// qsf::editor::OperationManager
			void onUndoOperationExecuted(const base::Operation& operation);
			void onRedoOperationExecuted(const base::Operation& operation);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Return the world space position of the currently pasted entity
			*
			*  @param[out] position
			*    Receives the entity world space position on success, not touched on failure
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			bool getEntityPastePosition(glm::vec3& position) const;

			/**
			*  @brief
			*    Update the cut marker component
			*
			*  @param[in] currentEditMode
			*    Current edit mode, can be a null pointer
			*/
			void updateCutMarkerComponent(EditMode* currentEditMode);

			/**
			*  @brief
			*    Perform component selection filtering
			*
			*  @param[in] component
			*    Component instance to check for filtering, can be a null pointer
			*
			*  @return
			*    The given component instance in case it was not filtered, else a null pointer
			*/
			const Component* performComponentSelectionFiltering(const Component* component) const;

			/**
			*  @brief
			*    If the settings say the user can only allow viewport selection on selected layer/sublayer, we have to check this
			*
			*  @param[in] component
			*    The component the user selects in viewport
			*
			*  @return
			*    Return true if the object selection is valid, this happens if the settings flag are disabled or the selected entity
			*    is on the same layer or sublayer of the selected layer
			*/
			bool checkSettingsAllowSelectionOfComponent(const Component& component) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			editor::EntitySelectionManager&	mEntitySelectionManager;	///< Entity selection manager instance (cached instance for more efficient access)
			const EditorSettingsGroup&		mEditorSettingsGroup;		///< Editor settings group instance (cached instance for more efficient access)
			EntitySelectionManager::IdSet	mCutEntityIdSet;			///< IDs of the currently cut entities
			EntitySelectionRectangle*		mSelectionRectangle;		///< Selection rectangle, can be a null pointer, we're responsible for destroying it


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ObjectEditMode)
