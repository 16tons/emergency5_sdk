// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/StringHash.h"
#include "qsf/localization/LocalizationMessage.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LocalizationSystem::LocalizationSystem() :
		mRedirectionDepth(0)
	{
		// Nothing to do in here
	}

	inline LocalizationSystem::~LocalizationSystem()
	{
		// Nothing to do in here
	}

	inline const std::vector<std::string>& LocalizationSystem::getLanguageDirectories() const
	{
		return mLanguageDirectories;
	}

	inline const std::string& LocalizationSystem::getLanguage() const
	{
		return mLanguage;
	}

	inline const std::vector<Translator*>& LocalizationSystem::getTranslators() const
	{
		return mTranslators;
	}

	inline const std::string& LocalizationSystem::translateById(uint32 id) const
	{
		static const std::string EMPTY_STRING;
		const MessageMap::const_iterator iterator = mMessageMap.find(id);
		return (iterator != mMessageMap.cend()) ? resolveIndirection(iterator->second->translate()) : EMPTY_STRING;
	}

	inline const std::string& LocalizationSystem::translateByString(const std::string& source) const
	{
		const MessageMap::const_iterator iterator = mMessageMap.find(StringHash(source.c_str()));
		return (iterator != mMessageMap.cend()) ? resolveIndirection(iterator->second->translate()) : source;
	}

	inline const std::string& LocalizationSystem::translateByString(const std::string& source, bool& wasTranslated) const
	{
		const MessageMap::const_iterator iterator = mMessageMap.find(StringHash(source.c_str()));
		if (iterator != mMessageMap.cend())
		{
			wasTranslated = true;
			return resolveIndirection(iterator->second->translate());
		}
		else
		{
			wasTranslated = false;
			return source;
		}
	}

	inline const std::string& LocalizationSystem::translateByStringWithFallback(const std::string& source, const std::string& fallbackSource) const
	{
		const MessageMap::const_iterator iterator = mMessageMap.find(StringHash(source.c_str()));
		return (iterator != mMessageMap.cend()) ? resolveIndirection(iterator->second->translate()) : translateByString(fallbackSource);
	}

	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* LocalizationSystem::getName() const
	{
		return "Localization system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
