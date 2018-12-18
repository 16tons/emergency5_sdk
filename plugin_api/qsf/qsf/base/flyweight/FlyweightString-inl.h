// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline FlyweightString::FlyweightString() :
		mEntry(&mStaticEmptyEntry)
	{
		++mEntry->mReferenceCounter;
	}

	inline FlyweightString::FlyweightString(const char* string) :
		mEntry(nullptr)
	{
		registerEntry(string);
	}

	inline FlyweightString::FlyweightString(const std::string& string) :
		mEntry(nullptr)
	{
		registerEntry(string);
	}

	inline FlyweightString::FlyweightString(const NamedIdentifier& string) :
		mEntry(nullptr)
	{
		registerEntry(string);
	}

	inline FlyweightString::FlyweightString(const FlyweightString& string) :
		mEntry(string.mEntry)
	{
		++mEntry->mReferenceCounter;
	}

	inline FlyweightString::~FlyweightString()
	{
		unregisterEntry();
	}

	inline FlyweightString& FlyweightString::operator=(const char* string)
	{
		return operator=(NamedIdentifier(string));
	}

	inline FlyweightString& FlyweightString::operator=(const std::string& string)
	{
		return operator=(NamedIdentifier(string));
	}

	inline FlyweightString& FlyweightString::operator=(const NamedIdentifier& string)
	{
		if (string != mEntry->mString)
		{
			unregisterEntry();
			registerEntry(string);
		}
		return *this;
	}

	inline FlyweightString& FlyweightString::operator=(const FlyweightString& string)
	{
		if (string != *this)
		{
			unregisterEntry();
			registerEntry(*string.mEntry);
		}
		return *this;
	}

	inline bool FlyweightString::operator==(const FlyweightString& string) const
	{
		return (&mEntry == &string.mEntry);
	}

	inline bool FlyweightString::operator!=(const FlyweightString& string) const
	{
		return (&mEntry != &string.mEntry);
	}

	inline FlyweightString::operator const std::string&() const
	{
		return mEntry->mString.getName();
	}

	inline FlyweightString::operator const NamedIdentifier&() const
	{
		return mEntry->mString;
	}

	inline bool FlyweightString::empty() const
	{
		return (mEntry == &mStaticEmptyEntry);
	}

	inline uint32 FlyweightString::getHash() const
	{
		return mEntry->mString.getHash();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline void FlyweightString::registerEntry(Entry& entry)
	{
		mEntry = &entry;
		++entry.mReferenceCounter;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
