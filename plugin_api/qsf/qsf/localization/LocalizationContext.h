// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/Context.h"

#include <boost/container/flat_map.hpp>

#include <vector>
#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Translator;
	class LocalizationMessage;
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
	*    Localization context class
	*
	*  @remarks
	*    A localization context is a collection of localization message instances which is part of a translator.
	*/
	class QSF_API_EXPORT LocalizationContext : public Context
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class Translator;
		friend class LocalizationMessage;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the owner translator
		*
		*  @return
		*    The owner translator, do not destroy the instance
		*/
		inline Translator& getTranslator() const;

		/**
		*  @brief
		*    Return the name of the context
		*
		*  @return
		*    The UTF-8 name of the context
		*/
		inline const std::string& getName() const;

		/**
		*  @brief
		*    Set the name of the context
		*
		*  @param[in] name
		*    The UTF-8 name of the context
		*/
		void setName(const std::string& name);

		/**
		*  @brief
		*    Create a new localization message instance
		*
		*  @return
		*    The created localization message instance, null pointer on error, don't destroy the returned instance
		*/
		LocalizationMessage* createLocalizationMessage();

		/**
		*  @brief
		*    Destroy a given localization message instance
		*
		*  @param[in] localizationMessage
		*    The localization message instance to destroy, after this method call this instance is no longer valid
		*/
		void destroyLocalizationMessage(LocalizationMessage& localizationMessage);

		/**
		*  @brief
		*    Destroy all localization messages within this translator
		*/
		void destroyAllLocalizationMessages();

		/**
		*  @brief
		*    Return the list of currently loaded localization messages
		*
		*  @return
		*    Reference to the internal list of currently loaded localization messages, do not manipulate the list or destroy the localization messages
		*/
		inline const std::vector<LocalizationMessage*>& getLocalizationMessages() const;

		/**
		*  @brief
		*    Translate a given message string
		*
		*  @param[in] string
		*    Source of the message to translate
		*  @param[out] wasTranslated
		*    "true" if a translation was found, "false" if not and the output string is equal to the input
		*
		*  @return
		*    The UTF-8 translation of the given UTF-8 message, given UTF-8 message on error
		*/
		const std::string& translateByString(const std::string& source, bool& wasTranslated) const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] translator
		*    Owner translator, must stay valid as long as this localization context exists, do not destroy the instance
		*/
		inline explicit LocalizationContext(Translator& translator);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~LocalizationContext();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef boost::container::flat_map<std::string, LocalizationMessage*> MessageMap;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Translator*							mTranslator;			///< Owner translator, always valid, do not destroy the instance
		std::string							mName;					///< UTF-8 context name
		std::vector<LocalizationMessage*>	mLocalizationMessages;	///< Localization message instances, we're responsible for destroying the instances in case we no longer need it
		mutable MessageMap					mMessageMap;			///< Map for direct message access, contains all messages, message source as key, localization message as value


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/localization/LocalizationContext-inl.h"
