// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline GameCounter::GameCounter(const std::string& name) :
			mName(name),
			mValue(0)
		{
			// Nothing here
		}

		inline GameCounter::~GameCounter()
		{
			// Nothing here
		}

		inline const std::string& GameCounter::getName() const
		{
			return mName.getName();
		}

		inline uint32 GameCounter::getId() const
		{
			return mName.getHash();
		}

		inline void GameCounter::reset()
		{
			setValue(0);
		}

		inline int32 GameCounter::getValue() const
		{
			return mValue;
		}

		inline int32 GameCounter::operator++()
		{
			setValue(mValue + 1);
			return mValue;
		}

		inline int32 GameCounter::operator--()
		{
			setValue(mValue - 1);
			return mValue;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
