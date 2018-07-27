// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/tool/nodes/NodesTool.h"
#include "qsf_editor/operation/street/StreetOperationHelper.h"

#include <qsf/component/street/crossing/StreetCrossingComponent.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class NavigationTileTool;
}
namespace qsf
{
	class StreetCrossingComponent;
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
		*    Navigation tile tool class
		*
		*  @note
		*    - The UI is created by using a Qt ui-file (create/edit it by using e.g. Qt Designer)
		*/
		class NavigationTileTool : public NodesTool
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::NavigationTileTool" unique pluginable tool ID


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
			explicit NavigationTileTool(ToolManager* toolManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~NavigationTileTool();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Tool methods           ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(ToolboxView& toolboxView) override;
			virtual void onShutdown(ToolboxView& toolboxView) override;
			virtual void retranslateUi(ToolboxView& toolboxView) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::NodesTool methods      ]
		//[-------------------------------------------------------]
		protected:
			virtual void onChangeEditMode(EditMode* newEditMode, EditMode* previousEditMode) override;
			virtual void onChangeEntity(Entity* entity) override;
			virtual void onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onClickedSimpleI(bool);
			void onClickedSimpleT(bool);
			void onClickedSimpleX(bool);
			void onClickedComplexI(bool);
			void onClickedComplexT(bool);
			void onClickedComplexX(bool);
			void onToggledEditMode(bool checked);
			void onToggledAddNode(bool checked);
			void onToggledDeleteNode(bool checked);
			void onToggledAddEdge(bool);
			// qsf::editor::StreetCrossingEditMode
			void onEdgeSelectionChange(unsigned int id);


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
			*    Return the currently used street crossing component instance
			*
			*  @return
			*    The currently used street crossing component instance, can be a null pointer, do not destroy the instance
			*/
			StreetCrossingComponent* getStreetCrossingComponent() const;

			void createStreetCrossingComponent(StreetOperationHelper::InitialType initialType);
			void updateButtons();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::NavigationTileTool* mUiNavigationTileTool;	///< UI path tool instance, always valid, we have to destroy the instance in case we no longer need it


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::NavigationTileTool)
