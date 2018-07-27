// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <em5/freeplay/event/FreeplayEvent.h>

#include <qsf/message/MessageProxy.h>
#include <qsf/time/Time.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Sample for a Variant of the EMERGENCY 5 freeplay event "medical emergency"
	*	 It contains the same content like the EM5 event, but the user has an extra task to do: start the "cheering" action with a unit
	*/
	class MedicalCheerEvent : public em5::FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "user::MedicalCheerEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MedicalCheerEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MedicalCheerEvent();

		/**
		*  @brief
		*    Get the ID of the target person
		*/
		uint64 getTargetPersonId() const;

		/**
		*  @brief
		*    Set the target entity
		*/
		void setTargetPerson(qsf::Entity& entity);

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
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(EventLayer, std::string, mEventLayer);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(RescueWithHelicopter, bool, mRescueWithHelicopter);
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
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, em5::eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual void hintCallback(em5::Observer& hintObserver) override;
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

		void onTreatedPerson(const qsf::MessageParameters& parameters);


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
		qsf::MessageProxy	 mTreatedPersonMessageProxy;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(user::MedicalCheerEvent)
