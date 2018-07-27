// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline WeatherComponent::WeatherComponent(qsf::Prototype* prototype) :
		qsf::game::WeatherComponent(prototype),
		mUseUmbrellas(false),
		mLastUseUmbrellaValue(0.f),
		mUmbrellaUpdateTimer(qsf::Time::ZERO),
		mCheckUmbrellaChanges(false)
	{
		// Nothing to do
	}

	inline WeatherComponent::~WeatherComponent()
	{
		// Nothing to do in here
	}

	inline bool WeatherComponent::getUseUmbrellas() const
	{
		return mUseUmbrellas;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
