// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class VehicleComponent;
}
namespace qsf
{
	class Layer;
	class Map;
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
	*    EMERGENCY 5 freeplay event "traffic accident 01"
	*/
	class EM5_API_EXPORT TrafficAccident01Event : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::TrafficAccident01Event" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static bool canSpawnEntitiesCollisionFree(const qsf::Layer& layer, qsf::Map& map);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		TrafficAccident01Event();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TrafficAccident01Event();

		void init(qsf::Layer* accidentLayer);

		// Range is 0 (no fire) to 1 (always fire)
		float getChanceFire() const;
		void setChanceFire(float chance);

		float getChanceHardInjury() const;
		void setChanceHardInjury(float chance);

		float getHint01WaitTime() const;
		void setHint01WaitTime(float seconds);

		float getHint03LifeEnergy() const;
		void setHint03LifeEnergy(float energy);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onRun() override;
		virtual void onShutdown() override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual void hintCallback(Observer& hintObserver) override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static bool isInjuredOutsideCars(const qsf::Entity& personEntity);
		static bool isBrawler(const qsf::Entity& personEntity);
		static bool isContaminationVehicle(const qsf::Entity& vehicleEntity);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startHintObservers();
		void setupScene(qsf::Layer& accidentLayer);
		void attackTarget(qsf::Entity& attackerEntity, qsf::Entity& targetEntity);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32 mAccidentLayerId;
		bool mHasFire;
		bool mIsBrawling;
		std::vector<uint64>	mLayerVehicles;					// All vehicles
		std::vector<uint64>	mLayerBrawler;					// All persons that are standing outside and brawl
		std::vector<uint64>	mLayerTrappedPersons;			// All persons that are trapped inside vehicles
		std::vector<uint64>	mLayerContaminationVehicles;	// All vehicles that causes contamination (chemical transporter)

		// Settings
		float mChanceFire;
		float mChanceHardInjury;							// Chance that injured persons have hard injuries, Only used for chemical contamination
		float mHint01TimeSeconds;
		float mHint03LifeEnergy;
		qsf::Time mChemicalTransportHint01WaitTime;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::TrafficAccident01Event)
