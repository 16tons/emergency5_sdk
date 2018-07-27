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
			inline TextureCubeAssetCompiler::TextureCubeAssetCompiler(const ProgressFunctionBinding& progressFunction) :
				AssetCompilerImpl<AssetCompilerConfig>(progressFunction)
			{
				// Nothing to do in here
			}

			inline TextureCubeAssetCompiler::~TextureCubeAssetCompiler()
			{
				// Nothing to do in here
			}


			//[-------------------------------------------------------]
			//[ Public virtual qsf::editor::base::AssetCompiler methods ]
			//[-------------------------------------------------------]
			inline const std::string& TextureCubeAssetCompiler::getClassName() const
			{
				return CLASS_NAME;
			}

			inline const std::string& TextureCubeAssetCompiler::getFileExtension() const
			{
				return FILE_EXTENSION;
			}

			inline uint32 TextureCubeAssetCompiler::getCompilerRevision() const
			{
				return COMPILER_REVISION;
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
