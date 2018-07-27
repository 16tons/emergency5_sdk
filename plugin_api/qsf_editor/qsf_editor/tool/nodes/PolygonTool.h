// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/tool/nodes/NodesTool.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QPushButton;
QT_END_NAMESPACE
namespace Ui
{
	class PolygonTool;
}
namespace qsf
{
	namespace editor
	{
		class ToolboxView;
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
		*    Polygon tool class
		*/
		class PolygonTool : public NodesTool
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::PolygonTool" unique pluginable tool ID


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
			*  @param[in] componentId
			*    Component ID (e.g. "qsf::StringHash("qsf::BlockerPolygonComponent")")
			*/
			explicit PolygonTool(ToolManager* toolManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~PolygonTool();


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
			virtual void onToggledRectangle(bool checked);
			virtual void onToggledEllipse(bool checked);
			virtual void onToggledEditMode(bool checked);
			virtual void onToggledAddNode(bool checked);
			virtual void onToggledDeleteNode(bool checked);


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
			*    - In worst case it returns qsf::BlockerPolygonComponent::COMPONENT_ID
			*/
			unsigned int getComponentIdViaComboBox() const;


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			Ui::PolygonTool*	mUiPolygonTool;	///< UI polygon tool instance, always valid, we have to destroy the instance in case we no longer need it


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::PolygonTool)
