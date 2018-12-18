// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class LocalizationSystem;
	class LocalizationContext;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Translator class
	*
	*  @remarks
	*    A translator is a collection of localization context instances. Translators are completely independent from
	*    each other and each translator is serialized by using its own file.
	*
	*    The translator is reusing the Qt internationalization infrastructure. The Qt translation class is named "QTranslator", so the class name
	*    "Translator" was chosen to highlight that we're basically using the Qt way of handling localization. Meaning we don't introduce new
	*    proprietary localization file formats. Instead, we're using Qt's XML "ts" file format. This way we can use the Qt tools "lupdate" and
	*    especially "linguist" in order to streamline the localization process.
	*/
	class QSF_API_EXPORT Translator : public Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class LocalizationSystem;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the owner localization system
		*
		*  @return
		*    The owner localization system, do not destroy the instance
		*/
		inline LocalizationSystem& getLocalizationSystem() const;

		/**
		*  @brief
		*    Load a translator by using a given filename
		*
		*  @param[in] filename
		*    UTF-8 filename in platform-independent notation of the translator to load
		*
		*  @return
		*    "true" if all went fine, else "false" (file not found?)
		*
		*  @note
		*    - The current translator content will get lost, so, you might want to save a manipulated translator before loading a new one
		*    - On success, automatically makes the given filename to the internally stored translator filename
		*
		*  @todo
		*    - TODO(co) For now, we only support Qt XML ts
		*/
		bool loadByFilename(const std::string& filename);

		/**
		*  @brief
		*    Save the translator by using a given filename
		*
		*  @param[in] filename
		*    UTF-8 filename in platform-independent notation of the translator to save
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - On success, automatically makes the given filename to the internally stored translator filename
		*
		*  @todo
		*    - TODO(co) For now, we only support Qt XML ts
		*/
		bool saveByFilename(const std::string& filename);

		/**
		*  @brief
		*    Return the filename of the translator
		*
		*  @return
		*    The UTF-8 filename in platform-independent notation of the translator, do no destroy the returned instance
		*
		*  @note
		*    - There's no "setFilename()"-method by intent
		*/
		inline const std::string& getFilename() const;

		/**
		*  @brief
		*    Return the language
		*
		*  @return
		*    The UTF-8 language, e.g. "en"
		*/
		inline const std::string& getLanguage() const;

		/**
		*  @brief
		*    Set the language
		*
		*  @param[in] language
		*    The UTF-8 language to set, e.g. "en"
		*/
		inline void setLanguage(const std::string& language);

		/**
		*  @brief
		*    Create a new localization context instance
		*
		*  @return
		*    The created localization context instance, null pointer on error, don't destroy the returned instance
		*/
		LocalizationContext* createLocalizationContext();

		/**
		*  @brief
		*    Destroy a given localization context instance
		*
		*  @param[in] localizationContext
		*    The localization context instance to destroy, after this method call this instance is no longer valid
		*/
		void destroyLocalizationContext(LocalizationContext& localizationContext);

		/**
		*  @brief
		*    Destroy all localization contexts within this translator
		*/
		void destroyAllLocalizationContexts();

		/**
		*  @brief
		*    Return the list of currently loaded localization contexts
		*
		*  @return
		*    Reference to the internal list of currently loaded localization context, do not manipulate the list or destroy the localization contexts
		*/
		inline const std::vector<LocalizationContext*>& getLocalizationContexts() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] localizationSystem
		*    Owner localization system, must stay valid as long as this translator instance exists, do not destroy the instance
		*/
		inline explicit Translator(LocalizationSystem& localizationSystem);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Translator();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		LocalizationSystem*				  mLocalizationSystem;		///< Owner localization system, always valid, do not destroy the instance
		std::string						  mFilename;				///< UTF-8 filename in platform-independent notation of the translator, in case it was loaded from a file
		std::string						  mLanguage;				///< The UTF-8 language, e.g. "en"
		std::vector<LocalizationContext*> mLocalizationContexts;	///< Localization context instances, we're responsible for destroying the instances in case we no longer need it


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/localization/Translator-inl.h"
