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
	inline LocalizationContext& LocalizationMessage::getLocalizationContext() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mLocalizationContext, "The localization context instance is invalid", QSF_REACT_THROW);
		return *mLocalizationContext;
	}

	inline const std::string& LocalizationMessage::getSource() const
	{
		return mSource;
	}

	inline const std::string& LocalizationMessage::getTranslatorComment() const
	{
		return mTranslatorComment;
	}

	inline void LocalizationMessage::setTranslatorComment(const std::string& translatorComment)
	{
		mTranslatorComment = translatorComment;
	}

	inline const std::string& LocalizationMessage::getTranslation() const
	{
		return mTranslation;
	}

	inline void LocalizationMessage::setTranslation(const std::string& translation)
	{
		mTranslation = translation;
	}

	inline LocalizationMessage::TRANSLATION_TYPE LocalizationMessage::getTranslationType() const
	{
		return mTranslationType;
	}

	inline void LocalizationMessage::setTranslationType(TRANSLATION_TYPE translationType)
	{
		mTranslationType = translationType;
	}

	inline const std::string& LocalizationMessage::translate() const
	{
		return (mTranslationType == TRANSLATION_UNFINISHED) ? mSource : mTranslation;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline LocalizationMessage::LocalizationMessage(LocalizationContext& localizationContext) :
		mLocalizationContext(&localizationContext),
		mTranslationType(TRANSLATION_UNFINISHED)
	{
		// Nothing to do in here
	}

	inline LocalizationMessage::~LocalizationMessage()
	{
		// Set empty string to unregister this localization message
		setSource("");
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
