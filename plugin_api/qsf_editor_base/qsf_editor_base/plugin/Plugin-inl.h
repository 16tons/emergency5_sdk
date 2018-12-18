// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{


			//[-------------------------------------------------------]
			//[ Public virtual qsf::Plugin methods                    ]
			//[-------------------------------------------------------]
			inline const char* Plugin::getName() const
			{
				return "QSF Editor Base";
			}

			inline const char* Plugin::getVendor() const
			{
				return "Promotion Software GmbH";
			}

			inline const char* Plugin::getDescription() const
			{
				return "Quadriga Simulation Framework (QSF) editor base, contains functionality which is shared between the QSF editor client and the QSF editor server";
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
