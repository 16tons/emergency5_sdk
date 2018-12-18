// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/application/VersionSwitch.h"

#include <qsf_game/security/ContentAuthentication.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Content authentication class
	*/
	class ContentAuthentication : protected qsf::game::ContentAuthentication
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum ChecksumCategory
		{
			CHECKSUM_EM5,
			CHECKSUM_EM5_DELUXE,
			CHECKSUM_EM2016,
			CHECKSUM_EM2017
		};

		static const qsf::NamedIdentifier CHECKSUM_NAME_EM5;
		static const qsf::NamedIdentifier CHECKSUM_NAME_EM5_DELUXE;
		static const qsf::NamedIdentifier CHECKSUM_NAME_EM2016;
		static const qsf::NamedIdentifier CHECKSUM_NAME_EM2017;

		typedef boost::container::flat_map<qsf::NamedIdentifier, uint64> ChecksumsMap;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the internal checksum name by its category
		*/
		static const qsf::NamedIdentifier& getChecksumNameByCategory(ChecksumCategory category);

		static bool compareChecksumsMaps(const ChecksumsMap& checksumsMap1, const ChecksumsMap& checksumsMap2, ChecksumCategory upToCategory, bool logResults = false);

		static ChecksumCategory getChecksumCategoryForEdition(VersionSwitch::Edition edition);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ContentAuthentication();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ContentAuthentication();

		/**
		*  @brief
		*    Start checking the content
		*/
		void start();

		/**
		*  @brief
		*    Return a specific checksum by its category
		*/
		Checksum getChecksum(ChecksumCategory category);

		// Make protected inherited method public
		using qsf::game::ContentAuthentication::setCheckFileModificationDates;

		void buildChecksumsMap(ChecksumsMap& checksumsMap);

		// Make this method public
		using qsf::game::ContentAuthentication::debugLogChecksums;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
