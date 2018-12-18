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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Layer;
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
	*    EMERGENCY 5 freeplay event "Storm damage"
	*/
	class EM5_API_EXPORT StormDamageEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::StormDamageEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		StormDamageEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~StormDamageEvent();

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TimeBeforeFirstTask, qsf::Time, mTimeBeforeFirstTask);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TimeBetweenTasks, qsf::Time, mTimeBetweenTasks);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(EboxRepairTime, qsf::Time, mEboxRepairTime);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TrappedPersonsInBuildings, uint32, mTrappedPersonsInBuildings);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(DeadPersonsToFail, uint32, mDeadPersonsToFail);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(BurningObjectsToFail, uint32, mBurningObjectsToFail);

		inline uint32 getTasksDummy() const  { return 0; }		// Lazy solution, the CAMP getter is irrelevant anyway
		void setNumberOfTreeOnRoadTasks(uint32 number);
		void setNumberOfTreeOnCarTasks(uint32 number);
		void setNumberOfCarAccidentTasks(uint32 number);
		void setNumberOfLightningFireTasks(uint32 number);
		void setNumberOfTornadoTasks(uint32 number);
		void setNumberOfBrokenElectricPylonTasks(uint32 number);
		void setNumberOfInjuriesTasks(uint32 number);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onFailure(EventResult& eventResult) override;
		virtual bool onSuccess(EventResult& eventResult) override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;
		virtual void hintCallback(Observer& hintObserver) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum TaskType
		{
			TASK_TREE_ON_ROAD,
			TASK_TREE_ON_CAR,
			TASK_CAR_ACCIDENT,
			TASK_LIGHTNING_FIRE,
			TASK_TORNADO,
			TASK_BROKEN_ELECTRIC_PYLON,
			TASK_INJURIES,
			_NUM_TASKS
		};
		struct Task
		{
			TaskType mType;
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startObjectives();
		void setupScene();

		bool tryTriggerNewTask();
		bool startTask(TaskType type);

		qsf::Layer* chooseSubLayer(const std::string& layerNaming, bool usesAddEraseLayers);
		void activateLayer(qsf::Layer& layer);
		void toggleAddEraseSubLayers(qsf::Layer& layer);
		void initBuryingObjectsInLayer(qsf::Layer& layer, const std::string& eventTag, bool makeCuttable);
		void initAccidentCarsInLayer(qsf::Layer& layer);
		void initEboxInLayer(qsf::Layer& layer);
		bool initDamagedBuildingsAssociatedWithLayer(qsf::Layer& layer);
		qsf::Entity* findLightningFireTarget();
		void setParticleEffectEnabled(qsf::Entity& entity, bool enabled);

		void onEboxRepaired(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		qsf::Time mTimeBeforeFirstTask;
		qsf::Time mTimeBetweenTasks;
		qsf::Time mEboxRepairTime;
		uint32	  mTrappedPersonsInBuildings;
		uint32	  mDeadPersonsToFail;
		uint32	  mBurningObjectsToFail;

		// Layers
		qsf::Layer*	mEventLayer;	///< Base layer for this event
		std::vector<std::pair<qsf::Layer*, qsf::Layer*>> mLayersToCleanup;		// First is the layer to cleanup, second is an optional "erase" layer to reactivate

		// Event
		qsf::Time mTimeUntilNextTask;
		bool	  mIsTriggeringNewTasks;

		// Tasks
		std::vector<Task> mTasksStarted;
		std::map<TaskType, uint32> mTaskPool;	///< How many tasks of which kind are left to trigger?

		// Eboxes
		struct EboxInfo
		{
			qsf::WeakPtr<qsf::Entity> mEbox;
			qsf::WeakPtr<qsf::Entity> mEffect;
			qsf::WeakPtr<qsf::Entity> mPylon;
		};
		std::vector<EboxInfo> mEboxInfos;

		// Messages
		qsf::MessageProxy mRepairEboxMessageProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::StormDamageEvent)
