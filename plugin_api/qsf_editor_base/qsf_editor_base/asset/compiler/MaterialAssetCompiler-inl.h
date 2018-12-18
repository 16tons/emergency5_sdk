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
			inline MaterialAssetCompiler::MaterialAssetCompiler(const ProgressFunctionBinding& progressFunction) :
				AssetCompilerImpl<MaterialAssetCompilerConfig>(progressFunction)
			{
				// Nothing to do in here
			}

			inline MaterialAssetCompiler::~MaterialAssetCompiler()
			{
				// Nothing to do in here
			}


			//[-------------------------------------------------------]
			//[ Public virtual qsf::AssetCompiler methods             ]
			//[-------------------------------------------------------]
			inline const std::string& MaterialAssetCompiler::getClassName() const
			{
				return CLASS_NAME;
			}

			inline const std::string& MaterialAssetCompiler::getFileExtension() const
			{
				return FILE_EXTENSION;
			}

			inline uint32 MaterialAssetCompiler::getCompilerRevision() const
			{
				return COMPILER_REVISION;
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
