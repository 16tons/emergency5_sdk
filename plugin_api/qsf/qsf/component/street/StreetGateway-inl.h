// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline StreetGateway::StreetGateway() :
		GatewayNodes(*this)
	{
		// Nothing to do in here
	}

	inline StreetGateway::StreetGateway(const std::vector<uint32>& gatewayNodes) :
		mGatewayNodes(gatewayNodes),
		GatewayNodes(*this)
	{
		// Nothing to do in here
	}

	inline StreetGateway::StreetGateway(const StreetGateway& source) :
		mGatewayNodes(source.mGatewayNodes),
		mStreetGatewayLink(source.mStreetGatewayLink),
		GatewayNodes(*this)
	{
		// Nothing to do in here
	}

	inline const std::vector<uint32>& StreetGateway::getGatewayNodes() const
	{
		return mGatewayNodes;
	}

	inline void StreetGateway::setGatewayNodes(const std::vector<uint32>& gatewayNodes)
	{
		mGatewayNodes = gatewayNodes;
	}

	inline const StreetGatewayLink& StreetGateway::getStreetGatewayLink() const
	{
		return mStreetGatewayLink;
	}

	inline void StreetGateway::setStreetGatewayLink(const StreetGatewayLink& streetGatewayLink)
	{
		mStreetGatewayLink = streetGatewayLink;
	}

	inline StreetGateway& StreetGateway::operator =(const StreetGateway& source)
	{
		mGatewayNodes = source.mGatewayNodes;
		mStreetGatewayLink = source.mStreetGatewayLink;
		return *this;
	}

	inline bool StreetGateway::operator ==(const StreetGateway& source) const
	{
		return (mGatewayNodes == source.mGatewayNodes && mStreetGatewayLink == source.mStreetGatewayLink);
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	inline std::size_t StreetGateway::GatewayNodesInterface::size() const
	{
		return getOwner().mGatewayNodes.size();
	}

	inline const uint32& StreetGateway::GatewayNodesInterface::get(std::size_t index) const
	{
		return getOwner().mGatewayNodes.at(index);
	}

	inline void StreetGateway::GatewayNodesInterface::set(std::size_t index, const uint32& value)
	{
		getOwner().mGatewayNodes.at(index) = value;
	}

	inline void StreetGateway::GatewayNodesInterface::insert(std::size_t before, const uint32& value)
	{
		getOwner().mGatewayNodes.insert(getOwner().mGatewayNodes.begin() + before, value);
	}

	inline void StreetGateway::GatewayNodesInterface::remove(std::size_t index)
	{
		getOwner().mGatewayNodes.erase(getOwner().mGatewayNodes.begin() + index);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
