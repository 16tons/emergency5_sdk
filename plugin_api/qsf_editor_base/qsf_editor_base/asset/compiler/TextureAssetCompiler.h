// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/asset/compiler/AssetCompiler.h"

#include <qsf/base/GetUninitialized.h>

#include <vector>


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
			struct TextureAssetCompilerConfig : public AssetCompilerConfig
			{
				enum Compression ///< enum registered in CAMP - please update Plugin.cpp accordingly when changing this enum!
				{
					CUSTOM_UNORM,
					CUSTOM_FLOAT,

					BC1,	///< DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block). Automatically switches to "BC1a" (does not exist in specification) if alpha present.
					BC2,	///< DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
					BC3,	///< DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block).
					BC3n,	///< Swizzled BC3 (only supports two channels: puts the first into alpha and the second one into green, sets red and blue to fixed values) format for normal map compression (red=x stored in alpha, blue=z unused and has to be calculated inside a shader, "DXT5\A2D5" will be written into the dds metadata and e.g. "The Compressenator" from AMD will show it as usual normal map)
					BC4,	///< 1 component texture compression (also known as 3DC+/ATI1N, known as LATC1 in OpenGL, known as BC4 in DirectX 10, 8 bytes per block)
					BC5,	///< 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as LATC2 in OpenGL, known as BC5 in DirectX 10, 16 bytes per block)
					BC6H,	///< BC6H	// TODO(co) Available now, as soon as we need it integrate and test it
					BC7,	///< BC7	// TODO(co) Available now, as soon as we need it integrate and test it

					//ETC1  ///< Unsupported by nvtt -- Suggestion: https://code.google.com/p/rg-etc1/
				};

				struct ChannelInfo
				{
					virtual inline ~ChannelInfo() {};

					uint32 sourceChannel;			///< Which channel index in source images to use (0 based index; every input file has 4 channels)
					uint32 bitCount;				///< How many bits has this channel? (only for custom formats)
					// Optional
					uint32 multiplyWithChannel;		///< Which channel index in source images to multiply with this one (0 based index; every input file has 4 channels; set to uninitialized to disable multiplication)
					float  multiplyWithConstant;	///< Constant to multiply with this channel (set to 1.0 to disable)
					float  clearValue;				///< In case the channel has no input, this is the value the channel is cleared with

					inline ChannelInfo() :
						sourceChannel(getUninitialized<uint32>()),
						bitCount(8),
						multiplyWithChannel(getUninitialized<uint32>()),
						multiplyWithConstant(1.0f),
						clearValue(1.0f)
					{
						// Nothing to do in here
					};

					inline ChannelInfo(uint32 sourceChannel, uint32 bitCount = 8) :
						sourceChannel(sourceChannel),
						bitCount(bitCount),
						multiplyWithChannel(getUninitialized<uint32>()),
						multiplyWithConstant(1.0f),
						clearValue(1.0f)
					{
						// Nothing to do in here
					};

					QSF_CAMP_RTTI()
				};

				typedef std::vector<ChannelInfo> ChannelConfig;

				Compression   compression;				///< Use what compression?
				ChannelConfig channelConfig;			///< How many channels, where to get them from and what they are
				float         downscaleFactor;			///< Value between 0.0F and 1.0F
				bool          forcePowerOfTwo;			///< true = make sure texture dimensions are a power of two
				bool          disallowNonPowerOfTwo;	///< true = break compilation when a non-power of two texture is encountered
				int			  dimensionLimit;			///< Downscale to this width/height if bigger (0 = unlimited)
				float         gamma;					///< Gamma correction value
				bool          useGamma;					///< true = apply gamma correction, usually not used because there's a reason why the gamma space is used for 8 bit image data, gamma to linear conversion is done e.g. by the GPU during runtime
				bool          generateMipmaps;			///< true = generate mipmaps
				float         alphaCoverageThreshold;	///< BC1a: alpha value threshold between transparent and opaque
				bool		  hasAlpha;					///< Does the 4th channel contain transparency information if present?

				inline TextureAssetCompilerConfig() :
					compression(CUSTOM_UNORM),
					downscaleFactor(1.0f),
					forcePowerOfTwo(false),
					disallowNonPowerOfTwo(false),
					dimensionLimit(0),
					gamma(2.2f),
					useGamma(false),
					generateMipmaps(false),
					alphaCoverageThreshold(0.95f),
					hasAlpha(true)
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
			class QSF_EDITOR_BASE_API_EXPORT TextureAssetCompiler : public AssetCompilerImpl<TextureAssetCompilerConfig>
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
				inline explicit TextureAssetCompiler(const ProgressFunctionBinding& progressFunction = ProgressFunctionBinding());

				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~TextureAssetCompiler();

				bool compile(const std::vector<std::string>& absoluteSourceFilenames, const std::string& absoluteDestinationFilename, BaseCachedAsset& cachedAsset) const;


			//[-------------------------------------------------------]
			//[ Public virtual qsf::editor::base::AssetCompiler methods ]
			//[-------------------------------------------------------]
			public:
				inline virtual const std::string& getClassName() const override;
				inline virtual const std::string& getFileExtension() const override;
				inline virtual uint32 getCompilerRevision() const override;
				virtual bool compile(const std::string& absoluteSourceFilename, const std::string& absoluteDestinationFilename, BaseCachedAsset& cachedAsset) const override;


			//[-------------------------------------------------------]
			//[ Private methods                                       ]
			//[-------------------------------------------------------]
			private:
				bool setPixelFormatFromConfig(const TextureAssetCompilerConfig::ChannelConfig& channelConfig, nvtt::CompressionOptions& compressionOptions) const;


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
#include "qsf_editor_base/asset/compiler/TextureAssetCompiler-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(qsf::editor::base::TextureAssetCompilerConfig::Compression);
QSF_CAMP_TYPE(qsf::editor::base::TextureAssetCompilerConfig::ChannelInfo);
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::TextureAssetCompilerConfig);
