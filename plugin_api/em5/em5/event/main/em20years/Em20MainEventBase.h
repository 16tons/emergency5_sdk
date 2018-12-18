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
	*    EMERGENCY 20 years edition mission base class
	*/
	class Em20MainEventBase : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		Em20MainEventBase(const std::string& eventLayerName);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Em20MainEventBase();


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onRun() override;
		virtual bool onSuccess(EventResult& eventResult) override;
		virtual bool onFailure(EventResult& eventResult) override;
		virtual uint32 calculatePointGain(EventResult& eventResult) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::MainEvent methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onIntroStart(const qsf::MessageParameters& parameters) override;
		virtual void onPrologueFinished(const qsf::MessageParameters& parameters) override;
		virtual void showSupervisorMessageForFinalEvaluation(uint32 pointGain) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::Em20MainEventBase methods      ]
	//[-------------------------------------------------------]
	protected:
		virtual void onUnitSetup(qsf::Entity& entity, uint32 unitTypeId) {}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void startIngameCutscene(const std::string& sequenceLocalAssetName);
		void setMusicLevel(uint32 musicLevel);
		void stopCurrentHints();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onUnitSetupInternal(const qsf::MessageParameters& parameters);
		void onEntitySpawn(const qsf::MessageParameters& parameters);
		void onAllowGameSpeedChanges(const qsf::JobArguments& jobArguments);
		void updateTrackInjureds(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Connected to CAMP
		uint32 mFailLimitDeadPersons;
		uint32 mFailLimitLostUnits;

		// Internal
		bool mDisableVehicleTowing;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Internal
		qsf::MessageProxy mUnitSetupMessageProxy;
		qsf::MessageProxy mEntitySpawnMessageProxy;

		// Jobs
		qsf::JobProxy mAllowGameSpeedChangesJobProxy;
		qsf::JobProxy mTrackInjuredsJobProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::Em20MainEventBase)
