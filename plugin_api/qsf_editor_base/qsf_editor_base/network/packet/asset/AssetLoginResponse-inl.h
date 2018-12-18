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
				inline AssetLoginResponse::~AssetLoginResponse()
				{
					// Nothing to do in here
				}

				inline const std::string& AssetLoginResponse::getToken() const
				{
					return mToken;
				}

				inline uint32 AssetLoginResponse::getUserId() const
				{
					return mUserId;
				}

				inline const UserBase& AssetLoginResponse::getUser() const
				{
					return mUser;
				}

				inline void AssetLoginResponse::set(const std::string& token, uint32 userId, const UserBase& user)
				{
					mToken = token;
					mUserId = userId;
					mUser = user;
				}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
			} // packet
		} // base
	} // editor
} // qsf
