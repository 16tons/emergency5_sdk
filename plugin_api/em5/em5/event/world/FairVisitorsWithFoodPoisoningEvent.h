// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"


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
	*    Emergency 5 freeplay event "fair visitors with food poisoning" 13.2
	*/
	class EM5_API_EXPORT FairVisitorsWithFoodPoisoningEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::FairVisitorsWithFoodPoisoningEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FairVisitorsWithFoodPoisoningEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FairVisitorsWithFoodPoisoningEvent();

		/**
		*  @brief
		*    Get number persons that have to die so that the event fails
		*/
		uint32 getDeadPersonsToFail() const;

		/**
		*  @brief
		*    Set number persons that have to die so that the event fails
		*/
		void setDeadPersonsToFail(uint32 personsToFail);

		uint32 getVictimCount() const;
		void setVictimCount(uint32 val);
		uint64 getBuildingId() const;
		void setBuildingId(uint64 val);
		float getHint08LifeEnergy() const;
		void setHint08LifeEnergy(float val);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onFailure(EventResult& eventResult) override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual void hintCallback(Observer& hintObserver) override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startObjectives();
		void startHintObservers();
		qsf::Entity* createVictim();
		qsf::Entity* createSalesman();
		bool checkIfReachedShop(qsf::Entity& entity);
		void startSalesmanEscape();
		uint32 getFailReasonObjectiveTypeId(const ObjectiveList& objectives);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum VictimState
		{
			GO_TO_SHOP,
			EATING,
			GO_AWAY,
			ILL
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32				mDeadPersonsToFail;			///< Number of persons that have to die until the player has failed this event
		uint64				mBuildingId;
		uint64				mSalesman;
		std::vector<uint64>	mVictimIds;
		std::vector<uint32>	mVictimStates;
		std::vector<float>	mVictimWalkingAwayTime;
		std::vector<float>	mVictimWalkingAwayTimeThreshold;
		uint32				mVictimCount;
		uint32				mVictimsEat;
		uint32				mVictimsIll;
		bool				mIsSalesmanEscaping;
		float				mHint08LifeEnergy;
		bool				mCanVictimsBecomeIll;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::FairVisitorsWithFoodPoisoningEvent)
