// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/fire/component/FireReceiverComponent.h"

#include <qsf/base/System.h>
#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/job/JobProxy.h>
#include <qsf/reflection/CampClass.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FireMaterialManager;
	class FireMaterial;
	class FireReceiverComponent;
	class FireSpreadCalculation;
	namespace firesimulation
	{
		struct ComponentData;
	}
}
namespace qsf
{
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
	*    Fire system
	*
	*  @remarks
	*    The fire system loads the fire materials via the FireMaterialManager class and keeps vital variables.
	*    Entities with a FireReceiverComponent or FireComponent are part of the fire system. They take "damage" from fire.
	*    Entities with FireComponent can burn with an particle effect, have different fire states and can get burned.
	*
	*    FireMaterials are a collection of data of the fire. Every FireComponent has one.
	*    Use "EM5_FIRE" as shortcut to the fire system class.
	*/
	class EM5_API_EXPORT FireSystem : public qsf::System
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum class SimulationVisualisationMode : uint8
		{
			SHOW_NONE,			///< No visualisation
			SHOW_GLOBAL,		///< Show for all items
			SHOW_ONLY_SELECTED	///< Show only for selected items
		};

		enum class FireSpreadLinesVisualisationMode : uint8
		{
			SHOW_BOTH,			///< Show receiving and sending lines
			SHOW_RECEIVING,		///< Show receiving lines
			SHOW_SENDING		///< Show sending lines
		};

		typedef boost::container::flat_set<uint64> EntityIdSet;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FireSystem();

		/**
		*  @brief
		*    Destructor
		*
		*  @brief
		*    - Not virtual by intent
		*/
		~FireSystem();

		/** Get fire material by ID */
		const FireMaterial* getFireMaterialByName(const std::string& id) const;

		/**
		*  @brief
		*    Get the fire material manager instance
		*/
		inline FireMaterialManager& getFireMaterialManager() const;

		inline bool getFireSimulationActive() const;
		void setFireSimulationActive(bool fireSimulationEnabled);

		/**
		*  @brief
		*    Gets called on game startup
		*/
		void onGameStartup();

		/**
		*  @brief
		*    Gets called on game shutdown
		*/
		void onGameShutdown();

		/**
		*   @brief
		*    Set a flag to reset the fire spread calculation.
		*/
		inline void resetFireSystem();

		float getFireEnergyMultiplier() const;
		void setFireEnergyMultiplier(float multiplier);

		inline SimulationVisualisationMode getFireRadiansSimulationVisualisationMode() const;
		inline void setFireRadiansSimulationVisualisationMode(SimulationVisualisationMode visualisationMode);

		inline SimulationVisualisationMode getFireSpreadSimulationVisualisationMode() const;
		inline void setFireSpreadSimulationVisualisationMode(SimulationVisualisationMode visualisationMode);

		inline FireSpreadLinesVisualisationMode getFireSpreadLinesVisualisationMode() const;
		inline void setFireSpreadLinesVisualisationMode(FireSpreadLinesVisualisationMode visualisationMode);

		void registerFireReceiver(FireReceiverComponent& fireReceiver);
		void unregisterFireReceiver(FireReceiverComponent& fireReceiver);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup(bool serverMode) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Methods for the fire spread calculation
		void updateSimulation(const qsf::JobArguments& jobArguments);
		void resetDebugDrawRequests(const qsf::Map& map);
		void writeCalculationResultsToEntities(const std::vector<firesimulation::ComponentData>& calculationResult);
		void updateDebugDrawRequests(const qsf::Map& map, const std::vector<firesimulation::ComponentData>& calculationResult);
		void prepareDataForNextCalculation(const qsf::Map& map, float secondsPassed);

		void drawFireChainLinesForAllFireReceiver(const qsf::Map& map);
		void drawFireRadianForAllFireComponents(const std::vector<firesimulation::ComponentData>& calculationResult, bool showFireRadians);
		void drawPositionCircleAllFireComponents(const std::vector<firesimulation::ComponentData>& calculationResult);

		void drawFireRadianForSelectedFireComponents(const std::vector<firesimulation::ComponentData>& calculationResult, const EntityIdSet& callerIds);
		void drawFireChainTextAndChainLinesForSelectedFireComponents(const EntityIdSet& callerIds);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FireMaterialManager*				mFireMaterialManager;				///< Fire material manager; always valid, we're responsible for destroying the instance
		bool								mFireSimulationEnabled;				///< "true" if fire simulation is enabled
		float								mFireEnergyMultiplier;				///< Usually 1.0f, or set to a different non-negative value to increase or decrease energy loss due to injuries

		// Job update
		qsf::JobProxy						mSimulationJobProxy;				///< Regular simulation job proxy; for updates once a game tick

		// Debug
		qsf::DebugDrawProxy					mDebugDrawProxy;					///< Only for debug output

		// Fire spread simulation
		FireSpreadCalculation&				mFireSpreadCalculation;				///< Calculates the fire spread inside a separate thread
		qsf::Time							mPassedUpdateSimulationTime;		///< Time passed between two fire spread calculations
		qsf::Time							mUpdateFireSpreadInterval;			///< Holds the interval how often (per second) the fire spread calculation should be done

		SimulationVisualisationMode			mFireRadiansVisualisationMode;		///< The mode of the simulation visualisation for fire radians
		SimulationVisualisationMode			mFireSpreadVisualisationMode;		///< The mode of the simulation visualisation for fire spread
		FireSpreadLinesVisualisationMode	mFireSpreadLinesVisualisationMode;	///< The mode of the fire spread receiver lines drawing (only for selected items)
		bool								mIsFireSpreadPreview;				///< Indicates if the fire spread preview is active or not
		bool								mSavedShowFireReceiverDebugFlag;	///< Holds the value of the show fire receiver debug flag
		bool								mDrawFireReceiverLinesActive;
		bool								mResetFireSystem;					///< "true" if the fire system is not allowed to give damage to other entities

		typedef firesimulation::FireReceiverData FireReceiverData;
		std::vector<FireReceiverData> mActiveFireReceivers;


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/fire/FireSystem-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::FireSystem)
