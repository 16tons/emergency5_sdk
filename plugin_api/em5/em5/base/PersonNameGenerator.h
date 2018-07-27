// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <vector>
#include <string>


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
	*    EMERGENCY 5 person names list
	*/
	class PersonNameGenerator
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static std::vector<std::string>& getFirstNames();
		static std::vector<std::string>& getLastNames();
		static std::vector<std::string>& getFirstNamesHistoric();
		static std::vector<std::string>& getLastNamesHistoric();

		static std::string getPersonName();
		static std::string getPersonNameHistoric();
		static std::string getWarblerFeedAuthor();


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static void gatherTranslationNames(std::vector<std::string>& result, const std::string& nameTag);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
