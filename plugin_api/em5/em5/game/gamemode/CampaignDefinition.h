// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/player/Player.h"
#include "em5/map/MapIdentifier.h"

#include <boost/property_tree/ptree.hpp>



//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class MainEventIdentifier;
}


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
	*    EMERGENCY 5 campaign definition class
	*/
	class CampaignDefinition : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum PhaseType
		{
			PHASE_TUTORIAL,		///< Tutorial event phase: A single event must be won to continue, but can be skipped (special handling for EM5 original tutorial here!)
			PHASE_MAINEVENT,	///< Main event phase: A single event must be won to continue, counts as main event
			PHASE_SINGLEEVENT,	///< Single event phase: A single event must be won to continue, does not count as main event
			PHASE_FREEPLAY		///< Campaign freeplay phase: Player earns money until all buy options are bought
		};

		struct CampaignPhase
		{
			PhaseType	mPhaseType;				///< Type of this campaign phase
			std::string	mEventName;				///< Name of the event to start, in case a single one should be triggered (for main events, tutorial)
			std::string	mEventPools;			///< Event pools to trigger events from, separated by semicolons
			std::string	mUnitPool;				///< Unit pool to set at phase start
			std::string	mMainEventId;			///< Only for "main event" phase type: Unique identifier of the main event
			std::string mRewardId;				///< The id of the reward the player gets after wining the event
			std::vector<Player::BuyOption> mBuyOptions;	///< Only for "freeplay" phase type: Buy options listing
		};
		typedef std::vector<CampaignPhase> CampaignPhaseArray;

		struct CampaignMap
		{
			MapIdentifier mMapIdentifier;
			CampaignPhaseArray mPhases;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		CampaignDefinition();

		/**
		*  @brief
		*    Destructor
		*/
		~CampaignDefinition();

		/**
		*  @brief
		*    Load the campaign definition from a JSON file
		*/
		void loadCampaignDefinition(const std::string& filename);

		/**
		*  @brief
		*    Return the campaign name
		*/
		const std::string& getName() const;

		/**
		*  @brief
		*    Return the campaign short-name
		*/
		const std::string& getShortName() const;

		/**
		*  @brief
		*    Return the campaign color
		*/
		const qsf::Color4& getColor() const;

		/**
		*  @brief
		*    Return the settings property tree
		*/
		const boost::property_tree::ptree& getSettings() const;

		/**
		*  @brief
		*    Return the campaign map structure for a given map index
		*
		*  @param[in] campaignMapIndex
		*    Map index inside the campaign (not the usual map index!) beginning with 0
		*
		*  @return
		*    The campaign map data, or a null pointer of map index was invalid
		*/
		const CampaignMap* getCampaignMapByIndex(uint32 campaignMapIndex) const;
		const CampaignMap* getCampaignMapByIdentifier(const MapIdentifier& mapIdentifier) const;

		/**
		*  @brief
		*    Return the map identifier for a given campaign map index
		*
		*  @param[in] campaignMapIndex
		*    Map index inside the campaign (not the usual map index!) beginning with 0
		*
		*  @return
		*    The map identifier, default constructed of map index was invalid
		*/
		MapIdentifier getMapIdentifierByCampaignMapIndex(uint32 campaignMapIndex) const;

		/**
		*  @brief
		*    Return the campaign phases for the given map
		*/
		bool getCampaignPhaseIndexForMainEvent(uint32& outCampaignPhase, uint32& outCampaignMapIndex, const MainEventIdentifier& mainEventIdentifier) const;

		/**
		*  @brief
		*    Return list of all campaign phases of the given type
		*/
		void getAllCampaignPhasesOfType(PhaseType phaseType, std::vector<const CampaignPhase*>& outCampaignPhases) const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void loadCampaignDefinitionFromBoostPTree(const boost::property_tree::ptree& campaignPTree);
		bool checkFormatHeader(const boost::property_tree::ptree& rootPTree);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string					mName;			// Campaign name ("EMERGENCY 5 Original Campaign", "EMERGENCY 2016 Campaign")
		std::string					mShortName;		// Campaign short name ("EM5, EM2016" etc.)
		qsf::Color4					mColor;			// Campaign color
		boost::property_tree::ptree mSettings;		// Campaign-specific settings
		std::vector<CampaignMap>	mCampaignMaps;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
