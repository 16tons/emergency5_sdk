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
	*    EMERGENCY 5 freeplay event "buried person"
	*/
	class EM5_API_EXPORT BuriedPersonEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::BuriedPersonEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BuriedPersonEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BuriedPersonEvent();

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
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(IsHidden, bool, mIsHidden);


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

		void activateLayer(const std::string& layerName);
		void deactivateLayer(const std::string& layerName);

		void onBuriedFound(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64				mInjuredEntityId;		///< ID of the injured person
		uint64				mBuryObjectEntityId;
		std::string			mInjuryName;			///< Name of the injury the person receives at start of the event
		qsf::Time			mRunningDelay;			///< Delay after which the event is set to running state
		qsf::MessageProxy	mBuriedFoundMessageProxy;
		std::string			mHintLowLifeEnergyName;
		std::string			mHintLowHealthEnergyName;
		std::string			mEventLayer;
		bool				mIsHidden;
		qsf::Entity*		mIconEntity;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::BuriedPersonEvent)
