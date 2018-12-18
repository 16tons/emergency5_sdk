// Copyright (C) 2012-2018 Promotion Software GmbH


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
	*    Value mapper for glm::ivec2
	*
	*  @note
	*    - This class tells CAMP how to handle glm::ivec2 as an user type
	*/
	template <>
	struct QSF_API_EXPORT ValueMapper<glm::ivec2>
	{

		/**
		*  @brief
		*    CAMP type associated to glm::ivec2
		*/
		static const int type = camp::userType;

		//[-------------------------------------------------------]
		//[ To                                                    ]
		//[-------------------------------------------------------]
		static camp::UserObject to(const glm::ivec2& source);

		//[-------------------------------------------------------]
		//[ From                                                  ]
		//[-------------------------------------------------------]
		static glm::ivec2 from(bool source)
		{
			return glm::ivec2(static_cast<int>(source), static_cast<int>(source));
		}

		static glm::ivec2 from(long source)
		{
			return glm::ivec2(static_cast<int>(source), static_cast<int>(source));
		}

		static glm::ivec2 from(double source)
		{
			return glm::ivec2(static_cast<int>(source), static_cast<int>(source));
		}

		static glm::ivec2 from(const std::string& source);

		static glm::ivec2 from(const camp::EnumObject& source)
		{
			return glm::ivec2(static_cast<int>(source.value()), static_cast<int>(source.value()));
		}

		static glm::ivec2 from(const camp::UserObject& source);

	};


} // camp_ext


// Support for glm::ivec2 output in std::ostream
inline std::ostream& operator<<(std::ostream& ostream, const glm::ivec2& vec)
{
	ostream << vec.x << ", " << vec.y;
	return ostream;
}


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(glm::ivec2)
