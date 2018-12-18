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
			inline TextureLutAssetCompiler::TextureLutAssetCompiler(const ProgressFunctionBinding& progressFunction) :
				AssetCompilerImpl<TextureLutAssetCompilerConfig>(progressFunction)
			{
				// Nothing to do in here
			}

			inline TextureLutAssetCompiler::~TextureLutAssetCompiler()
			{
				// Nothing to do in here
			}


			//[-------------------------------------------------------]
			//[ Public virtual qsf::editor::base::AssetCompiler methods ]
			//[-------------------------------------------------------]
			inline const std::string& TextureLutAssetCompiler::getClassName() const
			{
				return CLASS_NAME;
			}

			inline const std::string& TextureLutAssetCompiler::getFileExtension() const
			{
				return FILE_EXTENSION;
			}

			inline uint32 TextureLutAssetCompiler::getCompilerRevision() const
			{
				return COMPILER_REVISION;
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
