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
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			inline AudioAssetCompiler::AudioAssetCompiler(const ProgressFunctionBinding& progressFunction) :
				AssetCompilerImpl<AudioAssetCompilerConfig>(progressFunction)
			{
				// Nothing to do in here
			}

			inline AudioAssetCompiler::~AudioAssetCompiler()
			{
				// Nothing to do in here
			}


			//[-------------------------------------------------------]
			//[ Public virtual qsf::AssetCompiler methods             ]
			//[-------------------------------------------------------]
			inline const std::string& AudioAssetCompiler::getClassName() const
			{
				return CLASS_NAME;
			}

			inline const std::string& AudioAssetCompiler::getFileExtension() const
			{
				return FILE_EXTENSION;
			}

			inline uint32 AudioAssetCompiler::getCompilerRevision() const
			{
				return COMPILER_REVISION;
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
