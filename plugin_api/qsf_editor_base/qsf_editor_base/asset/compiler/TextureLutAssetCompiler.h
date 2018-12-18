// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/asset/compiler/AssetCompiler.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace nvtt
{
	struct CompressionOptions;
}


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
			struct TextureLutAssetCompilerConfig : public AssetCompilerConfig
			{
				float         downscaleFactor;			///< Value between 0.0F and 1.0F
				bool          forcePowerOfTwo;			///< true = make sure texture dimensions are a power of two
				int			  dimensionLimit;			///< Downscale to this width/height if bigger (0 = unlimited)
				float         gamma;					///< Gamma correction value
				bool          useGamma;					///< true = apply gamma correction, usually not used because there's a reason why the gamma space is used for 8 bit image data, gamma to linear conversion is done e.g. by the GPU during runtime

				inline TextureLutAssetCompilerConfig() :
					downscaleFactor(1.0f),
					forcePowerOfTwo(false),
					dimensionLimit(0),
					gamma(2.2f),
					useGamma(false)
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
			*    Texture asset compiler class
			*/
			class QSF_EDITOR_BASE_API_EXPORT TextureLutAssetCompiler : public AssetCompilerImpl<TextureLutAssetCompilerConfig>
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				// TODO(fw): Consider using qsf::NamedIdentifier here
				static const std::string  CLASS_NAME;
				static const uint32		 CLASS_ID;
				static const std::string  FILE_EXTENSION;
				static const uint32		 COMPILER_REVISION;


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
				inline explicit TextureLutAssetCompiler(const ProgressFunctionBinding& progressFunction = ProgressFunctionBinding());

				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~TextureLutAssetCompiler();


			//[-------------------------------------------------------]
			//[ Public virtual qsf::editor::base::AssetCompiler methods ]
			//[-------------------------------------------------------]
			public:
				inline virtual const std::string& getClassName() const override;
				inline virtual const std::string& getFileExtension() const override;
				inline virtual uint32 getCompilerRevision() const override;
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
#include "qsf_editor_base/asset/compiler/TextureLutAssetCompiler-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::TextureLutAssetCompilerConfig);
