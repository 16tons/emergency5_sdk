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
			inline TextureAssetCompiler::TextureAssetCompiler(const ProgressFunctionBinding& progressFunction) :
				AssetCompilerImpl<TextureAssetCompilerConfig>(progressFunction)
			{
				// Nothing to do in here
			}

			inline TextureAssetCompiler::~TextureAssetCompiler()
			{
				// Nothing to do in here
			}


			//[-------------------------------------------------------]
			//[ Public virtual qsf::editor::base::AssetCompiler methods ]
			//[-------------------------------------------------------]
			inline const std::string& TextureAssetCompiler::getClassName() const
			{
				return CLASS_NAME;
			}

			inline const std::string& TextureAssetCompiler::getFileExtension() const
			{
				return FILE_EXTENSION;
			}

			inline uint32 TextureAssetCompiler::getCompilerRevision() const
			{
				return COMPILER_REVISION;
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
