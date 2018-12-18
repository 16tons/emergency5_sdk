// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/asset/AssetProxy.h>
#include <qsf/base/StringHash.h>
#include <qsf/reflection/CampClass.h>
#include <qsf/time/Time.h>

#include <boost/property_tree/ptree_fwd.hpp>

#include <string>
#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Character animation definition class
		*/
		class QSF_GAME_API_EXPORT CharacterAnimationDefinition
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class CharacterAnimationSetManager;


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			enum class CharacterPose
			{
				UNKNOWN,
				LYING,
				KNEELING,
				STANDING
			};


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			CharacterAnimationDefinition();
			~CharacterAnimationDefinition();

			/**
			*  @brief
			*    Get the name of the definition
			*/
			const std::string& getName() const;
			const AssetProxy& getSkeleton() const;
			const AssetProxy& getSecondSkeleton() const;
			float getMinimumSpeed() const;
			float getDefaultSpeed() const;
			float getDefaultSecondSpeed() const;
			float getFullSecondSpeedDiff() const;
			const Time& getBlendTime() const;
			CharacterPose getCharacterPose() const;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void loadDefinitionDataFromPTree(const std::string& name, const boost::property_tree::ptree& basePTree);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::string		mName;					///< Name of the animation definition
			AssetProxy		mSkeleton;				///< The animation asset to use
			AssetProxy		mSecondSkeleton;		///< The second animation asset to use
			float			mMinimumSpeed;			///< The minimal speed at which this animation should be played
			float			mDefaultSpeed;			///< The speed for which the animation was designed
			float			mDefaultSecondSpeed;	///< The speed for which the second animation was designed
			float			mFullSecondSpeedDiff;	///< The speed difference (to default animation speed of 1.0) to use when full second animation is active
			Time			mBlendTime;				///< Blend time when to this animation is switched
			CharacterPose	mCharacterPose;			///< The character pose for the animation


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::CharacterAnimationDefinition::CharacterPose)
