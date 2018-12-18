// Copyright (C) 2012-2018 Promotion Software GmbH


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
		inline ServerHandshake::ServerHandshake()
		{
			// Nothing to do in here
		}

		inline ServerHandshake::~ServerHandshake()
		{
			// Nothing to do in here
		}

		inline const QsfProtocolIdList& ServerHandshake::getProtocolIds() const
		{
			return mProtocolIds;
		}

		inline void ServerHandshake::setProtocolIds(const QsfProtocolList& protocols)
		{
			mProtocolIds.clear();

			for (uint32 i = 0; i < protocols.size(); ++i)
			{
				mProtocolIds.push_back(protocols[i].first);
			}
		}

		inline void ServerHandshake::setProtocolIds(const QsfProtocolIdList& protocolIds)
		{
			mProtocolIds = protocolIds;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // packet
} // qsf
