// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/view/View.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QTreeView;
	class QLabel;
QT_END_NAMESPACE
namespace qsf
{
	class ParameterGroup;
	namespace editor
	{
		namespace base
		{
			class Operation;
		}
		class ObjectPropertiesModel;
		class ObjectPropertiesFilterModel;
		class EntitySelectionManager;
		class SearchWidget;
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
		*    Object properties view class
		*
		*  @note
		*    - The UI is created via source code
		*/
		class ObjectPropertiesView : public View
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::ObjectPropertiesView" unique pluginable view ID


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
			ObjectPropertiesView(ViewManager* viewManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ObjectPropertiesView();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::View methods           ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;
			virtual void changeVisibility(bool visible) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onSearchFieldTextChanged(const QString& searchText);
			void onCustomContextMenuRequested(const QPoint& qPoint);
			// qsf::editor::OperationManager
			void onUndoOperationExecuted(const base::Operation& operation);
			void onRedoOperationExecuted(const base::Operation& operation);
			void onClickedExpandAll();
			void onClickedCollapseAll();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Perform a GUI rebuild
			*/
			void rebuildGui();

			/**
			*  @brief
			*    Update the object name
			*/
			void updateObjectName();

			/**
			*  @brief
			*    Update the current selection inside the property view
			*/
			void updateSelection();

			/**
			*  @brief
			*    Create the object properties browser model
			*/
			void createObjectPropertiesBrowserModel();

			/**
			*  @brief
			*    Destroy the object properties browser model
			*/
			void destroyObjectPropertiesBrowserModel();

			/**
			*  @brief
			*    Called as soon as there's a property change inside the settings group manager
			*
			*  @param[in] parameterGroup
			*    Parameter group instance the changed property is in
			*  @param[in] propertyId
			*    Unique class property ID the component property which values was changed
			*/
			void onSettingsGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			editor::EntitySelectionManager& mEntitySelectionManager;		///< Entity selection manager, do not destroy the instance
			QLabel*							mObjectNameLabel;				///< Qt label, shows a string of useful information, Qt will destroy the instance automatically
			QTreeView*						mQTreeView;						///< Qt table view instance, can be a null pointer, Qt will destroy the instance automatically
			ObjectPropertiesModel*			mObjectPropertiesModel;			///< Qt model instance, can be a null pointer, has mQTableView as parent, Qt will destroy the instance automatically
			QSortFilterProxyModel*	mObjectPropertiesFilterModel;	///< Qt model instance, can be a null pointer, has mObjectPropertiesFilterModel as parent, Qt will destroy the instance automatically
			SearchWidget*					mSearchWidget;					///< The search widget instance, Qt will destroy the instance automatically


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ObjectPropertiesView)
