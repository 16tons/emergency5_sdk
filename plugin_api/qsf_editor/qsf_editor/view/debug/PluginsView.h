// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/view/debug/DebugView.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class Class;
	class TagHolder;
}
QT_BEGIN_NAMESPACE
	class QTreeWidget;
	class QTreeWidgetItem;
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
		*    Plugins view class
		*
		*  @note
		*    - The UI is created via source code
		*    - Introspection of "qsf::PluginSystem": Shows a list of all loaded plugins and CAMP metaclasses registered by each plugin
		*/
		class PluginsView : public DebugView
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::PluginsView" unique pluginable view ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] viewManager
			*    Optional pointer to the view manager this view should be registered to, can be a null pointer
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this view instance)
			*/
			PluginsView(ViewManager* viewManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~PluginsView();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::View methods           ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;
			virtual void changeVisibility(bool visible) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Fill the tree widget
			*/
			void fillTreeWidget();

			/**
			*  @brief
			*    Create a class tree widget item
			*
			*  @param[in] campClass
			*    CAMP metaclass to create the tree widget item for
			*
			*  @return
			*    The created class tree widget item, always valid, you're responsible for destroying the instance in case you no longer need it
			*/
			QTreeWidgetItem* createTreeWidgetItemClass(const camp::Class& campClass);

			/**
			*  @brief
			*    Add a tags tree widget item, if required
			*
			*  @param[in] qTreeWidgetItem
			*    Qt tree widget item to add the tags tree to, if required
			*  @param[in] campTagHolder
			*    CAMP tag holder to add the tree widget item for
			*/
			void addTreeWidgetItemTags(QTreeWidgetItem& qTreeWidgetItem, const camp::TagHolder& campTagHolder);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			QTreeWidget* mQTreeWidget;	///< Qt tree widget instance, can be a null pointer, Qt will destroy the instance automatically


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::PluginsView)
