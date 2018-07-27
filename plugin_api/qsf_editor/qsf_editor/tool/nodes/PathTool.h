// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/tool/nodes/NodesTool.h"

#include "qsf/debug/DebugDrawProxy.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class PathTool;
}
namespace qsf
{
	class PathComponent;
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
		*    Path tool class
		*
		*  @note
		*    - The UI is created by using a Qt ui-file (create/edit it by using e.g. Qt Designer)
		*/
		class PathTool : public NodesTool
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::PathTool" unique pluginable tool ID


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
			explicit PathTool(ToolManager* toolManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~PathTool();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Tool methods           ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(ToolboxView& toolboxView) override;
			virtual void retranslateUi(ToolboxView& toolboxView) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::NodesTool methods      ]
		//[-------------------------------------------------------]
		protected:
			virtual void onChangeEditMode(EditMode* newEditMode, EditMode* previousEditMode) override;
			virtual void onChangeEntity(Entity* entity) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			virtual void onToggledLine(bool checked);
			virtual void onToggledEditMode(bool checked);
			virtual void onToggledAddNode(bool checked);
			virtual void onToggledAddNodeOnSeqment(bool checked);
			virtual void onToggledDeleteNode(bool checked);

			void onPreviewClicked(bool checked);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    In case the current edit mode is one of the edit modes orchestrated by this tool, switch back to the object edit mode
			*/
			void switchToSelectionEditMode();

			/**
			*  @brief
			*    Return the component ID via the index of the combo box
			*
			*  @note
			*    - If the .ui file is changing the displayed order of the combo box, this function has to updated
			*    - In worst case it returns "qsf::PathComponent::COMPONENT_ID "
			*/
			unsigned int getComponentIdViaComboBox() const;

			PathComponent* getPathComponent() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::PathTool* mUiPathTool;	///< UI path tool instance, always valid, we have to destroy the instance in case we no longer need it
			qsf::DebugDrawProxy mPreviewDebugProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::PathTool)
