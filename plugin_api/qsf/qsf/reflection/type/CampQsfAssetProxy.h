// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/asset/AssetProxy.h"
#include "qsf/reflection/CampClass.h"

#include <camp/valuemapper.hpp>


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
namespace camp_ext
{


	/**
	*  @brief
	*    Value mapper for qsf::AssetProxy
	*
	*  @note
	*    - This class tells CAMP how to handle qsf::AssetProxy as an user type
	*/
	template <>
	struct QSF_API_EXPORT ValueMapper<qsf::AssetProxy>
	{

		/**
		*  @brief
		*    CAMP type associated to qsf::AssetProxy
		*/
		static const int type = camp::userType;

		//[-------------------------------------------------------]
		//[ To                                                    ]
		//[-------------------------------------------------------]
		static camp::UserObject to(const qsf::AssetProxy& source);

		//[-------------------------------------------------------]
		//[ From                                                  ]
		//[-------------------------------------------------------]
		static qsf::AssetProxy from(bool source)
		{
			// Bool does not make much sense in here, but doesn't hurt either
			return qsf::AssetProxy(static_cast<qsf::GlobalAssetId>(source));
		}

		static qsf::AssetProxy from(long source)
		{
			return qsf::AssetProxy(static_cast<qsf::GlobalAssetId>(source));
		}

		static qsf::AssetProxy from(double source)
		{
			return qsf::AssetProxy(static_cast<qsf::GlobalAssetId>(source));
		}

		static qsf::AssetProxy from(const std::string& source);

		static qsf::AssetProxy from(const camp::EnumObject& source)
		{
			return qsf::AssetProxy(static_cast<qsf::GlobalAssetId>(source.value()));
		}

		static qsf::AssetProxy from(const camp::UserObject& source);

	};


} // camp_ext


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(qsf::AssetProxy)
