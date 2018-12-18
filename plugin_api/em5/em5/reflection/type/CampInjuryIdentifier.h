// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/reflection/CampClass.h>
#include <qsf/base/GetUninitialized.h>

#include <camp/valuemapper.hpp>


//[-------------------------------------------------------]
//[ Type definition                                       ]
//[-------------------------------------------------------]
namespace em5
{

	// InjuryIdentifier is a simple container for an uint32
	// -> We can't use a typedef here because this wouldn't allow us to define a value mapper separate from the uint32 value mappe
	struct InjuryIdentifier
	{
		uint32 mId;

		InjuryIdentifier() : mId(qsf::getUninitialized<uint32>()) {}
		InjuryIdentifier(uint32 id) : mId(id) {}
		operator uint32() { return mId; }
	};

}


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
namespace camp_ext
{


	/**
	*  @brief
	*    Value mapper for em5::InjuryIdentifier
	*
	*  @note
	*    - This class tells CAMP how to handle em5::InjuryIdentifier as a user type
	*/
	template <>
	struct ValueMapper<em5::InjuryIdentifier>
	{

		/**
		*  @brief
		*    CAMP type associated to em5::InjuryIdentifier
		*/
		static const int type = camp::stringType;

		//[-------------------------------------------------------]
		//[ To                                                    ]
		//[-------------------------------------------------------]
		static std::string to(const em5::InjuryIdentifier& source);

		//[-------------------------------------------------------]
		//[ From                                                  ]
		//[-------------------------------------------------------]
		static em5::InjuryIdentifier from(bool source)
		{
			return em5::InjuryIdentifier(static_cast<uint32>(source));
		}

		static em5::InjuryIdentifier from(long source)
		{
			return em5::InjuryIdentifier(static_cast<uint32>(source));
		}

		static em5::InjuryIdentifier from(double source)
		{
			return em5::InjuryIdentifier(static_cast<uint32>(source));
		}

		static em5::InjuryIdentifier from(const std::string& source);

		static em5::InjuryIdentifier from(const camp::EnumObject& source)
		{
			return em5::InjuryIdentifier(static_cast<uint32>(source.value()));
		}

		static em5::InjuryIdentifier from(const camp::UserObject& source);

	};


} // camp_ext


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(em5::InjuryIdentifier)
