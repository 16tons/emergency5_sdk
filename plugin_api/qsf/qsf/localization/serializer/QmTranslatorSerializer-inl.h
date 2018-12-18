// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline QmTranslatorSerializer::~QmTranslatorSerializer()
	{
		// Nothing to do in here
	}

	template<typename T>
	inline void QmTranslatorSerializer::adjustByteorder(T& value) const
	{
		if (mIsLittleEndian)
		{
			endianconversion::ByteOrder<T>::Reverse(value);
		}
	}

	template<typename T>
	inline T QmTranslatorSerializer::read(std::istream& istream) const
	{
		T value = getUninitialized<T>();
		istream.read(reinterpret_cast<char*>(&value), sizeof(T));
		adjustByteorder(value);
		return value;
	}

	template<typename T>
	inline T QmTranslatorSerializer::read(const char*& readPtr) const
	{
		T value = *reinterpret_cast<const T*>(readPtr);
		readPtr += sizeof(T);
		adjustByteorder(value);
		return value;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
