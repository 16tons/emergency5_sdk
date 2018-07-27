// Copyright (C) 2012-2017 Promotion Software GmbH


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
		inline TrafficLightComponent::~TrafficLightComponent()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		inline std::size_t TrafficLightComponent::EntityIdArrayInterface::size() const
		{
			return getVariable().size();
		}

		inline const uint64& TrafficLightComponent::EntityIdArrayInterface::get(std::size_t index) const
		{
			return getVariable()[index];
		}

		inline void TrafficLightComponent::EntityIdArrayInterface::set(std::size_t index, const uint64& value)
		{
			getVariable()[index] = value;
		}

		inline void TrafficLightComponent::EntityIdArrayInterface::insert(std::size_t before, const uint64& value)
		{
			getVariable().insert(getVariable().begin() + before, value);
		}

		inline void TrafficLightComponent::EntityIdArrayInterface::remove(std::size_t index)
		{
			getVariable().erase(getVariable().begin() + index);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
