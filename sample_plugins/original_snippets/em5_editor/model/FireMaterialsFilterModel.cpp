// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/model/FireMaterialsFilterModel.h"
#include "em5_editor/model/FireMaterialsModel.h"

#include <em5/fire/FireSystem.h>
#include <em5/fire/FireMaterialManager.h>

#include <qsf_editor/settings/EditorSettingsGroup.h>

#include <qsf/reflection/CampHelper.h>
#include <qsf/reflection/ClassSystem.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Protected virtual QSortFilterProxyModel methods       ]
		//[-------------------------------------------------------]
		bool FireMaterialsFilterModel::filterAcceptsColumn(int sourceColumn, const QModelIndex& sourceParent) const
		{
			// The main task of this function is to filter the internal ID's. The ID's have the "hidden"-tag

			// Ask the settings for filtering hidden values, do this once per function call
			if (!qsf::editor::EditorSettingsGroup::getInstanceSafe().getShowHiddenComponents())
			{
				// We need to take the sourceModel() for checking
				const QModelIndex index = sourceModel()->index(0, sourceColumn, sourceParent);

				// Is it a fire material
				qsf::editor::BaseModel::IndexData* indexData = static_cast<qsf::editor::BaseModel*>(sourceModel())->getIndexDataByQModelIndex(index);
				if (nullptr != indexData && static_cast<FireMaterialsModel::IndexType>(indexData->indexType) == FireMaterialsModel::INDEXTYPE_FIREMATERIAL)
				{
					const FireMaterial* fireMaterial = static_cast<FireMaterial*>(indexData->object);
					if (nullptr != fireMaterial)
					{
						// Get the property and check if whether it has the hidden tag or not
						const camp::Property& campProperty = fireMaterial->getCampClass().getPropertyByIndex(index.column());
						const camp::Value* campValue = campProperty.tryGetTagById("Hidden");
						if (nullptr != campValue && campValue->to<bool>())
						{
							// Property is hidden, don't show it
							return false;
						}

						// No hidden-tag, do not filter
						return true;
					}
				}
			}

			// Do not filter
			return true;
		}

		bool FireMaterialsFilterModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
		{
			// Both are fire materials, check names
			const QVariant leftData = sourceModel()->data(left);
			if (leftData.type() == QVariant::Double)
			{
				const QVariant rightData = sourceModel()->data(right);
				const double leftValue = leftData.toDouble();
				const double rightValue = rightData.toDouble();
				return (leftValue < rightValue);
			}
			else if (leftData.type() == QVariant::String)
			{
				const QVariant rightData = sourceModel()->data(right);
				const QString leftString = leftData.toString();
				const QString rightString = rightData.toString();
				return (QString::localeAwareCompare(leftString, rightString) > 0);
			}
			return true;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
