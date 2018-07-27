// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/asset/compiler/AssetCompiler.h"

#include <qsf/reflection/CampClass.h>


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
			//[ Structs                                               ]
			//[-------------------------------------------------------]
			struct MaterialAssetCompilerConfig : public AssetCompilerConfig
			{
				inline MaterialAssetCompilerConfig()
				{
					// Nothing to do in here
				};

				QSF_CAMP_RTTI()
			};


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Material asset compiler class
			*
			*  @todo
			*    - TODO(co) Currently hard wired with the configuration used for EMERGENCY 5 PC, make it possible to configure the material compiler from the outside
			*/
			class QSF_EDITOR_BASE_API_EXPORT MaterialAssetCompiler : public AssetCompilerImpl<MaterialAssetCompilerConfig>
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const std::string  CLASS_NAME;
				static const uint32		  CLASS_ID;
				static const std::string  FILE_EXTENSION;
				static const uint32		  COMPILER_REVISION;


			//[-------------------------------------------------------]
			//[ Public static methods                                 ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Set texture downscale factor
				*
				*  @param[in] textureDownscaleFactor
				*    Texture downscale factor
				*
				*  @todo
				*    - TODO(co) Quick'n'dirty poor mans solution to be at least able to change the texture scale. We need a decent project specific asset compiler configuration.
				*/
				static void setTextureDownscaleFactor(float textureDownscaleFactor);


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] progressFunction
				*    Optional function to call on notable progress
				*/
				inline explicit MaterialAssetCompiler(const ProgressFunctionBinding& progressFunction = ProgressFunctionBinding());

				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~MaterialAssetCompiler();

				bool compile(const std::string& absoluteSourceFilename, const std::string& absoluteDestinationFilename, BaseCachedAsset& cachedAsset, bool compileTextures) const;

				bool buildLocalizedMaterial(const std::string& absoluteSourceFilename, const std::string& absoluteMaterialDestinationFilename, const std::string& absoluteTextureDestinationPath, const std::string& localization, const BaseCachedAsset& cachedAsset) const;


			//[-------------------------------------------------------]
			//[ Public virtual qsf::AssetCompiler methods             ]
			//[-------------------------------------------------------]
			public:
				inline virtual const std::string& getClassName() const override;
				inline virtual const std::string& getFileExtension() const override;
				inline virtual uint32 getCompilerRevision() const override;
				virtual boost::property_tree::ptree buildDependencyRevisionTree(GlobalAssetId globalAssetId) const override;
				virtual bool canCompile(GlobalAssetId globalAssetId) const override;
				virtual bool compile(const std::string& absoluteSourceFilename, const std::string& absoluteDestinationFilename, BaseCachedAsset& cachedAsset) const override;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor_base/asset/compiler/MaterialAssetCompiler-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::MaterialAssetCompilerConfig);
