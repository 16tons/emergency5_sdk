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
	*    EMERGENCY 5 freeplay event "medical emergency"
	*/
	class EM5_API_EXPORT MedicalEmergencyEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MedicalEmergencyEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MedicalEmergencyEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MedicalEmergencyEvent();

		/**
		*  @brief
		*    Get the ID of the target person
		*/
		uint64 getTargetPersonId() const;

		/**
		*  @brief
		*    Set the target entity
		*/
		void setTargetPerson(const qsf::Entity& entity);

		/**
		*  @brief
		*    Get name of the injury the first target receives
		*/
		const std::string& getInjuryName() const;

		/**
		*  @brief
		*    Set name of the injury zhe first target receives
		*/
		void setInjuryName(const std::string& injuryName);

		/**
		*  @brief
		*    Get the delay the event waits before its getting active
		*/
		float getRunningDelay() const;

		/**
		*  @brief
		*    Set seconds of the delay the event waits before its getting active
		*/
		void setRunningDelay(float runningDelay);

		/**
		*   @brief
		*    Get or set the event layer
		*/
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(EventLayer, const std::string&, mEventLayer);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(RescueWithHelicopter, bool, mRescueWithHelicopter);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(SpawnPointEntityId, uint64, mSpawnPointEntityId);	// TODO(fw): Not referenced anywhere (yet)?
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(SecondEventTag, const std::string&, mSecondEventTag);


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
		void startObjectives(const qsf::Entity& targetEntity);
		void startHintObservers(const qsf::Entity& targetEntity);

		/**
		*  @brief
		*    Check if the current circumstances are still good for the event
		*/
		bool checkCandidate(qsf::Entity* targetEntity);

		/**
		*  @brief
		*    Callback for a delayed start, to set this event running
		*/
		void delayedStartup(const qsf::MessageParameters& parameters);

		void activateLayer(const std::string& layerName);
		void deactivateLayer(const std::string& layerName);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		uint64				 mInjuredEntityId;			///< ID of the injured person
		uint64				 mSpawnPointEntityId;		///< ID of the spawn point
		qsf::NamedIdentifier mInjuryName;				///< Name and ID of the injury the person receives at start of the event
		qsf::Time			 mRunningDelay;				///< Delay after which the event is set to running state
		std::string			 mHintLowLifeEnergyName;
		std::string			 mHintLowHealthEnergyName;
		std::string			 mEventLayer;
		bool				 mRescueWithHelicopter;
		std::string			 mSecondEventTag;			///< Event tag of additional persons to be injured at startup (with the same injury); can also mark spawn points to create fresh victims

		// Internal
		qsf::MessageProxy	 mMessageProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MedicalEmergencyEvent)
