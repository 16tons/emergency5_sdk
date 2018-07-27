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
	namespace packet
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline ClientHandshake::ClientHandshake() :
			mSelectedProtocolId(getUninitialized<QsfProtocolId>())
		{
			// Nothing to do in here
		}

		inline ClientHandshake::~ClientHandshake()
		{
			// Nothing to do in here
		}

		inline void ClientHandshake::setSelectedProtocolId(QsfProtocolId protocolId)
		{
			mSelectedProtocolId = protocolId;
		}

		inline QsfProtocolId ClientHandshake::getSelectedProtocolId() const
		{
			return mSelectedProtocolId;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // packet
} // qsf
