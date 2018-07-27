// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_ai/navigation/steering/SpeedLimitFunctor.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Clock;
	namespace game
	{
		class WeatherComponent;
		class TimeOfDayComponent;
	}
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
	*    Speed limit functor for EMERGENCY 5
	*
	*  @remarks
	*    Can be used to finetune the speed entities currently used in case this depends on things like weather that are not easily available to the AI.
	*    Another usecase is speed variation that changes over the course of one action dynamically as the movement modes only allow a static functor / limit.
	*/
	class AiSpeedLimitFunctor : public qsf::ai::SpeedLimitFunctor
	{


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct SpeedCacheEntry
		{
			qsf::Time mLastUpdate;
			float mTargetResult;
			float mLastResult;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		AiSpeedLimitFunctor();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ai::SpeedLimitFunctor methods     ]
	//[-------------------------------------------------------]
	public:
		virtual qsf::UnsignedFloat getSpeedFor(const qsf::ai::NavigationComponent& navi, qsf::UnsignedFloat speedLimit) const override;
		virtual qsf::UnsignedFloat getHaltingDistanceBefore(const qsf::ai::NavigationComponent& navi, qsf::Entity& other, qsf::UnsignedFloat defaultDistance) const override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void calculateSpeed(const qsf::ai::NavigationComponent& navigationComponent, float& newSpeedLimit) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const qsf::game::WeatherComponent*   mWeatherComponent;		///< Weather component instance, can be a null pointer, do not destroy the instance
		const qsf::game::TimeOfDayComponent* mTimeOfDayComponent;	///< Time of day component instance, can be a null pointer, do not destroy the instance
		const qsf::Clock&					 mSimulationClock;

		typedef std::unordered_map<uint64, AiSpeedLimitFunctor::SpeedCacheEntry> SpeedCache;
		mutable SpeedCache mSpeedCache;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
