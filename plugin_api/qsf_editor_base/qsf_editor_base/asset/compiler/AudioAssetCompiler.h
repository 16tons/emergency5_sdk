// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/asset/compiler/AssetCompiler.h"

#include <qsf/reflection/object/Object.h>


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
			struct AudioAssetCompilerConfig : public AssetCompilerConfig
			{
				enum Encoding ///< enum registered in CAMP - please update Plugin.cpp accordingly when changing this enum!
				{
					CBR,
					VBR,
					ABR
				};

				Encoding encoding;		///< CBR, VBR or ABR?
				uint32	 targetBitrate;	///< desired (average) bitrate for CBR and ABR, maximum bitrate for VBR (0 = unlimited)
				float    targetQuality;	///< value between 1.0F and 0.0F specifying desired encoding quality for VBR encoding

				inline AudioAssetCompilerConfig() :
					encoding(CBR),
					targetBitrate(128000),
					targetQuality(0.5f)
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
			*    Audio asset compiler class
			*/
			class QSF_EDITOR_BASE_API_EXPORT AudioAssetCompiler : public AssetCompilerImpl<AudioAssetCompilerConfig>
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const std::string CLASS_NAME;
				static const uint32		 CLASS_ID;
				static const std::string FILE_EXTENSION;
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
				inline explicit AudioAssetCompiler(const ProgressFunctionBinding& progressFunction = ProgressFunctionBinding());

				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~AudioAssetCompiler();


			//[-------------------------------------------------------]
			//[ Public virtual qsf::AssetCompiler methods             ]
			//[-------------------------------------------------------]
			public:
				inline virtual const std::string& getClassName() const override;
				inline virtual const std::string& getFileExtension() const override;
				inline virtual uint32 getCompilerRevision() const override;
				virtual bool compile(const std::string& absoluteSourceFilename, const std::string& absoluteDestinationFilename, BaseCachedAsset& cachedAsset) const override;


			//[-------------------------------------------------------]
			//[ Private static data                                   ]
			//[-------------------------------------------------------]
			private:
				static const size_t WAV_READ_CACHE_SIZE = 4096;


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
#include "qsf_editor_base/asset/compiler/AudioAssetCompiler-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(qsf::editor::base::AudioAssetCompilerConfig::Encoding);
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::AudioAssetCompilerConfig);
