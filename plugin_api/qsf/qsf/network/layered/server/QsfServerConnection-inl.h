// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline QsfServerConnection::QsfServerConnection(QsfServer* server, uint32 clientId) :
		mServer(server),
		mClientId(clientId)
	{
		// Nothing to do in here
	}

	inline QsfServerConnection::~QsfServerConnection()
	{
		if (nullptr != mProtocol)
		{
			delete mProtocol;
			mProtocol = nullptr;
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
