// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/serialization/binary/BasicTypeSerialization.h"
#include "qsf/serialization/binary/StlTypeSerialization.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline NamedIdentifier::NamedIdentifier() :
		mHash(getUninitialized<uint32>())
	{
		// Nothing do to in here
	}

	inline NamedIdentifier::NamedIdentifier(const std::string& name) :
		mName(name),
		mHash(mName.empty() ? getUninitialized<uint32>() : StringHash::calculateFNV(mName.c_str()))
	{
		// Nothing do to in here
	}

	inline NamedIdentifier::NamedIdentifier(const char* name) :
		mName(name),
		mHash((name[0] == 0) ? getUninitialized<uint32>() : StringHash::calculateFNV(name))
	{
		// Nothing do to in here
	}

	inline NamedIdentifier::~NamedIdentifier()
	{
		// Nothing do to in here
	}

	inline bool NamedIdentifier::empty() const
	{
		return mName.empty();
	}

	inline const std::string& NamedIdentifier::getName() const
	{
		return mName;
	}

	inline uint32 NamedIdentifier::getHash() const
	{
		return mHash;
	}

	inline NamedIdentifier::operator const std::string&() const
	{
		return mName;
	}

	inline NamedIdentifier::operator uint32() const
	{
		return mHash;
	}

	inline NamedIdentifier::operator StringHash() const
	{
		return StringHash(mHash);
	}

	inline bool NamedIdentifier::operator ==(const NamedIdentifier& other) const
	{
		return (mHash == other.getHash());
	}

	inline bool NamedIdentifier::operator ==(uint32 other) const
	{
		return (mHash == other);
	}

	inline bool NamedIdentifier::operator ==(const std::string& other) const
	{
		return (mName == other);
	}

	inline void NamedIdentifier::reset()
	{
		mName.clear();
		setUninitialized(mHash);
	}


	// Specialization for uninitialized values
	template <>
	inline NamedIdentifier getUninitialized<NamedIdentifier>()
	{
		return NamedIdentifier();
	}


	//[-------------------------------------------------------]
	//[ Operators                                             ]
	//[-------------------------------------------------------]
	inline std::string operator +(const char* lhs, const NamedIdentifier& rhs)
	{
		return lhs + rhs.getName();
	}

	inline std::string operator +(const NamedIdentifier& lhs, const char* rhs)
	{
		return lhs.getName() + rhs;
	}


	//[-------------------------------------------------------]
	//[ Serialization                                         ]
	//[-------------------------------------------------------]
	namespace serialization
	{
		template <>
		struct serializer<NamedIdentifier>
		{
			inline static void serialize(BinarySerializer& serializer, NamedIdentifier& data)
			{
				std::string text = data.getName();
				serializer & text;
				if (serializer.isReading())
				{
					data = NamedIdentifier(text);
				}
			}
		};
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
