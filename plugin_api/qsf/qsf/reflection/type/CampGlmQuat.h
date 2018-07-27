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

#include <glm/gtc/quaternion.hpp>

#include <camp/valuemapper.hpp>


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
namespace camp_ext
{


	/**
	*  @brief
	*    Value mapper for glm::quat
	*
	*  @note
	*    - This class tells CAMP how to handle glm::quat as an user type
	*/
	template <>
	struct QSF_API_EXPORT ValueMapper<glm::quat>
	{

		/**
		*  @brief
		*    CAMP type associated to glm::quat
		*/
		static const int type = camp::userType;

		//[-------------------------------------------------------]
		//[ To                                                    ]
		//[-------------------------------------------------------]
		static camp::UserObject to(const glm::quat& source);

		//[-------------------------------------------------------]
		//[ From                                                  ]
		//[-------------------------------------------------------]
		static glm::quat from(bool)
		{
			// TODO(co) This conversion does not make any sense
			return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		}

		static glm::quat from(long)
		{
			// TODO(co) This conversion does not make any sense
			return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		}

		static glm::quat from(double)
		{
			// TODO(co) This conversion does not make any sense
			return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		}

		static glm::quat from(const std::string& source);

		static glm::quat from(const camp::EnumObject&)
		{
			// TODO(co) This conversion does not make any sense
			return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		}

		static glm::quat from(const camp::UserObject& source);

	};


} // camp_ext


// Support for glm::quat output in std::ostream
inline std::ostream& operator<<(std::ostream& ostream, const glm::quat& quat)
{
	// In glm::quat constructor, w is first, so we output this one a first component, not last
	ostream << quat.w << ", " << quat.x << ", " << quat.y << ", " << quat.z;
	return ostream;
}


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(glm::quat)
