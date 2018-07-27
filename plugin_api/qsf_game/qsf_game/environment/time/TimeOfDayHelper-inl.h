// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		inline glm::vec2 TimeOfDayHelper::getGlmVec2FromQsfTime(const Time& time)
		{
			const float hours = glm::clamp(time.getHours(), 0.0f, 24.0f);
			const float hoursFloor = std::floor(hours);
			return glm::vec2(hoursFloor, (hours - hoursFloor) * 60.0f);
		}

		inline Time TimeOfDayHelper::getQsfTimeFromGlmVec2(const glm::vec2& time)
		{
			return Time::fromHours(std::floor(time.x + 0.5f)) + Time::fromMinutes(time.y);
		}

		inline bool TimeOfDayHelper::isInBetweenDayTime(Time time, Time timeStart, Time timeEnd)
		{
			if (timeStart > timeEnd)
			{
				return (time > timeStart) || (time < timeEnd);
			}
			else
			{
				return (time > timeStart) && (time < timeEnd);
			}
		}

		inline bool TimeOfDayHelper::isInsideInterval(Time time, Time minTime, Time maxTime)
		{
			if (minTime < maxTime)
			{
				return (time > minTime) && (time < maxTime);
			}
			else
			{
				return (time > minTime) || (time < maxTime);
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
