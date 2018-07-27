// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/reflection/CampClass.h>
#include <qsf/platform/PlatformTypes.h>
#include <qsf/base/GetUninitialized.h>

#include <camp/valuemapper.hpp>


//[-------------------------------------------------------]
//[ Type definition                                       ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{

		// CommandIdentifier is a simple container for a uint32
		// -> We can't use a typedef here because this wouldn't allow us to define a value mapper separate from the uint32 value mapper
		struct CommandIdentifier
		{
			uint32 mId;

			CommandIdentifier() : mId(qsf::getUninitialized<uint32>()) {}
			CommandIdentifier(uint32 id) : mId(id) {}
			operator uint32() { return mId; }
		};

	}
}


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
namespace camp_ext
{


	/**
	*  @brief
	*    Value mapper for qsf::game::CommandIdentifier
	*
	*  @note
	*    - This class tells CAMP how to handle qsf::game::CommandIdentifier as a user type
	*/
	template <>
	struct ValueMapper<qsf::game::CommandIdentifier>
	{

		/**
		*  @brief
		*    CAMP type associated to qsf::game::CommandIdentifier
		*/
		static const int type = camp::stringType;

		//[-------------------------------------------------------]
		//[ To                                                    ]
		//[-------------------------------------------------------]
		static std::string to(const qsf::game::CommandIdentifier& source);

		//[-------------------------------------------------------]
		//[ From                                                  ]
		//[-------------------------------------------------------]
		static qsf::game::CommandIdentifier from(bool source)
		{
			return qsf::game::CommandIdentifier(static_cast<uint32>(source));
		}

		static qsf::game::CommandIdentifier from(long source)
		{
			return qsf::game::CommandIdentifier(static_cast<uint32>(source));
		}

		static qsf::game::CommandIdentifier from(double source)
		{
			return qsf::game::CommandIdentifier(static_cast<uint32>(source));
		}

		static qsf::game::CommandIdentifier from(const std::string& source);

		static qsf::game::CommandIdentifier from(const camp::EnumObject& source)
		{
			return qsf::game::CommandIdentifier(static_cast<uint32>(source.value()));
		}

		static qsf::game::CommandIdentifier from(const camp::UserObject& source);

	};


} // camp_ext


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(qsf::game::CommandIdentifier)
