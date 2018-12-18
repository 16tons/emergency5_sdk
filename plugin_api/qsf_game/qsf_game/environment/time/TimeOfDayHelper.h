// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/time/Time.h>

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


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
		*    Time of day static helper class
		*/
		class QSF_GAME_API_EXPORT TimeOfDayHelper
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Conversion functions
			*/
			static inline glm::vec2 getGlmVec2FromQsfTime(const Time& time);

			static inline Time getQsfTimeFromGlmVec2(const glm::vec2& time);
			static Time getQsfTimeFromGlmVec2String(const std::string& timeString);

			// Is time between start time and end time
			static inline bool isInBetweenDayTime(Time time, Time timeStart, Time timeEnd);
			static inline bool isInsideInterval(Time time, Time minTime, Time maxTime);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/environment/time/TimeOfDayHelper-inl.h"
