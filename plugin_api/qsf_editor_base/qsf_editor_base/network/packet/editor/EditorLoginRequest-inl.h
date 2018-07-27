// Copyright (C) 2012-2017 Promotion Software GmbH


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
				inline EditorLoginRequest::EditorLoginRequest()
				{
					// Nothing to be done here
				}

				inline const std::string& EditorLoginRequest::getLogin() const
				{
					return mLogin;
				}

				inline void EditorLoginRequest::setLogin(const std::string& login)
				{
					mLogin = login;
				}

				inline const std::string& EditorLoginRequest::getToken() const
				{
					return mToken;
				}

				inline void EditorLoginRequest::setToken(const std::string& token)
				{
					mToken = token;
				}


				//[-------------------------------------------------------]
				//[ Public virtual methods                                ]
				//[-------------------------------------------------------]
				inline EditorLoginRequest::~EditorLoginRequest()
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
