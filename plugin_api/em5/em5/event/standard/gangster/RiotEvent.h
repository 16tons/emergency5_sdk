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
	*    EMERGENCY 5 freeplay event class for "Riot"
	*/
	class EM5_API_EXPORT RiotEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::RiotEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		RiotEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RiotEvent();

		/**
		*  @brief
		*    Get the ID of the gangster
		*/
		std::vector<uint64> const& getGangsterIds() const;

		/**
		*  @brief
		*    Add an initial gangster
		*/
		void addGangster(const qsf::Entity& entity);

		/**
		*  @brief
		*    Get the entity ID of the building
		*/
		uint64 getBuildingId() const;

		/**
		*  @brief
		*    Set the building entity
		*/
		void setBuilding(const qsf::Entity& entity);

		void setPeopleToAttack(uint32 peopleToAttack);
		void setStartGangsters(uint32 startGangsters);
		void setHintToTriggerWhenFleeing(const std::string& hintId);

		/**
		*  @brief
		*    Enables fleeing of gangsters when others are arrested
		*
		*  @remarks
		*    When "remaingGangsters" are left (not arrested and not injured) then the
		*    other gangsters are automatically fleeing. This cannot be resetted.
		*/
		void enableGangsterFleeingBehavior(uint32 remainingGangsters);

		/**
		*  @brief
		*    Sets the preferation of attacking squad units when they are near
		*
		*  @remarks
		*    Enables the variation "Gangsters attack squad members preferably" (true) or
		*    "Gangsters attack squad only when no one else is nearby" (false)
		*/
		void setPreferToAttackSquad(bool gangstersPreferToAttackSquad);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onFailure(EventResult& eventResult) override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual void hintCallback(Observer& hintObserver) override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool checkIfIsHealthy(qsf::Entity& victim);
		bool checkIfReachedVictim(qsf::Entity& gangster, qsf::Entity& victim);
		qsf::Entity* findNewTarget(qsf::Entity& gangster);
		void attackTarget(qsf::Entity& gangster, qsf::Entity& victim);
		void vigilanteAttackTarget(qsf::Entity& gangster, qsf::Entity& victim);
		bool checkVigilante(qsf::Entity& gangster, qsf::Entity& victim, bool onlyCheckClose);
		bool isAttacked(qsf::Entity& gangster);
		bool isFighting(qsf::Entity& gangster);
		bool isAttackingSquad(qsf::Entity& gangsterEntity);
		bool tryAttackSquad(qsf::Entity& gangsterEntity);
		bool tryAttackGangster(qsf::Entity& gangsterEntity);
		qsf::Entity* createRiotGangster();
		void instructGangstersToFlee();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64 mBuildingId;
		std::vector<uint64> mGangsterIds;
		std::vector<uint64> mAttackingSquad;
		boost::container::flat_map<uint64, uint64> mCurrentFights;
		boost::container::flat_map<uint64, Observer*> mArrestObservers;
		std::string mHintToTriggerWhenFleeing;
		uint32 mAttackersLeft;
		uint32 mStartGangsters;
		uint32 mGangstersRemainingBeforeTheyFlee;
		bool mAreGangstersFleeing;						///< Run-time state flag
		bool mDontShootTriggered;
		bool mVigilanteTriggered;
		bool mPreferToAttackSquad;
		bool mRemainingGangstersFleeWhenOthersArrested;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::RiotEvent)
