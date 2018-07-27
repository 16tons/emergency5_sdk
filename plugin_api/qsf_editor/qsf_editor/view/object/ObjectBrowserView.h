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
	class QModelIndex;
QT_END_NAMESPACE
namespace Ui
{
	class ObjectBrowserView;
}
namespace qsf
{
	class Layer;
	class ParameterGroup;
	namespace editor
	{
		namespace base
		{
			class Operation;
		}
		class ObjectBrowserModel;
		class ObjectBrowserFilterModel;
		class FilterConfiguration;
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
		*    Object browser view class
		*
		*  @note
		*    - The UI is created by using a Qt ui-file (create/edit it by using e.g. Qt Designer)
		*    - Introspection of "qsf::Map" with a sophisticated view incorporating layers, folders and the special object view of the entity relationships
		*    - The name "Object Browser" is the decision of the editor vision keeper, that's the reason why this class is named "qsf::editor::ObjectBrowserView" and not "qsf::editor::ObjectsView"
		*/
		class ObjectBrowserView : public View
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::ObjectBrowserView" unique pluginable view ID


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
			ObjectBrowserView(ViewManager* viewManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ObjectBrowserView();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::View methods           ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;
			virtual void changeVisibility(bool visible) override;


		//[-------------------------------------------------------]
		//[ Protected virtual QObject methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void timerEvent(QTimerEvent* qTimerEvent) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onCustomContextMenuRequested(const QPoint& qPoint);
			void onClicked(const QModelIndex& qModelIndex);
			void onDoubleClicked(const QModelIndex& qModelIndex);
			void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
			void onButtonAddLayerClicked();
			void onButtonAddFolderClicked();
			void onButtonExpandLayerClicked();
			void onButtonCollapseLayerClicked();
			void onButtonDeleteClicked();
			void onButtonFilterSelectionClicked();
			// qsf::editor::OperationManager
			void onUndoOperationExecuted(const base::Operation& operation);
			void onRedoOperationExecuted(const base::Operation& operation);
			// qsf::editor::LayerSelection
			void onSelectedLayerChanged();
			// Search widget
			void onSearchFilterChanged(const FilterConfiguration& newFilter);
			void onSearchFieldTextChanged(const QString& searchText);
			void onSearchFieldReturnPressed();
			// qsf::editor::Application
			void onPreMapRebuild();
			void onPostMapRebuild();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
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

			/**
			*  @brief
			*    Update the enabled state of the buttons
			*
			*  @param[in] qModelIndex
			*    Qt model index to use
			*/
			void updateButtonsEnabledState(const QModelIndex& qModelIndex);

			/**
			*  @brief
			*    Update the current selection inside the entities view
			*
			*  @param[in] entityOnly
			*    Do only take entities into selection consideration?
			*/
			void updateSelection(bool entityOnly = false);

			/**
			*  @brief
			*    Create the object browser model
			*/
			void createObjectBrowserModel();

			/**
			*  @brief
			*    Destroy the object browser model
			*/
			void destroyObjectBrowserModel();

			void stopSearchQtTimerId();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			QTreeView*					mQTreeView;					///< Qt tree view instance, can be a null pointer, Qt will destroy the instance automatically
			ObjectBrowserFilterModel*	mObjectBrowserFilterModel;	///< Filter for sorting non object entities in mObjectBrowserModel, Qt will destroy the instance automatically ("mQTreeView" is the parent)
			ObjectBrowserModel*			mObjectBrowserModel;		///< Entities model instance, can be a null pointer, Qt will destroy the instance automatically ("mObjectBrowserFilterModel" is the parent)
			Ui::ObjectBrowserView*		mUiObjectBrowserView;		///< UI object browser instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			SearchWidget*				mSearchWidget;				///< The search widget instance, Qt will destroy the instance automatically
			int							mSearchQtTimerId;			///< Search Qt timer ID, "0" means no timer
			bool						mFilterSelection;			///< If true, filter the selection in the tree view


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ObjectBrowserView)
