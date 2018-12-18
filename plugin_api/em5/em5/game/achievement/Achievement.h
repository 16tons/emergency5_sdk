// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_game/achievement/Achievement.h>

#include "em5/application/VersionSwitch.h"


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
	*    EMERGENCY 5 specialized achievement class
	*/
	class EM5_API_EXPORT Achievement : public qsf::game::Achievement
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum AchievementType
		{
			NORMAL_ACHIEVEMENT,
			TITLE_ACHIEVEMENT
		};

		// Represents the game mode where you can unlock the achievement
		enum AchievementPlace
		{
			GENERAL,
			MAINEVENT,
			FREEPLAY,
			CHALLENGE,
			MULTIPLAY_COOP,
			MULTIPLAY_AGAINST
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor for normal achievement
		*/
		Achievement(const std::string& name, const std::string& title, const std::string& description, const std::string& icon, uint32 points,
					AchievementType achievementType, float neededProgress, AchievementPlace achievementPlace, VersionSwitch::Edition achievementEdition);

		/**
		*  @brief
		*    Constructor for rank achievement
		*/
		Achievement(const std::string& name, const std::string& title, const std::string& icon, AchievementType achievementType = NORMAL_ACHIEVEMENT,
					float neededPoints = 0.0f, AchievementPlace achievementPlace = GENERAL, VersionSwitch::Edition achievementEdition = VersionSwitch::Edition::STANDARD);

		/**
		*  @brief
		*    Return the achievement type
		*/
		AchievementType getAchievementType() const;

		/**
		*  @brief
		*    Return the achievement place
		*/
		AchievementPlace getAchievementPlace() const;

		/**
		*  @brief
		*    Return just the icon name (e.g. "em5_rank-medals_01" or "em5_rank-medals_deluxe_01")
		*/
		std::string getIconName() const;

		/**
		*  @brief
		*    Return just the icon name (e.g. "mini-medals_01" or "mini-medals_deluxe_01")
		*/
		std::string getMiniIconName() const;

		/**
		*  @brief
		*    Get application edition associated with this achievement
		*/
		VersionSwitch::Edition getEdition() const;

		/**
		*  @brief
		*    Get campaign index associated with this achievement
		*/
		uint32 getCampaignIndex() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::Achievement methods         ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		std::string mTitle;			///< Title as localizable text ID
		std::string mDescription;	///< Description as localizable text ID
		uint32		mPoints;		///< Achievement point value


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string			   mIcon;				///< Icon name (use getIconName() and getMiniIconName() methods)
		AchievementType		   mAchievementType;	///< This member will be set to NORMAL_ACHIEVEMENT for a normal achievement
		AchievementPlace	   mAchievementPlace;
		VersionSwitch::Edition mAchievementEdition;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
