// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"
#include "qsf_editor_base/asset/AssetSystemTypes.h"

#include <qsf/base/BoostPtreeHelper.h>
#include <qsf/reflection/object/Object.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	class BaseCachedAsset;
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
			struct AssetCompilerConfig : public Object
			{
			public:
				inline AssetCompilerConfig();

				inline AssetCompilerConfigRevision getConfigRevision() const;
				inline void setConfigRevision(AssetCompilerConfigRevision configRevision);

				inline float getProcessingPriority() const;
				inline void setProcessingPriority(float processingPriority);

			private:
				AssetCompilerConfigRevision mConfigRevision;
				float						mProcessingPriority;

				QSF_CAMP_RTTI()
			};


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Asset compiler base class
			*/
			class QSF_EDITOR_BASE_API_EXPORT AssetCompiler : public boost::noncopyable
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				/** Function binding signature for progress callbacks (progress between [0..1]) */
				typedef void ProgressBindingSignature(const std::string& message, float progress);

				/** Boost function binding definition for progress callbacks */
				typedef boost::function<ProgressBindingSignature> ProgressFunctionBinding;


			//[-------------------------------------------------------]
			//[ Public virtual qsf::editor::base::AssetCompiler methods ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~AssetCompiler();

				/** Returns the type name this compiler instance is meant to process */
				inline const std::string& getTypeName() const;

				/** Sets the type name this compiler instance is meant to process */
				inline void setTypeName(const std::string& typeName);


			//[---------------------------------------------------------]
			//[ Public virtual qsf::editor::base::AssetCompiler methods ]
			//[---------------------------------------------------------]
			public:
				/** Returns the compiler's class name */
				virtual const std::string& getClassName() const = 0;

				/** Returns the file extension of the cached assets this compiler instance produces */
				virtual const std::string& getFileExtension() const = 0;

				/** Returns the queue priority of this compiler instance (long-running compilers should get a lower priority so they don't block the compilation of other assets) */
				virtual float getAssetPriority() const = 0;

				/** Returns the configuration object of this compiler instance */
				virtual Object& getConfigObject() = 0;

				/** Returns the configuration object of this compiler instance */
				virtual const Object& getConfigObject() const = 0;

				/** Returns the compiler revison of this compiler instance */
				virtual AssetCompilerRevision getCompilerRevision() const = 0;

				/** Returns the compiler configuration revison of this compiler instance */
				virtual AssetCompilerConfigRevision getConfigRevision() const = 0;

				/** Returns a boost ptree containing all relevant asset dependencies of the given asset with their source revisions. (default implementation returns an empty ptree) */
				inline virtual boost::property_tree::ptree buildDependencyRevisionTree(GlobalAssetId globalAssetId) const;

				/** Returns true if the asset with given global asset ID can be compiled right now. (default implementation always returns true)
				*
				* @note
				*   - This method is meant to delay compilation attempts if asset dependencies are not available yet. Return true here if the reason for the asset not getting successfully compiled is permanent and should be reported to the user.
				*/
				inline virtual bool canCompile(GlobalAssetId globalAssetId) const;

				/**
				*  @brief
				*    Compile an asset
				*
				*  @param[in] absoluteSourceFilename
				*    Absolute filename of the source asset to compile
				*  @param[in] absoluteDestinationFilename
				*    Absolute filename of the compiled asset
				*  @param[in] globalAssetId
				*    Global asset ID of the asset to compile
				*  @param[in] cachedAsset
				*    Optional cached asset instance which might get feed with dynamic asset properties (e.g. a texture compiler might add pixel format information etc. for a texture streamer)
				*
				*  @return
				*    "true" if all went fine, else "false"
				*/
				virtual bool compile(const std::string& absoluteSourceFilename, const std::string& absoluteDestinationFilename, BaseCachedAsset& cachedAsset) const = 0;


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			protected:
				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] progressFunction
				*    Optional function to call on notable progress
				*/
				inline explicit AssetCompiler(const ProgressFunctionBinding& progressFunction);

				/**
				*  @brief
				*    Call this method in order to tell others about the asset compiler progress
				*
				*  @param[in] message
				*    Progress message
				*  @param[in] progress
				*    Progress between [0..1] (1 means done)
				*/
				void emitProgress(const std::string& message, float progress = 0.0f) const;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				ProgressFunctionBinding mProgressFunction;	///< Optional function to call on notable progress
				std::string				mTypeName;


			};

			template<class ConfigType>
			class AssetCompilerImpl : public AssetCompiler
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~AssetCompilerImpl();

				/** Returns the configuration object of this compiler instance (like getConfigObject() but this time casted to the correct type for easy access) */
				inline ConfigType& getConfig();

				/** Returns the configuration object of this compiler instance (like getConfigObject() but this time casted to the correct type for easy access) */
				inline const ConfigType& getConfig() const;


			//[---------------------------------------------------------]
			//[ Public virtual qsf::editor::base::AssetCompiler methods ]
			//[---------------------------------------------------------]
			public:
				inline virtual float getAssetPriority() const override;
				inline virtual Object& getConfigObject() override;
				inline virtual const Object& getConfigObject() const override;
				inline virtual AssetCompilerConfigRevision getConfigRevision() const override;


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			protected:
				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] progressFunction
				*    Optional function to call on notable progress
				*/
				inline explicit AssetCompilerImpl(const ProgressFunctionBinding& progressFunction);


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				ConfigType mConfig;	///< Asset compiler configuration, always valid


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
#include "qsf_editor_base/asset/compiler/AssetCompiler-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::AssetCompilerConfig);
