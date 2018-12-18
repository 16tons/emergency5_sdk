// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/tool/Tool.h"
#include "qsf_editor/model/properties/GenericPropertiesModel.h"

#include <QtCore/QAbstractListModel>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class MassComponentEditTool;
}
namespace qsf
{
	class Entity;
	class Component;
	namespace editor
	{
		class ObjectPropertiesDelegate;
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
		// TODO(sw) For a quick test -> move this to separate file
		class ComponentListModel : public QAbstractListModel
		{
			Q_OBJECT

		public:
			explicit ComponentListModel(QObject *parent = 0);

			void setComponents(const std::vector<qsf::Component*>& componentList);

			QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
			int rowCount(const QModelIndex &parent) const override;

		private:
			struct ItemData
			{
				uint32 componentId;
				QString name;
			};

		private:
			std::vector<ItemData> mItems;

		};

		class MassComponentEditPropertiesModel : public GenericPropertiesModel
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
			*  @param[in] parent
			*    Optional parent, can be a null pointer, in case there's a parent it will take over the (memory) control of this model
			*  @param[in] campUserObject
			*    CAMP user object to list the properties from, must be valid, the instance the user object is wrapping up must stay valid as long as this properties model exist (not the "camp::UserObject"-instance itself)
			*/
			MassComponentEditPropertiesModel(QObject* parent, const camp::UserObject& campUserObject);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~MassComponentEditPropertiesModel();

			/**
			*  @brief
			*    Sets the affected entities
			*
			*  @note
			*    The content gets moved
			*/
			void setAffectedEntities(std::vector<qsf::Entity*>& affectedEntities);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::GenericPropertiesModel methods ]
		//[-------------------------------------------------------]
		protected:
			virtual void onPropertyValueChanged(const camp::UserObject& campUserObject, const camp::Property& campProperty, const std::string& newPropertyValue) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::vector<qsf::Entity*> mAffectedEntities;


		};



		/**
		*  @brief
		*    Mass component edit tool class
		*
		*  @note
		*    - The UI is created by using a Qt ui-file (create/edit it by using e.g. Qt Designer)
		*/
		class MassComponentEditTool : public QObject, public Tool
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::MassComponentEditTool" unique pluginable tool ID


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
			explicit MassComponentEditTool(ToolManager* toolManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~MassComponentEditTool();


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
			// qsf::editor::EntitySelectionManager
			void onChangeEntity(Entity* entity);

			// qsf::editor::OperationManager
			virtual void onUndoOperationExecuted(const base::Operation& operation);
			virtual void onRedoOperationExecuted(const base::Operation& operation);

			void onCurrentChanged(const QModelIndex& current, const QModelIndex& previous);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void setupModels();
			void updatePropertiesModel(qsf::Component* component);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::MassComponentEditTool*			mUiMassComponentEditTool;	///< UI collision tool instance, always valid, we have to destroy the instance in case we no longer need it
			ComponentListModel*					mComponentListModel;
			MassComponentEditPropertiesModel*	mPropertiesModel;
			ObjectPropertiesDelegate*			mPropertiesDelegate;
			uint32								mSelectedComponentId;
			QMetaObject::Connection				mSelectionChangedConnection;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::MassComponentEditTool)
