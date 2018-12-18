// Copyright (C) 2012-2018 Promotion Software GmbH


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
	*    EMERGENCY 5 freeplay event class for "Heist"
	*/
	class EM5_API_EXPORT HeistEvent : public GangsterEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::HeistEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		HeistEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HeistEvent();

		/**
		*  @brief
		*    Get the ID of the victim
		*/
		uint64 getBuildingId() const;

		/**
		*  @brief
		*    Set the victim
		*/
		void setBuilding(qsf::Entity& entity);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(HeistDuration, float, mHeistDuration);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(VictimPool, const std::string&, mVictimPool);


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
		void startHintObservers();
		bool checkIfIsHealthy( qsf::Entity& victim);
		bool checkIfReachedBuilding(qsf::Entity& gangster, qsf::Entity& victim);
		void setupGangster(qsf::Entity& gangster, qsf::Entity& victim);
		void escape(qsf::Entity& gangster);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64 mBuildingId;
		uint64 mVictimId;
		bool mPerpetrating;
		bool mPerpetrated;
		bool mRescue;
		float mHeistDuration;
		std::string mVictimPool;
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
QSF_CAMP_TYPE_NONCOPYABLE(em5::HeistEvent)
