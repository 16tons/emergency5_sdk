// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <camp/arraymapper.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline GameLogicComponent::GameLogicComponent(Prototype* prototype) :
		Component(prototype),
		GameLogicArray(*this)
	{
		// Nothing here
	}

	inline GameLogicComponent::~GameLogicComponent()
	{
		// Nothing here
	}

	template <typename T>
	T* GameLogicComponent::createGameLogic()
	{
		return static_cast<T*>(createGameLogicByTypeId(T::GAMELOGIC_TYPE_ID));
	}

	template <typename T>
	T* GameLogicComponent::getGameLogic() const
	{
		return static_cast<T*>(getGameLogicByTypeId(T::GAMELOGIC_TYPE_ID));
	}

	template <typename T>
	T* GameLogicComponent::getOrCreateGameLogic()
	{
		return static_cast<T*>(getOrCreateGameLogicByTypeId(T::GAMELOGIC_TYPE_ID));
	}

	template <typename T>
	T& GameLogicComponent::getOrCreateGameLogicSafe()
	{
		T* logic = getOrCreateGameLogic<T>();
		QSF_CHECK(nullptr != logic, "Could not create an instance of logic with type ID " << T::GAMELOGIC_TYPE_ID << " in prototype " << getId(), QSF_REACT_THROW);
		return *logic;
	}

	template <typename T>
	const std::vector<T*>& GameLogicComponent::getAllGameLogics()
	{
		return reinterpret_cast<const std::vector<T*>&>(getAllGameLogics(T::GAMELOGIC_TYPE_ID));
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	inline std::size_t GameLogicComponent::GameLogicArrayInterface::size() const
	{
		return getOwner().mGameLogicConfigArray.size();
	}

	inline const GameLogicComponent::GameLogicConfig& GameLogicComponent::GameLogicArrayInterface::get(std::size_t index) const
	{
		return getOwner().mGameLogicConfigArray[index];
	}

	inline void GameLogicComponent::GameLogicArrayInterface::set(std::size_t index, const GameLogicConfig& value)
	{
		getOwner().mGameLogicConfigArray[index] = value;
	}

	inline void GameLogicComponent::GameLogicArrayInterface::insert(std::size_t before, const GameLogicConfig& value)
	{
		getOwner().mGameLogicConfigArray.insert(getOwner().mGameLogicConfigArray.begin() + before, value);
	}

	inline void GameLogicComponent::GameLogicArrayInterface::remove(std::size_t index)
	{
		getOwner().mGameLogicConfigArray.erase(getOwner().mGameLogicConfigArray.begin() + index);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
