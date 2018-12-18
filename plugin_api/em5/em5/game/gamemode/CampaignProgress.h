// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/gamemode/CampaignDefinition.h"
#include "em5/map/MapIdentifier.h"

#include <qsf/base/NamedIdentifier.h>
#include <qsf/reflection/type/CampQsfTime.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
}
namespace em5
{
	class UnitPool;
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
	*    EMERGENCY 5 campaign progress class
	*
	*  @remarks
	*    This class is used for both save games and automatically created campaign progress.
	*    A campaign progress instance stores the progress data of only a single campaign.
	*/
	class CampaignProgress : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const std::string AUTOSAVE_FILENAME;
		typedef boost::container::flat_set<qsf::NamedIdentifier> NameSet;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static std::string getAbsoluteCampaignFilename(const std::string& filename);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		CampaignProgress();

		/**
		*  @brief
		*    Constructor
		*/
		explicit CampaignProgress(uint32 campaignIndex);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CampaignProgress();

		/**
		*  @brief
		*    Returns the campaign index
		*/
		inline uint32 getCampaignIndex() const;

		/**
		*  @brief
		*    Set the campaign index
		*/
		void setCampaignIndex(uint32 campaignIndex);

		/**
		*  @brief
		*    Returns the current credit amount
		*/
		inline uint32 getCurrentCredits() const;

		/**
		*  @brief
		*    Set the current credit amount
		*/
		inline void setCurrentCredits(uint32 creditsAmount);

		/**
		*  @brief
		*    Returns the current map index inside the campaign
		*/
		inline uint32 getCurrentMapIndex() const;

		/**
		*  @brief
		*    Set the current map index inside the campaign
		*/
		inline void setCurrentMapIndex(uint32 index);

		/**
		*  @brief
		*    Returns the current phase index
		*/
		inline uint32 getCurrentPhaseIndex() const;

		/**
		*  @brief
		*    Set the current phase index
		*/
		inline void setCurrentPhaseIndex(uint32 phaseIndex);

		/**
		*  @brief
		*    Returns the last completed mainevent
		*/
		inline const qsf::NamedIdentifier& getLastMainEvent() const;

		/**
		*  @brief
		*    Set the last completed main event
		*/
		inline void setLastMainEvent(const qsf::NamedIdentifier& mainevent);

		/**
		*  @brief
		*    Returns the last completed standard event index in the current freeplay phase
		*/
		inline int32 getLastStandardEventIndex() const;

		/**
		*  @brief
		*    Set the last completed standard event index in the current freeplay phase
		*/
		inline void setLastStandardEventIndex(int32 eventIndex);

		/**
		*  @brief
		*    Clears the list of already bought units (of a freeplay phase)
		*/
		inline void clearBoughtUnits();

		/**
		*  @brief
		*    Add a bought unit to the list
		*/
		inline void addBoughtUnit(const std::string& unitName);

		/**
		*  @brief
		*    Returns the list of already bought units (of a freeplay phase)
		*/
		inline const std::vector<std::string>& getBoughtUnitList();

		/**
		*  @brief
		*    Clears the list of already bought upgrades (of a freeplay phase)
		*/
		inline void clearBoughtUpgrades();

		/**
		*  @brief
		*    Add a bought upgrade to the list
		*/
		inline void addBoughtUpgrade(const std::string& upgradeName);

		/**
		*  @brief
		*    Returns the list of already bought upgrades
		*/
		inline const std::vector<std::string>& getBoughtUpgradeList();

		/**
		*  @brief
		*    Returns the number of successfully finished freeplay events
		*/
		inline uint32 getFinishedFreeplayEventsCount() const;

		/**
		*  @brief
		*    Set the number of successfully finished freeplay events
		*/
		inline void setFinishedFreeplayEventsCount(uint32 number);

		/**
		*  @brief
		*    Return campaign definition
		*/
		inline const CampaignDefinition& getCampaignDefinition() const;

		/**
		*  @brief
		*    Check if this campaign progress is valid for the running game edition
		*/
		bool isValidForEdition() const;

		//[-------------------------------------------------------]
		//[ Serialization                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Load campaign progress from disk
		*
		*  @param[in] filename
		*    The filename from which the progress should be loaded (must be an absolute path); if empty, the default filename is used
		*/
		bool loadProgress(const std::string& filename);

		/**
		*  @brief
		*    Save campaign progress to disk
		*
		*  @param[in] filename
		*    The filename to which the progress should be saved (must be an absolute path); if empty, the default filename is used
		*/
		bool saveProgress(const std::string& filename);

		/**
		*  @brief
		*    Serialization of the campaign progress
		*/
		void serialize(qsf::BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Campaign progress
		uint32					  mCampaignIndex;			///< Campaign index
		const CampaignDefinition* mCampaignDefinition;		///< Campaign definition; do not destroy the instance

		uint32					 mCurrentCredits;			///< The amount of credits the player currently have
		uint32					 mCurrentMapIndex;			///< Campaign-internal index of the map which is currently active
		uint32					 mCurrentPhaseIndex;		///< The current phase index in which we are currently in
		qsf::NamedIdentifier	 mLastMainEvent;
		int32					 mLastStandardEventIndex;	///< Index in event order of last completed (won or lost) standard event during freeplay phase
		std::vector<std::string> mBoughtUnits;				///< A list of already bought units (This list should only contain entries for a single freeplay phase)
		std::vector<std::string> mBoughtUpgrades;			///< A list of already bought upgrades (This list should only contain entries for a single freeplay phase)
		uint32					 mFinishedFreeplayEvents;	///< The number of already finished freeplay event in the freeplay phase


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/game/gamemode/CampaignProgress-inl.h"
