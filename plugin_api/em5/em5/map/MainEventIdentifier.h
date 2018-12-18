// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
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
	*    EMERGENCY 5 main event identifier class
	*/
	class MainEventIdentifier
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static const std::vector<uint32>& getMainEventNumberArray(uint32 campaignIndex);

		static MainEventIdentifier fromIndex(uint32 campaignIndex, uint32 mainEventIndex);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEventIdentifier();

		/**
		*  @brief
		*    Constructor
		*/
		explicit MainEventIdentifier(uint32 number);

		/**
		*  @brief
		*    Constructor
		*/
		explicit MainEventIdentifier(const std::string& identifier);

		/**
		*  @brief
		*    Destructor
		*/
		~MainEventIdentifier();

		uint32 getNumber() const;
		const std::string& getIdentifier() const;
		std::string getLocalizationString() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32		mNumber;
		std::string mIdentifier;	///< Identifier of the main event, usually "ME01".."ME15"


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
