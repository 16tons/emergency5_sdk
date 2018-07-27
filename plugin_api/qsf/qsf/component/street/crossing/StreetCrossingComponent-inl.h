// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline StreetCrossingComponent::~StreetCrossingComponent()
	{
		// Nothing to do in here
	}

	inline uint32 StreetCrossingComponent::getNumberOfStreetEdges() const
	{
		return static_cast<uint32>(mStreetEdges.size());
	}

	inline const std::vector<StreetEdge>& StreetCrossingComponent::getEdges() const
	{
		return mStreetEdges;
	}

	inline void StreetCrossingComponent::setStreetGateways(const std::vector<StreetGateway>& streetGateways)
	{
		mStreetGateways = streetGateways;

		promotePropertyChange(STREET_GATEWAYS);
	}

	inline void StreetCrossingComponent::repaintDebugDrawing()
	{
		startDebugDrawing();
	}

	inline uint32 StreetCrossingComponent::getBridgeId() const
	{
		return mBridgeId;
	}

	inline void StreetCrossingComponent::setBridgeId(uint32 bridgeId)
	{
		mBridgeId = bridgeId;
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	// Edges interface
	inline std::size_t StreetCrossingComponent::EdgesInterface::size() const
	{
		return getOwner().getEdges().size();
	}

	inline const StreetEdge& StreetCrossingComponent::EdgesInterface::get(std::size_t index) const
	{
		return getOwner().getEdges().at(index);
	}

	inline void StreetCrossingComponent::EdgesInterface::set(std::size_t index, const StreetEdge& value)
	{
		getOwner().mStreetEdges.at(index) = value;

		getOwner().promotePropertyChange(EDGES);
	}

	inline void StreetCrossingComponent::EdgesInterface::insert(std::size_t before, const StreetEdge& value)
	{
		getOwner().mStreetEdges.insert(getOwner().mStreetEdges.begin() + before, value);

		getOwner().promotePropertyChange(EDGES);
	}

	inline void StreetCrossingComponent::EdgesInterface::remove(std::size_t index)
	{
		getOwner().mStreetEdges.erase(getOwner().mStreetEdges.begin() + index);

		getOwner().promotePropertyChange(EDGES);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
