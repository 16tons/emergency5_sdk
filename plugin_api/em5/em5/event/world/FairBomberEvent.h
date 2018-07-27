// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/event/standard/gangster/GangsterEvent.h"

#include <qsf/message/MessageProxy.h>
#include <qsf/base/PropertyHelper.h>


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
	*    Emergency 5 freeplay event "fair bomber" 13.4
	*/
	class EM5_API_EXPORT FairBomberEvent : public GangsterEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::FairBomberEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FairBomberEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FairBomberEvent();

		/**
		*  @brief
		*    Get the ID of the victim
		*/
		uint64 getBombSpotId() const;

		/**
		*  @brief
		*    Set the victim
		*/
		void setBombSpot(qsf::Entity& entity);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(BombTime, qsf::Time, mBombTime);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(BombDefuseTime, qsf::Time, mBombDefuseTime);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(EscapeTime, qsf::Time, mEscapeTime);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(ExplosionRadius, float, mExplosionRadius);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onFailure(EventResult& eventResult);
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startHintObservers();
		bool checkIfIsHealthy(const qsf::Entity& victim) const;
		bool checkIfReachedBombSpot(qsf::Entity& gangster, const qsf::Entity& victim) const;
		void setupGangster(qsf::Entity& gangster, qsf::Entity& bombSpot);
		void escape(qsf::Entity& gangster);
		void onExplode(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum BomberState
		{
			RUNNING_TO_BOMB_SPOT,
			PERPETRATING,
			GUARDING,
			ESCAPING,
			RESCUE
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64			  mBombSpotId;
		uint64			  mBombId;
		qsf::Time		  mBombTime;
		qsf::Time		  mBombDefuseTime;
		qsf::Time		  mEscapeTime;
		BomberState		  mState;
		qsf::MessageProxy mMessageProxy;
		bool			  mBombExploded;			///< Keep track of whether the bomb exploded
		qsf::Time		  mBombDeletionCountdown;	///< After what time shall the bomb be deleted after it has exploded?
		float			  mExplosionRadius;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::FairBomberEvent)
