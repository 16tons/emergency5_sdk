// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/math/Color4.h"
#include "qsf/reflection/CampClass.h"

#include <camp/valuemapper.hpp>


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
namespace camp_ext
{


	/**
	*  @brief
	*    Value mapper for qsf::Color4
	*
	*  @note
	*    - This class tells CAMP how to handle qsf::Color4 as an user type
	*/
	template <>
	struct QSF_API_EXPORT ValueMapper<qsf::Color4>
	{

		/**
		*  @brief
		*    CAMP type associated to qsf::Color4
		*/
		static const int type = camp::userType;

		//[-------------------------------------------------------]
		//[ To                                                    ]
		//[-------------------------------------------------------]
		static camp::UserObject to(const qsf::Color4& source);

		//[-------------------------------------------------------]
		//[ From                                                  ]
		//[-------------------------------------------------------]
		static qsf::Color4 from(bool source)
		{
			return qsf::Color4(static_cast<float>(source), static_cast<float>(source), static_cast<float>(source), 1.0f);
		}

		static qsf::Color4 from(long source)
		{
			return qsf::Color4(static_cast<float>(source), static_cast<float>(source), static_cast<float>(source), 1.0f);
		}

		static qsf::Color4 from(double source)
		{
			return qsf::Color4(static_cast<float>(source), static_cast<float>(source), static_cast<float>(source), 1.0f);
		}

		static qsf::Color4 from(const std::string& source);

		static qsf::Color4 from(const camp::EnumObject& source)
		{
			return qsf::Color4(static_cast<float>(source.value()), static_cast<float>(source.value()), static_cast<float>(source.value()), 1.0f);
		}

		static qsf::Color4 from(const camp::UserObject& source);

	};


} // camp_ext


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(qsf::Color4)
