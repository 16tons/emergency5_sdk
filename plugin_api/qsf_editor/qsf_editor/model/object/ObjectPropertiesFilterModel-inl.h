// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline ObjectPropertiesFilterModel::ObjectPropertiesFilterModel(QObject* parent) :
			QSortFilterProxyModel(parent),
			mSettingsAllowFilteringHiddenTag(true)
		{
			// Nothing to do in here
		}

		inline ObjectPropertiesFilterModel::~ObjectPropertiesFilterModel()
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
