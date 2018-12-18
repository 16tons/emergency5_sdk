// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/main/MainEvent.h"


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
	*    EMERGENCY 5 main event 101 "Cologne"
	*/
	class MainEvent101 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;	// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;	///< "em5MainEvent101" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent101();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent101();


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onSuccess(EventResult& eventResult) override;
		virtual bool onFailure(EventResult& eventResult) override;

		//[-------------------------------------------------------]
		//[ Points evaluation                                     ]
		//[-------------------------------------------------------]
		virtual uint32 calculatePointGain(EventResult& eventResult) override;

		//[-------------------------------------------------------]
		//[ Callbacks from observers                              ]
		//[-------------------------------------------------------]
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual void hintCallback(Observer& hintObserver) override;
		virtual void hintCallbackWithMessage(Observer& hintObserver, const qsf::MessageParameters& parameters) override;

		//[-------------------------------------------------------]
		//[ Miscellaneous                                         ]
		//[-------------------------------------------------------]
		virtual const qsf::Entity* getFocusEntity() override;

		//[-------------------------------------------------------]
		//[ Serialize                                             ]
		//[-------------------------------------------------------]
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::MainEvent methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onIntroStart(const qsf::MessageParameters& parameters) override;
		virtual void onIntroFinished(const qsf::MessageParameters& parameters) override;
		virtual void onPrologueFinished(const qsf::MessageParameters& parameters) override;
		virtual uint32 getEventNumber() override;
		virtual std::string getCamStartEventTag() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Gather all necessary entities for this event
		*/
		void gatherEventEntities();

		/**
		*  @brief
		*    Set all objectives for the start of this event
		*/
		void startObjectives();

		/**
		*  @brief
		*    Set all hint observers for the start of this event
		*/
		void startHintObservers();

		/**
		*  @brief
		*    React to the first finished plague quick test
		*/
		void onFirstQuickTest(const qsf::MessageParameters& parameters);

		void onCoolingFinished(const qsf::MessageParameters& parameters);

		/**
		* @brief
		*    Check if the cooling of the plague cloud was successful and reacts to it
		*/
		void checkCoolPlagueCloud();

		/**
		*  @brief
		*    Create the cool contamination cloud objective and its observer
		*/
		void createCoolContaminationCloudObjectives();

		/**
		*  @brief
		*    Create observer for the infected person fail condition
		*/
		void createInfectedPersonFailCondition();

		/**
		*  @brief
		*    Create general observer for objective success supervisor messages
		*/
		void createObjectiveFinishedObserver();

		/**
		*  @brief
		*    Create general observer for added cooling source energy message
		*/
		void createStartedCoolingObserver();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Constant data
		qsf::Time mHint01WaitTime;
		uint32 mMinNumberInfected;
		uint32 mDeadCiviliansCount;
		uint32 mDeadSquadsCount;
		uint32 mMaxNewInfectedCount;
		uint32 mMinNumberCoolingSources;
		float mNeededCoolingEnergy;

		// Internal data
		qsf::WeakPtr<qsf::Entity>				mPlagueCloud;					///< The entity of the plague cloud
		qsf::WeakPtr<qsf::Entity>				mPlagueCloudEffect;				///< Plague cloud particle effect entity
		std::vector<qsf::WeakPtr<qsf::Entity>>	mInfectedCivilians;				///< The civilians that are infected at the start of the event


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent101)
