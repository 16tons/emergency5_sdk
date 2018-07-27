// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/tool/Tool.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class CollisionTool;
}
namespace qsf
{
	class Entity;
	class Component;
	namespace editor
	{
		namespace base
		{
			class Operation;
		}
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
		*    Collision tool class
		*
		*  @note
		*    - The UI is created by using a Qt ui-file (create/edit it by using e.g. Qt Designer)
		*
		*  @todo
		*    - TODO(co) Make it possible to create a collision component instance of a certain type
		*/
		class CollisionTool : public QObject, public Tool
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::CollisionTool" unique pluginable tool ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] toolManager
			*    The tool manager this tool instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit CollisionTool(ToolManager* toolManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~CollisionTool();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Tool methods           ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(ToolboxView& toolboxView) override;
			virtual void onShutdown(ToolboxView& toolboxView) override;
			virtual void retranslateUi(ToolboxView& toolboxView) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onToggledEditMode(bool checked);
			void onClickedCreate(bool checked);
			void onClickedDelete(bool checked);
			// qsf::editor::EditModeManager
			void onChangeEditMode(EditMode* newEditMode, EditMode* previousEditMode);
			// qsf::editor::EntitySelectionManager
			void onChangeEntity(Entity* entity);
			// qsf::editor::OperationManager
			virtual void onUndoOperationExecuted(const base::Operation& operation);
			virtual void onRedoOperationExecuted(const base::Operation& operation);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Return the component this tool interacts with
			*
			*  @return
			*    The component this tool interacts with, can be a null pointer, do not destroy the instance
			*/
			Component* getComponent() const;

			void updateButtons();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::CollisionTool* mUiCollisionTool;	///< UI collision tool instance, always valid, we have to destroy the instance in case we no longer need it


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::CollisionTool)
