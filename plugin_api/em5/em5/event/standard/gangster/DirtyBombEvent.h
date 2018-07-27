// Copyright (C) 2012-2014 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/event/standard/gangster/GangsterEvent.h"

#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class JobArguments;
	class MessageProxy;
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
	*    EMERGENCY 5 freeplay event class for "dirty bomb
	*/
	class EM5_API_EXPORT DirtyBombEvent : public GangsterEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::DirtyBombEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		DirtyBombEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DirtyBombEvent();

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(BombTargetId, uint64, mBombTargetId);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(ExplosionRadius, float, mExplosionRadius);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(ContaminationRadius, float, mContaminationRadius);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TerroristSeekTime, qsf::Time, mTerroristSeekTime);

		std::vector<uint64>& getTerroristIds();


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
		void explodeDirtyBomb(qsf::Entity& entity);

		void onTerroristSeekFail(const qsf::JobArguments& jobArguments);
		void onTerroristFound(const qsf::MessageParameters& parameters);

		void playHint02();
		void playHint04();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float				mExplosionRadius;
		float				mContaminationRadius;
		uint64				mBombTargetId;
		std::vector<uint64>	mTerroristIds;
		qsf::Time			mTerroristSeekTime;

		std::vector<qsf::MessageProxy>	mTerroristFoundMessageProxies;
		qsf::JobProxy					mTerroristSeekTimer;

		qsf::Time			mHint01WaitTime;	// Waiting time for terrorist seek hint
		qsf::Time			mHint03WaitTime;
		float				mHint04LifeEnergy;
		bool				mHint02Triggered;
		bool				mHint04Triggered;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::DirtyBombEvent)
