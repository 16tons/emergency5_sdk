// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline QsfClientConnection::QsfClientConnection(QsfClient* client) :
		mClient(client)
	{
		// Nothing to do in here
	}

	inline QsfClientConnection::~QsfClientConnection()
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
