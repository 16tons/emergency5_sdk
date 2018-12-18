// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/model/FireMaterialsModel.h"

#include <em5/fire/FireSystem.h>
#include <em5/fire/FireMaterialManager.h>

#include <qsf_editor/base/GuiHelper.h>
#include <qsf_editor/model/properties/PropertiesModelDefinitions.h> // Needed because the view uses the object properties delegate

#include <qsf/reflection/CampHelper.h>
#include <qsf/reflection/ClassSystem.h>
#include <qsf/QsfHelper.h>

#include <QtCore/qcoreapplication.h>
#include <QtGui/qfont.h>
#include <QtGui/QBrush>
#include <QtGui/QPalette>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		FireMaterialsModel::FireMaterialsModel(QObject* parent, FireSystem& fireSystem) :
			BaseModel(parent),
			mFireSystem(fireSystem),
			mEditingEnabled(false) // Default no editing allowed
		{
			setupModelData(fireSystem, mRootTreeItem);
			fireSystem.getFireMaterialManager().FireMaterialsChanged.connect(boost::bind(&FireMaterialsModel::onFireMaterialsChanged, this));
		}

		FireMaterialsModel::~FireMaterialsModel()
		{
			mFireSystem.getFireMaterialManager().FireMaterialsChanged.disconnect(boost::bind(&FireMaterialsModel::onFireMaterialsChanged, this));
		}

		void FireMaterialsModel::enableEditing(bool enabled)
		{
			mEditingEnabled = enabled;

			dataChanged(index(0, 0, QModelIndex()), index(rowCount() - 1, columnCount() - 1, QModelIndex()));
		}

		void FireMaterialsModel::refresh()
		{
			beginResetModel();

			// Clear old items
			mRootTreeItem->clearChildrens();

			// Reload data from fire system
			setupModelData(mFireSystem, mRootTreeItem);

			endResetModel();
		}


		//[-------------------------------------------------------]
		//[ Public virtual QAbstractItemModel methods             ]
		//[-------------------------------------------------------]
		QModelIndex FireMaterialsModel::parent(const QModelIndex& child) const
		{
			// All data is top level data
			return QModelIndex();
		}

		int FireMaterialsModel::rowCount(const QModelIndex& parent) const
		{
			// Get the number of fire materials
			return static_cast<int>(mFireSystem.getFireMaterialManager().getElements().size());
		}

		int FireMaterialsModel::columnCount(const QModelIndex& parent) const
		{
			// Get the number of properties fire materials have
			static const int propertyCount = static_cast<int>(camp::classByType<FireMaterial>().propertyCount());
			return propertyCount;
		}

		bool FireMaterialsModel::hasChildren(const QModelIndex& parent) const
		{
			// TODO(fr) Check if this function is necessary, return value is always false
			return false;
		}

		QVariant FireMaterialsModel::data(const QModelIndex& index, int role) const
		{
			// Evaluate the requested role
			if (index.isValid())
			{
				switch (role)
				{
					case Qt::DisplayRole:
					case Qt::EditRole:
					{
						// Check if the model index is valid
						if (index.isValid())
						{
							IndexData* indexData = getIndexDataByQModelIndex(index);
							if (nullptr != indexData && static_cast<IndexType>(indexData->indexType) == INDEXTYPE_FIREMATERIAL)
							{
								FireMaterial* fireMaterial = static_cast<FireMaterial*>(indexData->object);

								// Return name or value of the property
								const camp::UserObject& campUserObject = camp::UserObject(fireMaterial);
								const camp::Property& campProperty = campUserObject.getClass().getPropertyByIndex(index.column());
								return qsf::editor::GuiHelper::getCampPropertyValueAsGuiString(campUserObject, campProperty);
							}
						}
						return QVariant();
					}

					// The fire material description goes here
					case Qt::ToolTipRole:
					{
						IndexData* indexData = getIndexDataByQModelIndex(index);
						if (nullptr != indexData && static_cast<IndexType>(indexData->indexType) == INDEXTYPE_FIREMATERIAL)
						{
							// Store the CAMP metaclass class
							static const camp::Class& campClass = camp::classByType<FireMaterial>();
							std::string name = campClass.getPropertyByIndex(index.column()).tag("Description").to<std::string>();
							return QCoreApplication::translate("", name.c_str());
						}
						return QVariant();
					}

					case Qt::ForegroundRole:
					{
						if (!mEditingEnabled)
							return QBrush( QPalette::Disabled); // TODO(sw) This color isn't override able by a qt style
						return QVariant();
					}

					case Qt::FontRole:
					{
						// Get the CAMP property
						const IndexData* indexData = getIndexDataByQModelIndex(index);
						if (nullptr != indexData && static_cast<IndexType>(indexData->indexType) == INDEXTYPE_FIREMATERIAL)
						{
							const camp::Class& campClass = camp::UserObject(indexData->object).getClass();
							const camp::Property& campProperty = campClass.getPropertyByIndex(index.column());
							qsf::Object* qsfObject = static_cast<qsf::Object*>(indexData->object);

							// Override state of the property
							// -> Can be equal to the property value inside a prototype
							// -> Can be equal to the default value specified in C++
							// In case the default value is overridden, use a bold font
							if (qsfObject->getPropertyOverrideState(campProperty.id()) == qsf::Object::PROPERTY_OVERRIDE)
							{
								QFont qFontBold;
								qFontBold.setBold(true);
								return qFontBold;
							}
						}
						return QVariant();
					}

					case qsf::editor::PROPERTIESMODELROLE_PROPERTY:
					{
						qsf::editor::TreeItem<IndexData>* treeItem = getTreeItemByQModelIndex(index);
						const IndexData* indexData = treeItem->data();
						if (nullptr != indexData && static_cast<IndexType>(indexData->indexType) == INDEXTYPE_FIREMATERIAL)
						{
							const camp::Property* campProperty = &indexData->object->getCampClass().getPropertyByIndex(index.column());
							return QVariant::fromValue(campProperty);
						}
						break;
					}

					case qsf::editor::PROPERTIESMODELROLE_OBJECT:
					{
						qsf::editor::TreeItem<IndexData>* treeItem = getTreeItemByQModelIndex(index);
						const IndexData* indexData = treeItem->data();
						if (nullptr != indexData)
						{
							return QVariant::fromValue(indexData->object);
						}
						break;
					}

					default:
						return QVariant();
				}
			}

			// We should never ever be here
			return QVariant();
		}

		QVariant FireMaterialsModel::headerData(int section, Qt::Orientation orientation, int role) const
		{
			if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
			{
				// Store the CAMP metaclass class
				return QString::fromUtf8(camp::classByType<FireMaterial>().getPropertyByIndex(section).name());
			}

			// We should never ever be here
			return QVariant();
		}

		bool FireMaterialsModel::setData(const QModelIndex& index, const QVariant& value, int role)
		{
			if (index.isValid() && Qt::EditRole == role && !index.parent().isValid())
			{
				const IndexData* indexData = getIndexDataByQModelIndex(index);
				if (nullptr != indexData)
				{
					// Make sure we only get fire materials
					if (static_cast<IndexType>(indexData->indexType) == INDEXTYPE_FIREMATERIAL)
					{
						// It's a fire material
						FireMaterial* fireMaterial = static_cast<FireMaterial*>(indexData->object);

						// Previous value
						const camp::Property& campProperty = fireMaterial->getCampClass().getPropertyByIndex(index.column());
						const std::string previousValue(qsf::CampHelper::campPropertyValueAsString(fireMaterial, campProperty));

						// New value
						std::string newValue = value.toString().toUtf8().constData();
						if (previousValue != newValue)
						{
							// First check if the user changed the name of a material
							const uint32 propertyId = campProperty.id();
							if (propertyId == camp::StringId("Name"))
							{
								newValue = mFireSystem.getFireMaterialManager().findUniqueNameForMaterial(newValue);
							}

							// Set the new value as the property value
							camp::UserObject(fireMaterial).set(propertyId, newValue);

							// Note: We have no undo /redo operation for fire materials

							// Flag fire material manager
							mFireSystem.getFireMaterialManager().setContentHasChanged(true);

							// According to the "QAbstractItemModel::setData()"-documentation we should emit the "dataChanged()"-signal
							Q_EMIT dataChanged(index, index);
							return true;
						}
					}
				}
			}

			// No data set
			return false;
		}

		Qt::ItemFlags FireMaterialsModel::flags(const QModelIndex& index) const
		{
			// Item must be valid
			if (!index.isValid())
				return 0;

			// The IDs should not be editable
			if (index.column() == 0)
				// TODO(ca) Review: Was 'return ~Qt::ItemIsEditable;'
				return Qt::ItemIsSelectable;

			Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

			if (mEditingEnabled)
				flags |= Qt::ItemIsEditable;

			return flags;
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		void FireMaterialsModel::setupModelData(const FireSystem& fireSystem, qsf::editor::TreeItem<IndexData>* parentItem)
		{
			const FireMaterialManager& fireMaterialManager = mFireSystem.getFireMaterialManager();
			for (auto fireMaterialPair : fireMaterialManager.getElements())
			{
				FireMaterial* fireMaterial = fireMaterialPair.second;

				IndexData indexData;
				indexData.object    = fireMaterial;
				indexData.indexType = static_cast<BaseModel::IndexType>(INDEXTYPE_FIREMATERIAL);
				indexData.objectId  = fireMaterial->getFireMaterialId();

				QList<IndexData> dataList;
				dataList << indexData;

				// Add tree item (fire material)
				parentItem->appendChild(*(new qsf::editor::TreeItem<IndexData>(dataList, parentItem)));
			}
		}

		void FireMaterialsModel::onFireMaterialsChanged()
		{
			// The fire materials asset was changed (updated/deleted)
			// -> We simply refresh the model completely
			refresh();
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
