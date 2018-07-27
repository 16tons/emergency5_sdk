// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <boost/noncopyable.hpp>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
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
	*    Localization message class
	*
	*  @remarks
	*    A localization message is a concrete text.
	*/
	class QSF_API_EXPORT LocalizationMessage : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class LocalizationContext;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Translation type
		*/
		enum TRANSLATION_TYPE
		{
			TRANSLATION_FINISHED,	///< Translation is finished
			TRANSLATION_UNFINISHED,	///< Translation is unfinished
			TRANSLATION_OBSOLETE	///< Translation is obsolete
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the owner localization context
		*
		*  @return
		*    The owner localization context, do not destroy the instance
		*/
		inline LocalizationContext& getLocalizationContext() const;

		/**
		*  @brief
		*    Return the source of the message
		*
		*  @return
		*    The UTF-8 source of the message
		*/
		inline const std::string& getSource() const;

		/**
		*  @brief
		*    Set the source of the message
		*
		*  @param[in] source
		*    The UTF-8 source of the message, an empty string can't be translated
		*/
		void setSource(const std::string& source);

		/**
		*  @brief
		*    Return the translator comment of the message
		*
		*  @return
		*    The UTF-8 translator comment of the message
		*/
		inline const std::string& getTranslatorComment() const;

		/**
		*  @brief
		*    Set the translator comment of the message
		*
		*  @param[in] translatorComment
		*    The UTF-8 translator comment of the message
		*/
		inline void setTranslatorComment(const std::string& translatorComment);

		/**
		*  @brief
		*    Return the translation of the message
		*
		*  @return
		*    The UTF-8 translation of the message
		*/
		inline const std::string& getTranslation() const;

		/**
		*  @brief
		*    Set the translation of the message
		*
		*  @param[in] translation
		*    The UTF-8 translation of the message
		*/
		inline void setTranslation(const std::string& translation);

		/**
		*  @brief
		*    Return the translation type of the message
		*
		*  @return
		*    The translation type of the message
		*/
		inline TRANSLATION_TYPE getTranslationType() const;

		/**
		*  @brief
		*    Set the translation type of the message
		*
		*  @param[in] translationType
		*    The translation type of the message
		*/
		inline void setTranslationType(TRANSLATION_TYPE translationType);

		/**
		*  @brief
		*    Translate the message
		*
		*  @return
		*    The UTF-8 translation of the message, returns the source of the message in case the translation type is unfinished
		*/
		inline const std::string& translate() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] localizationContext
		*    Owner localization context, must stay valid as long as this localization message exists, do not destroy the instance
		*/
		inline explicit LocalizationMessage(LocalizationContext& localizationContext);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~LocalizationMessage();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		LocalizationContext* mLocalizationContext;	///< Owner localization context, always valid, do not destroy the instance
		std::string			 mSource;				///< UTF-8 source of the message
		std::string			 mTranslatorComment;	///< UTF-8 translator comment of the message
		std::string			 mTranslation;			///< UTF-8 translation of the message
		TRANSLATION_TYPE	 mTranslationType;		///< Translation type of the message


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/localization/LocalizationMessage-inl.h"
