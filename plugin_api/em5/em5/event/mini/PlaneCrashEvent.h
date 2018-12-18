// Copyright (C) 2012-2018 Promotion Software GmbH


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
	*    Emergency 5 freeplay event "munich pest scientist plane crash" EM5X_MINI04
	*/
	class EM5_API_EXPORT PlaneCrashEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FreeplayEventMetaclasses2;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::PlaneCrashEvent" unique main event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PlaneCrashEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PlaneCrashEvent();


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

		/**
		*  @brief
		*    Activate the scenery layer
		*/
		void activateLayer(const std::string& activate);

		/**
		*  @brief
		*    Deactivate the scenery layer
		*/
		void deactivateLayer(const std::string& deactivate);

		void onWreckFound(const qsf::MessageParameters& parameters);

		void setupPilotVictim();

		void onEboxRepaired(const qsf::MessageParameters& parameters);
		void onAnyoneStartsToCutPlaneWreck(const qsf::MessageParameters& parameters);
		void onPilotDiscovered(const qsf::MessageParameters& parameters);
		void onScientistRescued(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool				mIsEnergyDisabled;	///< Flag indicating whether the ebox has been repaired and there is no longer any current on the electricity lines
		qsf::Time			mWreckCuttingDuration;
		qsf::Time			mHint02WaitTime;
		bool				mHint01Triggered;
		std::string			mElectrocutedFiremanInjury;
		std::string			mScientistInjury;
		std::string			mPilotInjury;
		qsf::MessageProxy	mWreckFoundProxy;
		qsf::MessageProxy	mEboxRepairedProxy;
		qsf::MessageProxy	mStartToCutPlaneWreck;
		qsf::MessageProxy	mPilotDiscoveredProxy;
		qsf::MessageProxy	mScientistRescuedProxy;
		qsf::WeakPtr<qsf::Entity> mWreckUndiscoveredCameraTarget;
		qsf::WeakPtr<qsf::Entity> mDroneTarget;
		qsf::WeakPtr<qsf::Entity> mPlaneWreck;
		qsf::WeakPtr<qsf::Entity> mEbox;
		qsf::WeakPtr<qsf::Entity> mScientist;
		qsf::WeakPtr<qsf::Entity> mScientistInPlane;
		qsf::WeakPtr<qsf::Entity> mPilot;
		qsf::WeakPtr<qsf::Entity> mMinimapPolygonArea;
		qsf::WeakPtr<qsf::Entity> mMinimapPolygonSearchArea;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::PlaneCrashEvent)
