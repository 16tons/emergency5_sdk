// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include <qsf/base/WeakPtr.h>
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
	*    EMERGENCY 5 freeplay event "defect ebox"
	*/
	class EM5_API_EXPORT DefectEboxEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::DefectEboxEvent" unique freeplay event ID

		enum EboxState
		{
			STATE_INTACT,
			STATE_DEFECT,
			STATE_BURNING,
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		DefectEboxEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DefectEboxEvent();

		/**
		*  @brief
		*    Set the target entity
		*/
		void setEboxEntity(qsf::Entity& entity);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(GapeChance, float, mGapeChance);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(ReactionRadius, float, mReactionRadius);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(InjuredPersonsToStartFire, uint16, mInjuredPersonsToStartFire);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(FireStartTime, qsf::Time, mFireStartTime);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(RepairTime, qsf::Time, mRepairTime);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
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
		void startObjectives(const qsf::Entity& targetEntity);
		void onRepaired(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::WeakPtr<qsf::Entity> mEboxEntity;					///< The ebox entity
		float					  mGapeChance;					///< Chance that a person is gaping and injured by electric shock
		float					  mReactionRadius;				///< Radius for person reaction
		uint16					  mInjuredPersonsToStartFire;	///< Number of injured persons to start fire on ebox
		qsf::Time				  mFireStartTime;				///< Time to wait until the fire starts
		qsf::Time				  mRepairTime;					///< Time to repair the ebox

		// Intern
		qsf::Time		  mTimeSinceStart;						///< Time counter to save the duration of the event
		qsf::MessageProxy mRepairMessageProxy;					///< Listens to repair messages
		EboxState		  mCurrentEboxState;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::DefectEboxEvent)
