// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/GetUninitialized.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline StreetGatewayLink::StreetGatewayLink() :
		mConnectedEntityId(getUninitialized<uint64>()),
		mStreetGatewayIndex(getUninitialized<uint32>())
	{
		// Nothing to do in here
	}

	inline StreetGatewayLink::StreetGatewayLink(const StreetGatewayLink& source) :
		mConnectedEntityId(source.mConnectedEntityId),
		mStreetGatewayIndex(source.mStreetGatewayIndex)
	{
		// Nothing to do in here
	}

	inline StreetGatewayLink::StreetGatewayLink(uint64 entityId, const StreetGatewayLink& streetGatewayLink) :
		mConnectedEntityId(entityId),
		mStreetGatewayIndex(streetGatewayLink.mStreetGatewayIndex)
	{
		// Nothing to do in here
	}

	inline StreetGatewayLink::StreetGatewayLink(uint64 entityId, uint32 streetGatewayIndex) :
		mConnectedEntityId(entityId),
		mStreetGatewayIndex(streetGatewayIndex)
	{
		// Nothing to do in here
	}

	inline uint64 StreetGatewayLink::getConnectedEntityId() const
	{
		return mConnectedEntityId;
	}

	inline void StreetGatewayLink::setConnectedEntityId(uint64 entityId)
	{
		mConnectedEntityId = entityId;
	}

	inline uint32 StreetGatewayLink::getStreetGatewayIndex() const
	{
		return mStreetGatewayIndex;
	}

	inline void StreetGatewayLink::setStreetGatewayIndex(uint32 streetGatewayIndex)
	{
		mStreetGatewayIndex = streetGatewayIndex;
	}

	inline StreetGatewayLink& StreetGatewayLink::operator =(const StreetGatewayLink& source)
	{
		mConnectedEntityId = source.mConnectedEntityId;
		mStreetGatewayIndex = source.mStreetGatewayIndex;
		return *this;
	}

	inline bool StreetGatewayLink::operator ==(const StreetGatewayLink& source) const
	{
		return (mConnectedEntityId == source.mConnectedEntityId && mStreetGatewayIndex == source.mStreetGatewayIndex);
	}

	inline bool StreetGatewayLink::operator !=(const StreetGatewayLink& source) const
	{
		return (mConnectedEntityId != source.mConnectedEntityId || mStreetGatewayIndex != source.mStreetGatewayIndex);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}
