// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/math/Color3.h"
#include "qsf/reflection/CampClass.h"

#include <camp/valuemapper.hpp>


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
namespace camp_ext
{


	/**
	*  @brief
	*    Value mapper for qsf::Color3
	*
	*  @note
	*    - This class tells CAMP how to handle qsf::Color3 as an user type
	*/
	template <>
	struct QSF_API_EXPORT ValueMapper<qsf::Color3>
	{

		/**
		*  @brief
		*    CAMP type associated to qsf::Color3
		*/
		static const int type = camp::userType;

		//[-------------------------------------------------------]
		//[ To                                                    ]
		//[-------------------------------------------------------]
		static camp::UserObject to(const qsf::Color3& source);

		//[-------------------------------------------------------]
		//[ From                                                  ]
		//[-------------------------------------------------------]
		static qsf::Color3 from(bool source)
		{
			return qsf::Color3(static_cast<float>(source), static_cast<float>(source), static_cast<float>(source));
		}

		static qsf::Color3 from(long source)
		{
			return qsf::Color3(static_cast<float>(source), static_cast<float>(source), static_cast<float>(source));
		}

		static qsf::Color3 from(double source)
		{
			return qsf::Color3(static_cast<float>(source), static_cast<float>(source), static_cast<float>(source));
		}

		static qsf::Color3 from(const std::string& source);

		static qsf::Color3 from(const camp::EnumObject& source)
		{
			return qsf::Color3(static_cast<float>(source.value()), static_cast<float>(source.value()), static_cast<float>(source.value()));
		}

		static qsf::Color3 from(const camp::UserObject& source);

	};


} // camp_ext


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(qsf::Color3)
