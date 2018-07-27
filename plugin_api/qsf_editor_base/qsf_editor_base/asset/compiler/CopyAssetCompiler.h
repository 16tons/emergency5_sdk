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
			struct CopyAssetCompilerConfig : public AssetCompilerConfig
			{
				std::string fileExtension;	///< File extension to add to this unprocessed asset type

				QSF_CAMP_RTTI()
			};


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Copy asset compiler class
			*/
			class QSF_EDITOR_BASE_API_EXPORT CopyAssetCompiler : public AssetCompilerImpl<CopyAssetCompilerConfig>
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const std::string CLASS_NAME;
				static const uint32		 CLASS_ID;
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
				inline explicit CopyAssetCompiler(const ProgressFunctionBinding& progressFunction = ProgressFunctionBinding());

				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~CopyAssetCompiler();


			//[-------------------------------------------------------]
			//[ Public virtual qsf::AssetCompiler methods             ]
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
#include "qsf_editor_base/asset/compiler/CopyAssetCompiler-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::CopyAssetCompilerConfig);
