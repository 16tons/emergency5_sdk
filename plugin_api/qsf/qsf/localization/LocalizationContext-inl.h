// Copyright (C) 2012-2018 Promotion Software GmbH


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
	inline Translator& LocalizationContext::getTranslator() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mTranslator, "The translator instance is invalid", QSF_REACT_THROW);
		return *mTranslator;
	}

	inline const std::string& LocalizationContext::getName() const
	{
		return mName;
	}

	inline const std::vector<LocalizationMessage*>& LocalizationContext::getLocalizationMessages() const
	{
		return mLocalizationMessages;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline LocalizationContext::LocalizationContext(Translator& translator) :
		mTranslator(&translator)
	{
		// Nothing to do in here
	}

	inline LocalizationContext::~LocalizationContext()
	{
		// Destroy all localization messages within this translator
		destroyAllLocalizationMessages();

		// Set empty string to unregister this localization context
		setName("");
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
