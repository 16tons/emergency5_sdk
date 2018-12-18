// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/reflection/object/ObjectProxy.h>

#include <QtWidgets/qtreeview.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Object;
	namespace editor
	{
		class GenericPropertiesModel;
		class PropertiesModel;
		class PropertiesFilterModel;
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
		*    Properties widget class
		*/
		class QSF_EDITOR_API_EXPORT PropertiesWidget : public QTreeView
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this widget instance)
			*  @param[in] object
			*    Object to operate on, can be a null pointer
			*  @param[in] supportEntityMultiSelection
			*    Support entity multiSelection?
			*/
			PropertiesWidget(QWidget* qWidgetParent = nullptr, Object* object = nullptr, bool supportEntityMultiSelection = true);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~PropertiesWidget();

			/**
			*  @brief
			*    Return the inspected object instance
			*
			*  @return
			*    Inspected object instance, can be an null pointer, do not destroy the instance
			*/
			Object* getInspectedObject() const;

			/**
			*  @brief
			*    Set the inspected object instance
			*
			*  @param[in] object
			*    Object instance to inspect, can be a null pointer
			*/
			void setInspectedObject(Object* object);

			/**
			*  @brief
			*    Set the modified flag state of the underlying properties model
			*
			*  @param[in] isModified
			*    The new modified flag state
			*/
			void setModified(bool isModified);

			/**
			*  @brief
			*    Return the current modified flag state of the underlying properties model
			*
			*  @return
			*    The current modified flag state
			*/
			bool isModified() const;

			//[-------------------------------------------------------]
			//[ Qt model                                              ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the properties model
			*
			*  @return
			*    Properties model, can be a null pointer, do not destroy the instance
			*/
			PropertiesModel* getPropertiesModel() const;

			/**
			*  @brief
			*    Return the properties filter model
			*
			*  @return
			*    Properties filter model, can be a null pointer, do not destroy the instance
			*/
			PropertiesFilterModel* getPropertiesFilterModel() const;


		//[-------------------------------------------------------]
		//[ Virtual protected qsf::editor::PropertiesWidget methods ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Create the property model which should be used to fill the widget with properties from the object
			*
			*  @return
			*    New properties model, ownership goes to the caller
			*
			*  @note
			*    This method is virtualized so child classes can put in their own properties model
			*/
			virtual GenericPropertiesModel* createPropertiesModelForObject(PropertiesFilterModel* filterModel, Object* forObject);


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void dragEnterEvent(QDragEnterEvent* qDragEnterEvent) override;
			virtual void dragMoveEvent(QDragMoveEvent* event) override;
			virtual void dropEvent(QDropEvent* qDropEvent) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onCustomContextMenuRequested(const QPoint& qPoint);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Called in case the object proxy receives a new object
			*
			*  @param[in] object
			*    New object instance, can be a null pointer
			*/
			void onSetObject(Object* object);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool					mSupportEntityMultiSelection;
			ObjectProxy				mObjectProxy;			///< Object proxy wrapping up the inspected object, is automatically updated in case the wrapped up object gets destroyed
			PropertiesFilterModel*	mPropertiesFilterModel;	///< Properties filter model instance, can be a null pointer, Qt will destroy the instance automatically


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
