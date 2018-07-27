// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include <qsf/message/MessageProxy.h>
#include <qsf/time/Time.h>


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
	*    Emergency 5 freeplay event class for "Terror Attack"
	*/
	class EM5_API_EXPORT TerrorAttackEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::TerrorAttackEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		TerrorAttackEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TerrorAttackEvent();

		void addTerrorist(qsf::Entity& gangster, qsf::Entity& bombSpot);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(BombTime, qsf::Time, mBombTime);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onFailure(EventResult& eventResult) override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;
		virtual bool addEntityToEvent( qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason ) override;
		virtual void hintCallback( Observer& hintObserver ) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum TerroristState
		{
			RUNNING_TO_BOMB_SPOT,
			PERPETRATING,
			RUNNING_AMOK,
			RESCUE
		};

		struct Terrorist
		{
			uint64 mGangsterId;
			uint64 mBombSpotId;
			uint64 mBombId;
			TerroristState mState;
			bool mBombExploded;		///< Keep track of whether the bomb exploded
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startHintObservers();
		bool checkIfIsHealthy(const qsf::Entity& victim) const;
		bool checkIfReachedBombSpot(qsf::Entity& gangster, qsf::Entity& victim) const;
		void setupGangster(Terrorist& terrorist);
		void onExplode(const qsf::MessageParameters& parameters, Terrorist& terrorist);
		void removeTerrorist(Terrorist& terrorist);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<Terrorist> mTerrorists;
		qsf::Time			   mBombTime;
		qsf::MessageProxy	   mMessageProxy;
		int					   mBombsPlaced;
		qsf::Time			   mBombDeletionCountdown;	///< After what time shall the bomb be deleted after it has exploded?


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::TerrorAttackEvent)
