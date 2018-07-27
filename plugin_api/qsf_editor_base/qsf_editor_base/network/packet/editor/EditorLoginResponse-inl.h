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
				inline EditorLoginResponse::EditorLoginResponse() :
					mIsSuccess(false),
					mIsServerEmpty(false)
				{
					// Nothing to do in here
				}

				inline bool EditorLoginResponse::isSuccess() const
				{
					return mIsSuccess;
				}

				inline void EditorLoginResponse::setSuccess(bool isSuccess)
				{
					mIsSuccess = isSuccess;
				}

				inline bool EditorLoginResponse::isServerEmpty() const
				{
					return mIsServerEmpty;
				}

				inline void EditorLoginResponse::setServerEmpty(bool isServerEmpty)
				{
					mIsServerEmpty = isServerEmpty;
				}

				inline const std::string& EditorLoginResponse::getServerUuid() const
				{
					return mServerUuid;
				}

				inline void EditorLoginResponse::setServerUuid(const std::string& serverUuid)
				{
					mServerUuid = serverUuid;
				}

				inline const boost::property_tree::ptree& EditorLoginResponse::getPluginVersionsPTree() const
				{
					return mPluginVersionsPTree;
				}

				inline void EditorLoginResponse::setPluginVersionsPTree(const boost::property_tree::ptree& pluginVersionsPtree)
				{
					mPluginVersionsPTree = pluginVersionsPtree;
				}

				inline void EditorLoginResponse::setPluginVersionsPTree(boost::property_tree::ptree&& pluginVersionsPtree)
				{
					mPluginVersionsPTree = pluginVersionsPtree;
				}


				//[-------------------------------------------------------]
				//[ Public virtual methods                                ]
				//[-------------------------------------------------------]
				inline EditorLoginResponse::~EditorLoginResponse()
				{
					// Nothing to do in here
				}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
			} // packet
		} // base
	} // editor
} // qsf
