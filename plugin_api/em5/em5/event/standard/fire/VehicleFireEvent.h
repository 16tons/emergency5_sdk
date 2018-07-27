// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include <qsf/base/PropertyHelper.h>
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
	*    EMERGENCY 5 freeplay event "vehicle fire"
	*/
	class EM5_API_EXPORT VehicleFireEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::VehicleFireEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		VehicleFireEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~VehicleFireEvent();

		/**
		*  @brief
		*    Get the ID of the target entity
		*/
		uint64 getTargetEntityId() const;

		/**
		*  @brief
		*    Set the target entity
		*/
		void setTargetEntity(const qsf::Entity& entity);

		/**
		*  @brief
		*    Get number persons that have to die so that the event fails
		*/
		uint32 getDeadPersonsToFail() const;

		/**
		*  @brief
		*    Set number persons that have to die so that the event fails
		*/
		void setDeadPersonsToFail(uint32 personsToFail);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PersonsDamage, bool, mPersonsDamage)


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;
		virtual void hintCallback(Observer& hintObserver) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startObjectives(const qsf::Entity& targetEntity);
		void startHintObservers(const qsf::Entity& targetEntity);

		/**
		*  @brief
		*    React to the signal action
		*/
		void onSignal(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Clear the action plan and stop the vehicle
		*/
		void stopTheCar(qsf::Entity& targetEntity);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64						mTargetEntityId;	///< ID of the inflamed building
		uint32						mDeadPersonsToFail;	///< Number of persons that have to die until the player has failed this event
		bool						mPersonsDamage;		///< Does the event effects perons
		qsf::MessageConfiguration	mMessage;			///< Signal message
		qsf::MessageProxy			mMessageProxy;		///< Message proxy, that listens if the car did stop


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::VehicleFireEvent)
