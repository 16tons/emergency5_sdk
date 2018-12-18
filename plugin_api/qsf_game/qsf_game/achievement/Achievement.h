// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/base/NamedIdentifier.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
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
		*    Achievement class
		*/
		class QSF_GAME_API_EXPORT Achievement : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public data                                           ]
		//[-------------------------------------------------------]
		friend class AchievementSystem;		// May create and destroy achievements


		//[-------------------------------------------------------]
		//[ Public data                                           ]
		//[-------------------------------------------------------]
		public:
			NamedIdentifier mName;
			float			mCurrentProgress;
			float			mNeededProgress;
			bool			mSteamAchievement;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			Achievement(const std::string& name, float neededProgress = 1.0f, bool steamAchievement = true);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~Achievement();

			/**
			*  @brief
			*    Check whether the achievement is completed
			*/
			bool isCompleted() const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::game::Achievement methods         ]
		//[-------------------------------------------------------]
		public:
			virtual void serialize(BinarySerializer& serializer);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
