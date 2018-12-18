// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/view/View.h"

#include <camp/userobject.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class AnnotationBrowserView;
}
QT_BEGIN_NAMESPACE
	class QTableWidget;
	class QModelIndex;
QT_END_NAMESPACE
namespace qsf
{
	class AnnotationComponent;
	namespace editor
	{
		class SearchWidget;
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
		*    Annotation browser view class
		*
		*  @note
		*    - The UI is created via source code
		*    - Searches for all entities with "qsf::AnnotationComponent"-instances and presents them
		*/
		class AnnotationBrowserView : public View
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::AnnotationBrowserView" unique pluginable view ID


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
			AnnotationBrowserView(ViewManager* viewManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AnnotationBrowserView();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::View methods           ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;
			virtual void changeVisibility(bool visible) override;


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void showEvent(QShowEvent* qShowEvent) override;
			virtual void hideEvent(QHideEvent* qHideEvent) override;


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			static const int CATEGORY_COLUMN	= 0;	///< Column of the category
			static const int NAME_COLUMN		= 1;	///< Column of the action name


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
			*    Inform the annotation browser view that a component has been created
			*
			*  @param[in] entityId
			*    ID of the entity the created component is in
			*/
			void componentCreated(uint64 entityId);

			/**
			*  @brief
			*    Add a annotation component to the list
			*
			*  @param[in] annotationComponent
			*    Annotation component to add
			*/
			void addAnnotationComponent(const AnnotationComponent& annotationComponent);

			/**
			*  @brief
			*    Inform the annotation browser view that a component has been destroyed
			*
			*  @param[in] entityId
			*    ID of the entity the destroyed component is in
			*/
			void componentDestroyed(uint64 entityId);

			/**
			*  @brief
			*    Inform the annotation browser view that the annotation component has been changed
			*
			*  @param[in] entityId
			*    ID of the entity the updated component is in
			*/
			void updateComponentData(uint64 entityId);

			/**
			*  @brief
			*    Get the annotation component instance from the given entity
			*
			*  @param[in] entityId
			*   ID of the entity to return the annotation component from
			*
			*  @return
			*    The annotation component instance from the given entity, can be a null pointer, do not destroy the instance
			*/
			AnnotationComponent* getAnnotationComponentByEntityId(uint64 entityId) const;

			/**
			*  @brief
			*    Update the current selection inside the annotation browser view
			*/
			void updateSelection();

			/**
			*  @brief
			*    Enable/disable the annotation component data edit GUI elements
			*
			*  @param[in] enabled
			*    "true" to enable the annotation component data edit GUI elements, else "false"
			*/
			void setAnnotationComponentDataEditable(bool enabled);


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onCustomContextMenuRequested(const QPoint& qPoint);
			void onDoubleClicked(const QModelIndex& index);
			void onCurrentChanged(const QModelIndex& current, const QModelIndex& previous);
			void onDeleteItem();
			void onNameChanged();
			void onDescriptionChanged();
			void onTypeChanged(int type);
			void onSearchFieldTextChanged(const QString& searchText);
			// qsf::editor::OperationManager
			void onUndoOperationExecuted(const base::Operation& operation);
			void onRedoOperationExecuted(const base::Operation& operation);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			QTableWidget*				mQTableWidgetAnnotations;	///< Qt table widget instance, can be a null pointer, Qt will destroy the instance automatically
			Ui::AnnotationBrowserView*	mUiAnnotationBrowserView;	///< UI Annotation browser instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			SearchWidget*				mSearchWidget;				///< The search widget instance, can be a null pointer, Qt will destroy the instance automatically


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::AnnotationBrowserView)
