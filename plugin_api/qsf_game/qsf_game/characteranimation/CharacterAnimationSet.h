// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"
#include "qsf_game/characteranimation/CharacterAnimationDefinition.h"

#include <qsf/asset/AssetProxy.h>
#include <qsf/base/StringHash.h>
#include <qsf/time/Time.h>

#include <boost/property_tree/ptree_fwd.hpp>

#include <string>
#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class CharacterAnimationSetManager;
	}
}


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
		*    Character animation set class
		*/
		class QSF_GAME_API_EXPORT CharacterAnimationSet
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class CharacterAnimationSetManager;


		//[-------------------------------------------------------]
		//[ Public declarations                                   ]
		//[-------------------------------------------------------]
		public:
			struct AnimationData
			{
				AssetProxy									mSkeleton;				///< The animation asset to use
				AssetProxy									mSecondSkeleton;		///< The second animation asset to use
				float										mMinimumSpeed;			///< The minimal speed at which this animation should be played
				float										mDefaultSpeed;			///< The speed for which the animation was designed
				float										mDefaultSecondSpeed;	///< The speed for which the second animation was designed
				float										mFullSecondSpeedDiff;	///< The speed difference (to default animation speed of 1.0) to use when full second animation is active
				Time										mBlendTime;				///< Blend time when to this animation is switched
				CharacterAnimationDefinition::CharacterPose mCharacterPose;			///< The character pose for the animation
			};

			typedef std::vector<AnimationData> AnimationDataList;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			CharacterAnimationSet();
			~CharacterAnimationSet();

			/**
			*  @brief
			*    Get the name of the set
			*/
			const std::string& getName() const;

			const AnimationDataList& getAnimationList() const;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void loadSetDataFromPTree(const std::string& name, const boost::property_tree::ptree& basePTree, const CharacterAnimationSetManager& manager);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::string			mName;			///< Name of the animation set
			AnimationDataList	mAnimationList;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
