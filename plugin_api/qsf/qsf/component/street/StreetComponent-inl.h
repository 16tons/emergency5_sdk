// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline StreetComponent::StreetComponent(Prototype* prototype) :
		NodesComponent(prototype),
		mAiNavigationMapId(0), // Zero is a sensible default here as it allows creating the street network as zero without setting too many values for every street section or crossing
		StreetGateways(*this)
	{
		// Nothing here
	}

	inline StreetComponent::~StreetComponent()
	{
		// Nothing here
	}

	inline const std::vector<StreetGateway>& StreetComponent::getStreetGateways() const
	{
		return mStreetGateways;
	}

	inline StreetGateway* StreetComponent::getStreetGatewayByIndex(uint32 streetGatewayIndex)
	{
		// No assert in here, please
		return (streetGatewayIndex < mStreetGateways.size()) ? &mStreetGateways[streetGatewayIndex] : nullptr;
	}

	inline const StreetGateway* StreetComponent::getStreetGatewayByIndex(size_t streetGatewayIndex) const
	{
		// No assert in here, please
		return (streetGatewayIndex < mStreetGateways.size()) ? &mStreetGateways[streetGatewayIndex] : nullptr;
	}

	inline uint32 StreetComponent::getAiNavigationMapId() const
	{
		return mAiNavigationMapId;
	}

	inline void StreetComponent::setAiNavigationMapId(uint32 id)
	{
		assignAndPromoteChange(mAiNavigationMapId, id, AI_NAVIGATION_MAP_ID);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::NodesComponent methods         ]
	//[-------------------------------------------------------]
	inline bool StreetComponent::closedContinuousLines() const
	{
		return false;
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	// Gateways interface
	inline std::size_t StreetComponent::StreetGatewaysInterface::size() const
	{
		return getOwner().mStreetGateways.size();
	}

	inline const StreetGateway& StreetComponent::StreetGatewaysInterface::get(std::size_t index) const
	{
		return getOwner().mStreetGateways.at(index);
	}

	inline void StreetComponent::StreetGatewaysInterface::set(std::size_t index, const StreetGateway& value)
	{
		getOwner().mStreetGateways.at(index) = value;

		getOwner().promotePropertyChange(STREET_GATEWAYS);
	}

	inline void StreetComponent::StreetGatewaysInterface::insert(std::size_t before, const StreetGateway& value)
	{
		getOwner().mStreetGateways.insert(getOwner().mStreetGateways.begin() + before, value);

		getOwner().promotePropertyChange(STREET_GATEWAYS);
	}

	inline void StreetComponent::StreetGatewaysInterface::remove(std::size_t index)
	{
		getOwner().mStreetGateways.erase(getOwner().mStreetGateways.begin() + index);

		getOwner().promotePropertyChange(STREET_GATEWAYS);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
