// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"

#include <qsf/message/MessageProxy.h>


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
	*    Game logic for gangster type "ME200Fanatic"
	*/
	class EM5_API_EXPORT GangsterFanaticArsonistLogic : public GangsterBaseLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::GangsterFanaticArsonistLogic" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		GangsterFanaticArsonistLogic();

		inline void setIgniteTimeStake(const qsf::Time& time)  { mIgniteTimeStake = time; }
		inline void setIgniteTimeObject(const qsf::Time& time) { mIgniteTimeObject = time; }
		inline void setCheeringTime(const qsf::Time& time) { mCheeringTime = time; }

		void setArsonistTargets(std::vector<std::string>& targetEventTags);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GangsterBaseLogic methods      ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSimulationUpdate(const qsf::JobArguments& jobArguments) override;

		// Note: Don't simply save the list because there is no possibility to make sure that the entity references stay valid after the method returns
		virtual void onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits) override;

		// Notifies us when no more police units are within range
		virtual void onNoUnitsSpotted() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		qsf::Entity* getNextTarget();
		void onTargetWasIgnited(const qsf::MessageParameters& parameters);
		bool isTargetInvalid() const;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum State
		{
			STATE_UNDEFINED = 0,
			STATE_ANGRY_PROTEST,		///< Play some Animations
			STATE_ANGRY_PROTEST_END,
			STATE_GO_TO_NEXT_TARGET,	///< Go to the next target in list
			STATE_SET_TARGET_ON_FIRE,	///< Set that target on fire
			STATE_SET_TARGET_ON_FIRE_END,
			STATE_WATCH_AND_CHEER,		///< Step away from the building and cheer
			STATE_WATCH_AND_CHEER_END,
			STATE_DO_NOTHING
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64					 mCurrentTargetId;
		qsf::Time				 mIgniteTimeStake;
		qsf::Time				 mIgniteTimeObject;
		qsf::Time				 mCheeringTime;
		State					 mCurrentState;
		std::vector<std::string> mTargetEventTags;
		int						 mCurrentTargetIndex;
		qsf::Transform			 mStartingTransform;
		qsf::MessageProxy		 mIgniteTargetMessageProxy;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::GangsterFanaticArsonistLogic)
