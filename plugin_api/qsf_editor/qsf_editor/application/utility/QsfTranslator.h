// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtCore/qtranslator.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class LocalizationSystem;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    QTranslator to QSF localization system
		*/
		class QsfTranslator : public QTranslator
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] localizationSystem
			*    Localization system to use, must stay valid as long as this QSF Qt translator exists
			*/
			explicit QsfTranslator(qsf::LocalizationSystem& localizationSystem);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~QsfTranslator();


		//[-------------------------------------------------------]
		//[ Public virtual QTranslator methods                    ]
		//[-------------------------------------------------------]
		public:
			virtual QString translate(const char* context, const char* sourceText, const char* disambiguation = 0, int n = -1) const override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Called when the language has been changed
			*/
			void onLanguageChanged();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			qsf::LocalizationSystem* mLocalizationSystem;	///< Used QSF localization system, always valid, do not destroy the instance


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
