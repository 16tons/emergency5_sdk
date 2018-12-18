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


			//[-------------------------------------------------------]
			//[ Public qsf::editor::base::AssetCompilerConfig methods ]
			//[-------------------------------------------------------]
			inline AssetCompilerConfig::AssetCompilerConfig() :
				mConfigRevision(0),
				mProcessingPriority(0.5f)
			{
				// Nothing to do in here
			}

			inline AssetCompilerConfigRevision AssetCompilerConfig::getConfigRevision() const
			{
				return mConfigRevision;
			}

			inline void AssetCompilerConfig::setConfigRevision(AssetCompilerConfigRevision configRevision)
			{
				mConfigRevision = configRevision;
			}

			inline float AssetCompilerConfig::getProcessingPriority() const
			{
				return mProcessingPriority;
			}

			inline void AssetCompilerConfig::setProcessingPriority(float processingPriority)
			{
				mProcessingPriority = processingPriority;
			}


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			inline AssetCompiler::~AssetCompiler()
			{
				// Nothing to do in here
			}

			inline const std::string& AssetCompiler::getTypeName() const
			{
				return mTypeName;
			}

			inline void AssetCompiler::setTypeName(const std::string& typeName)
			{
				mTypeName = typeName;
			}


			//[---------------------------------------------------------]
			//[ Public virtual qsf::editor::base::AssetCompiler methods ]
			//[---------------------------------------------------------]
			boost::property_tree::ptree AssetCompiler::buildDependencyRevisionTree(GlobalAssetId globalAssetId) const
			{
				return boost::property_tree::ptree();
			}

			bool AssetCompiler::canCompile(GlobalAssetId globalAssetId) const
			{
				return true;
			}


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			inline AssetCompiler::AssetCompiler(const ProgressFunctionBinding& progressFunction) :
				mProgressFunction(progressFunction)
			{
				// Nothing to do in here
			}

			inline void AssetCompiler::emitProgress(const std::string& message, float progress) const
			{
				if (!mProgressFunction.empty())
				{
					mProgressFunction(message, progress);
				}
			}


			//[-------------------------------------------------------]
			//[ Public qsf::editor::base::AssetCompilerImpl<ConfigType> methods ]
			//[-------------------------------------------------------]
			template<class ConfigType>
			inline AssetCompilerImpl<ConfigType>::~AssetCompilerImpl()
			{
				// Nothing to do in here
			}

			template<class ConfigType>
			inline ConfigType& AssetCompilerImpl<ConfigType>::getConfig()
			{
				return mConfig;
			}

			template<class ConfigType>
			inline const ConfigType& AssetCompilerImpl<ConfigType>::getConfig() const
			{
				return mConfig;
			}


			//[---------------------------------------------------------]
			//[ Public virtual qsf::editor::base::AssetCompiler methods ]
			//[---------------------------------------------------------]
			template<class ConfigType>
			inline float AssetCompilerImpl<ConfigType>::getAssetPriority() const
			{
				return mConfig.getProcessingPriority();
			}

			template<class ConfigType>
			inline Object& AssetCompilerImpl<ConfigType>::getConfigObject()
			{
				return mConfig;
			}

			template<class ConfigType>
			inline const Object& AssetCompilerImpl<ConfigType>::getConfigObject() const
			{
				return mConfig;
			}

			template<class ConfigType>
			inline AssetCompilerConfigRevision AssetCompilerImpl<ConfigType>::getConfigRevision() const
			{
				return mConfig.getConfigRevision();
			}


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			template<class ConfigType>
			inline AssetCompilerImpl<ConfigType>::AssetCompilerImpl(const ProgressFunctionBinding& progressFunction) :
				AssetCompiler(progressFunction)
			{
				// Nothing to do in here
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
