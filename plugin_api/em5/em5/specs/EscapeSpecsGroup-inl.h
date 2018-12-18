// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline EscapeSpecsGroup::EscapeSpecsGroup(SpecsGroupManager* specsGroupManager) :
		SpecsGroup(specsGroupManager),
		mEscapeTimeFootLong(45.f),
		mEscapeTimeCarLong(45.f),
		mRunTimeFootLongMin(15.f),
		mRunTimeFootLongMax(25.f),
		mRunTimeFootShortMin(15.f),
		mRunTimeFootShortMax(25.f),
		mBreatheTimeFootLongMin(5.f),
		mBreatheTimeFootLongMax(10.f),
		mBreatheTimeFootShortMin(5.f),
		mBreatheTimeFootShortMax(10.f),
		mHelicopterFollowTime(10.f)
	{
		// Nothing to do in here
	}

	inline EscapeSpecsGroup::~EscapeSpecsGroup()
	{
		// Nothing to do in here
	}

	inline float EscapeSpecsGroup::getEscapeTimeFootLong() const
	{
		return mEscapeTimeFootLong;
	}

	inline float EscapeSpecsGroup::getEscapeTimeCarLong() const
	{
		return mEscapeTimeCarLong;
	}

	inline float EscapeSpecsGroup::getRunTimeFootLongMin() const
	{
		return mRunTimeFootLongMin;
	}

	inline float EscapeSpecsGroup::getRunTimeFootLongMax() const
	{
		return mRunTimeFootLongMax;
	}

	inline float EscapeSpecsGroup::getRunTimeFootShortMin() const
	{
		return mRunTimeFootShortMin;
	}

	inline float EscapeSpecsGroup::getRunTimeFootShortMax() const
	{
		return mRunTimeFootShortMax;
	}

	inline float EscapeSpecsGroup::getBreatheTimeFootLongMin() const
	{
		return mBreatheTimeFootLongMin;
	}

	inline float EscapeSpecsGroup::getBreatheTimeFootLongMax() const
	{
		return mBreatheTimeFootLongMax;
	}

	inline float EscapeSpecsGroup::getBreatheTimeFootShortMin() const
	{
		return mBreatheTimeFootShortMin;
	}

	inline float EscapeSpecsGroup::getBreatheTimeFootShortMax() const
	{
		return mBreatheTimeFootShortMax;
	}

	inline float EscapeSpecsGroup::getHelicopterFollowTime() const
	{
		return mHelicopterFollowTime;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
