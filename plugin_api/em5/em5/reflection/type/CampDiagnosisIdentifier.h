// Copyright (C) 2012-2017 Promotion Software GmbH


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

	// DiagnosisIdentifier is a simple container for an uint32
	// -> We can't use a typedef here because this wouldn't allow us to define a value mapper separate from the uint32 value mapper
	struct DiagnosisIdentifier
	{
		uint32 mId;

		DiagnosisIdentifier() : mId(qsf::getUninitialized<uint32>()) {}
		explicit DiagnosisIdentifier(uint32 id) : mId(id) {}
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
	*    Value mapper for em5::DiagnosisIdentifier
	*
	*  @note
	*    - This class tells CAMP how to handle em5::DiagnosisIdentifier as a user type
	*/
	template <>
	struct ValueMapper<em5::DiagnosisIdentifier>
	{

		/**
		*  @brief
		*    CAMP type associated to em5::DiagnosisIdentifier
		*/
		static const int type = camp::stringType;

		//[-------------------------------------------------------]
		//[ To                                                    ]
		//[-------------------------------------------------------]
		static std::string to(const em5::DiagnosisIdentifier& source);

		//[-------------------------------------------------------]
		//[ From                                                  ]
		//[-------------------------------------------------------]
		static em5::DiagnosisIdentifier from(bool source)
		{
			return em5::DiagnosisIdentifier(static_cast<uint32>(source));
		}

		static em5::DiagnosisIdentifier from(long source)
		{
			return em5::DiagnosisIdentifier(static_cast<uint32>(source));
		}

		static em5::DiagnosisIdentifier from(double source)
		{
			return em5::DiagnosisIdentifier(static_cast<uint32>(source));
		}

		static em5::DiagnosisIdentifier from(const std::string& source);

		static em5::DiagnosisIdentifier from(const camp::EnumObject& source)
		{
			return em5::DiagnosisIdentifier(static_cast<uint32>(source.value()));
		}

		static em5::DiagnosisIdentifier from(const camp::UserObject& source);

	};


} // camp_ext


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(em5::DiagnosisIdentifier)
