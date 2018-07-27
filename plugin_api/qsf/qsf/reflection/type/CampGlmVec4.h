// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/reflection/CampClass.h"

#include <glm/glm.hpp>

#include <camp/valuemapper.hpp>


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
namespace camp_ext
{


	/**
	*  @brief
	*    Value mapper for glm::vec4
	*
	*  @note
	*    - This class tells CAMP how to handle glm::vec4 as an user type
	*/
	template <>
	struct QSF_API_EXPORT ValueMapper<glm::vec4>
	{

		/**
		*  @brief
		*    CAMP type associated to glm::vec4
		*/
		static const int type = camp::userType;

		//[-------------------------------------------------------]
		//[ To                                                    ]
		//[-------------------------------------------------------]
		static camp::UserObject to(const glm::vec4& source);

		//[-------------------------------------------------------]
		//[ From                                                  ]
		//[-------------------------------------------------------]
		static glm::vec4 from(bool source)
		{
			return glm::vec4(static_cast<float>(source), static_cast<float>(source), static_cast<float>(source), static_cast<float>(source));
		}

		static glm::vec4 from(long source)
		{
			return glm::vec4(static_cast<float>(source), static_cast<float>(source), static_cast<float>(source), static_cast<float>(source));
		}

		static glm::vec4 from(double source)
		{
			return glm::vec4(static_cast<float>(source), static_cast<float>(source), static_cast<float>(source), static_cast<float>(source));
		}

		static glm::vec4 from(const std::string& source);

		static glm::vec4 from(const camp::EnumObject& source)
		{
			return glm::vec4(static_cast<float>(source.value()), static_cast<float>(source.value()), static_cast<float>(source.value()), static_cast<float>(source.value()));
		}

		static glm::vec4 from(const camp::UserObject& source);

	};


} // camp_ext


// Support for glm::vec4 output in std::ostream
inline std::ostream& operator<<(std::ostream& ostream, const glm::vec4& vec)
{
	ostream << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
	return ostream;
}


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(glm::vec4)
