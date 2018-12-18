// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	template<int N, typename T>
	inline bool BinarySerializer::isVersionSupported(const T version, const T (&supportedVersions)[N])
	{
		bool result = false;

		for (size_t i = 0; i < N; ++i)
		{
			if (version == supportedVersions[i])
			{
				result = true;
				break;
			}
		}

		return result;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline bool BinarySerializer::isReading() const
	{
		return (nullptr != mInputStream);
	}

	inline bool BinarySerializer::isWriting() const
	{
		return (nullptr != mOutputStream);
	}

	template<typename T>
	void BinarySerializer::serialize(T& value, const char* name)
	{
		// Serialize tokens
		if (mTokenMode != TOKEN_FLAG_NONE)
		{
			// Note that the token must be platform independent
			tokenSerialization(serialization::getToken<T>(), name);
		}

		// Serialize the variable itself
		serialization::serializer<T>::serialize(*this, value);
	}

	template<typename T>
	BinarySerializer& BinarySerializer::operator&(T& value)
	{
		serialize(value);
		return *this;
	}

	template<typename TARGETTYPE, typename ORIGINALTYPE>
	void BinarySerializer::serializeAs(ORIGINALTYPE& value, const char* name)
	{
		if (nullptr != mInputStream)
		{
			// Read value
			TARGETTYPE targetTypeValue;
			serialize(targetTypeValue, name);
			value = static_cast<ORIGINALTYPE>(targetTypeValue);
		}
		else if (nullptr != mOutputStream)
		{
			// Write value
			TARGETTYPE targetTypeValue = static_cast<TARGETTYPE>(value);
			serialize(targetTypeValue, name);
		}
		else
		{
			QSF_ERROR("Undefined serialization mode, this should not happen.", QSF_REACT_THROW);
		}
	}

	template<typename T>
	T BinarySerializer::read(const char* name)
	{
		QSF_ASSERT(isReading(), "Don't call 'BinarySerializer::read' if you are not in reading mode", QSF_REACT_THROW);
		T value;
		serialize(value, name);
		return value;
	}

	template<typename T>
	void BinarySerializer::read(T& value, const char* name)
	{
		QSF_ASSERT(isReading(), "Don't call 'BinarySerializer::read' if you are not in reading mode", QSF_REACT_THROW);
		serialize(value, name);
	}

	template<typename T>
	void BinarySerializer::write(const T& value, const char* name)
	{
		QSF_ASSERT(isWriting(), "Don't call 'BinarySerializer::write' if you are not in writing mode", QSF_REACT_THROW);
		serialize(const_cast<T&>(value), name);
	}

	template<typename TARGETTYPE, typename ORIGINALTYPE>
	void BinarySerializer::writeAs(const ORIGINALTYPE& value, const char* name)
	{
		QSF_ASSERT(isWriting(), "Don't call 'BinarySerializer::writeAs' if you are not in writing mode", QSF_REACT_THROW);
		TARGETTYPE targetTypeValue = static_cast<TARGETTYPE>(value);
		serialize(targetTypeValue, name);
	}

	inline const std::string& BinarySerializer::getFormatType() const
	{
		return mFormatType;
	}

	inline uint16 BinarySerializer::getFormatVersion() const
	{
		return mFormatVersion;
	}

	inline uint8 BinarySerializer::getInternalVersion() const
	{
		return mInternalVersion;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
