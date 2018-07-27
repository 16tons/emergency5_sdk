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


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			inline CopyAssetCompiler::CopyAssetCompiler(const ProgressFunctionBinding& progressFunction) :
				AssetCompilerImpl<CopyAssetCompilerConfig>(progressFunction)
			{
				// Nothing to do in here
			}

			inline CopyAssetCompiler::~CopyAssetCompiler()
			{
				// Nothing to do in here
			}


			//[---------------------------------------------------------]
			//[ Public virtual qsf::editor::base::AssetCompiler methods ]
			//[---------------------------------------------------------]
			inline const std::string& CopyAssetCompiler::getClassName() const
			{
				return CLASS_NAME;
			}

			inline const std::string& CopyAssetCompiler::getFileExtension() const
			{
				return getConfig().fileExtension;
			}

			inline uint32 CopyAssetCompiler::getCompilerRevision() const
			{
				return COMPILER_REVISION;
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
