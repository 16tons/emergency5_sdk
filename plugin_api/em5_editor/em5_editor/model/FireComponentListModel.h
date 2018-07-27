// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/platform/PlatformTypes.h>
#include <qsf/base/GetUninitialized.h>

#include <QtCore/QAbstractListModel>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class Component;
	namespace editor
	{
		namespace base
		{
			class Operation;
		}
	}
}

namespace em5
{
	class FireComponent;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Fire component list model
		*
		*/
		class FireComponentListModel : public QAbstractListModel
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
			enum FireComponentListModelRoles
			{
				FireEntityIdRole = Qt::UserRole+1
			};


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			explicit FireComponentListModel(QObject* parent);
			virtual ~FireComponentListModel();
			void updateFireComponentList(const std::vector<em5::FireComponent*> &fireComponentList);
			void onUndoOperationExecuted(const qsf::editor::base::Operation& operation);
			void onRedoOperationExecuted(const qsf::editor::base::Operation& operation);


		//[-------------------------------------------------------]
		//[ Protected virtual QAbstractListModel methods          ]
		//[-------------------------------------------------------]
		protected:
			virtual int rowCount(const QModelIndex & parent = QModelIndex()) const override;
			virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			struct FireComponentData
			{
				uint64			mEntityId;		//<< Entity ID to which the fire component belongs (we save the entity ID separately because the pointer itself can get invalid due an delete component operation)
				FireComponent*	mFireComponent;	//<< The fire component itself, this pointer can get invalid

				// Need an default constructor otherwise vector::emplace_back(<val>, <val2>) doesn't work...
				inline FireComponentData() :
					mEntityId(qsf::getUninitialized<uint64>()),
					mFireComponent(nullptr)
				{}

				// Need an constructor otherwise vector::emplace_back(<val>, <val2>) doesn't work...
				inline FireComponentData(const uint64 entityId, FireComponent* fireComponent) :
					mEntityId(entityId),
					mFireComponent(fireComponent)
				{}
			};


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void addFireComponentByEntity(const qsf::Entity* entity, qsf::Component* component);
			void removeFireComponentByEntity(const uint64 entityId);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::vector<FireComponentData>	mFireComponents;

		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
