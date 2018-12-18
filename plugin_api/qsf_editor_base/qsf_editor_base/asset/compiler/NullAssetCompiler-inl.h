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
			inline NullAssetCompiler::NullAssetCompiler(const ProgressFunctionBinding& progressFunction) :
				AssetCompilerImpl<NullAssetCompilerConfig>(progressFunction)
			{
				// Nothing to do in here
			}

			inline NullAssetCompiler::~NullAssetCompiler()
			{
				// Nothing to do in here
			}


			//[---------------------------------------------------------]
			//[ Public virtual qsf::editor::base::AssetCompiler methods ]
			//[---------------------------------------------------------]
			inline const std::string& NullAssetCompiler::getClassName() const
			{
				return CLASS_NAME;
			}

			inline const std::string& NullAssetCompiler::getFileExtension() const
			{
				static const std::string emptyString;
				return emptyString;
			}

			inline uint32 NullAssetCompiler::getCompilerRevision() const
			{
				return COMPILER_REVISION;
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
