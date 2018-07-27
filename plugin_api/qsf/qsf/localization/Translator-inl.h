// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/BoostAssertHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LocalizationSystem& Translator::getLocalizationSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mLocalizationSystem, "The localization system instance is invalid", QSF_REACT_THROW);
		return *mLocalizationSystem;
	}

	inline const std::string& Translator::getFilename() const
	{
		return mFilename;
	}

	inline const std::string& Translator::getLanguage() const
	{
		return mLanguage;
	}

	inline void Translator::setLanguage(const std::string& language)
	{
		mLanguage = language;
	}

	inline const std::vector<LocalizationContext*>& Translator::getLocalizationContexts() const
	{
		return mLocalizationContexts;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline Translator::Translator(LocalizationSystem& localizationSystem) :
		mLocalizationSystem(&localizationSystem)
	{
		// Nothing to do in here
	}

	inline Translator::~Translator()
	{
		// Destroy all localization contexts within this translator
		destroyAllLocalizationContexts();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
