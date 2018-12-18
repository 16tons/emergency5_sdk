// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			namespace packet
			{


				//[-------------------------------------------------------]
				//[ Public methods                                        ]
				//[-------------------------------------------------------]
				inline AssetLoginRequest::AssetLoginRequest()
				{
					// Nothing to be done here
				}

				inline const std::string& AssetLoginRequest::getLogin() const
				{
					return mLogin;
				}

				inline void AssetLoginRequest::setLogin(const std::string& login)
				{
					mLogin = login;
				}


				//[-------------------------------------------------------]
				//[ Public virtual methods                                ]
				//[-------------------------------------------------------]
				inline AssetLoginRequest::~AssetLoginRequest()
				{
					// Nothing to be done here
				}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
			} // packet
		} // base
	} // editor
} // qsf
