// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/event/standard/gangster/GangsterEvent.h"


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
	*    EMERGENCY 5 freeplay event class for "Rowdy"
	*/
	class EM5_API_EXPORT RowdyEvent : public GangsterEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::RowdyEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		RowdyEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RowdyEvent();

		/**
		*  @brief
		*    Get the ID of the gangster
		*/
		uint64 getGangsterId() const;

		/**
		*  @brief
		*    Set the gangster
		*/
		void setGangster(const qsf::Entity& entity);

		/**
		*  @brief
		*    Get the ID of the building
		*/
		uint64 getBuildingId() const;

		/**
		*  @brief
		*    Set the building
		*/
		void setBuilding(const qsf::Entity& entity);

		void setPeopleToAttack(uint64 peopleToAttack);


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


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startObjectives(qsf::Entity& targetEntity);
		void startHintObservers();
		bool checkIfIsHealthy(const qsf::Entity& victim) const;
		bool checkIfReachedVictim(qsf::Entity& gangster, qsf::Entity& victim) const;
		qsf::Entity* findNewTarget(qsf::Entity& gangster) const;
		qsf::Entity* findNewPoliceTarget(qsf::Entity& gangster) const;
		void attackTarget(qsf::Entity& gangster, qsf::Entity& victim);
		bool checkVictim(qsf::Entity& gangster, qsf::Entity& victim) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64 mBuildingId;
		uint64 mGangsterId;
		uint64 mCurrentVictimId;
		uint64 mAttacksLeft;
		bool mWaiting;
		bool mRowdyHurt;
		bool mVisible;
		bool mDontShootTriggered;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::RowdyEvent)
