// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline AssetProtocol* AssetClient::getAssetProtocol()
		{
			return mAssetProtocol;
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::QsfClient methods                 ]
		//[-------------------------------------------------------]
		inline const QsfProtocolList& AssetClient::getProtocols() const
		{
			return mProtocols;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
