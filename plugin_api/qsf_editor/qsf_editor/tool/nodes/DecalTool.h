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
namespace Ui
{
	class DecalTool;
}
namespace qsf
{
	class DecalComponent;
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
		*    Decal tool class
		*
		*  @note
		*    - The UI is created by using a Qt ui-file (create/edit it by using e.g. Qt Designer)
		*/
		class DecalTool : public NodesTool
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::DecalTool" unique pluginable tool ID


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
			explicit DecalTool(ToolManager* toolManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~DecalTool();


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
			virtual void onNodeChange(Node& node) override;
			virtual void onChangeEditMode(EditMode* newEditMode, EditMode* previousEditMode) override;
			virtual void onChangeEntity(Entity* entity) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onToggledEditMode(bool checked);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Return the decal component this tool interacts with
			*
			*  @return
			*    The decal component this tool interacts with, can be a null pointer, do not destroy the instance
			*/
			DecalComponent* getDecalComponent() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::DecalTool* mUiDecalTool;	///< UI decal tool instance, always valid, we have to destroy the instance in case we no longer need it


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::DecalTool)
