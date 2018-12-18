// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/vehicle/parts/WheelComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"

#include <qsf_game/component/base/LightControllerComponent.h>

#include <qsf/debug/DebugDrawProxy.h>

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class WaterCannonComponent;
}
namespace qsf
{
	class MovableComponent;
	class Prototype;
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
	*    EMERGENCY 5 road vehicle component class
	*
	*  @note
	*    - The chassis mesh should be part of the same entity the road vehicle component is in
	*/
	class EM5_API_EXPORT RoadVehicleComponent : public em5::VehicleComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::RoadVehicleComponent" unique component ID
		static const uint32 VEHICLE_WHEELS;	///< "VehicleWheels" unique property ID
		static const uint32 VEHICLE_LIGHTS;	///< "VehicleLights" unique property ID

		enum BlinkerState
		{
			BLINKERS_OFF,
			BLINKERS_LEFT_ON,
			BLINKERS_RIGHT_ON,
			BLINKERS_BOTH_ON
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		inline explicit RoadVehicleComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~RoadVehicleComponent();

		inline void setFrontWheelDirection(float degrees);

		const std::vector<uint64>& getVehicleWheelIdsByType(WheelComponent::WheelType wheelType) const;

		const std::vector<uint64>& getVehicleLightIdsByType(qsf::game::LightControllerComponent::LightPosition lightPosition) const;

		/**
		*  @brief
		*    Forces an update of the light states (is needed, when either the texture or the mesh/prefab was changed)
		*/
		void updateLightStates();
		void setBlinkerState(BlinkerState blinkerState, bool force = false);
		void setHeadLightState(bool enabled, bool force = false);
		void setBrakeLightState(bool enabled, bool force = false);
		void setBlueLightState(bool enabled, bool force = false);
		void setEmergencyFlasher(bool enabled);

		inline bool getTooBigToTow() const;
		inline void setTooBigToTow(bool tooBigToTow);

		inline bool getCanSpawnDriver() const;
		inline void setCanSpawnDriver(bool canSpawnDriver);

		WaterCannonComponent* getWaterCannonComponent() const;

		/**
		*  @brief
		*    Disables connected hydrants. Some vehicles has mobile hydrants (e.g RW has 4 connected hydrants).
		*/
		void disableVehicleHydrants();


	//[-------------------------------------------------------]
	//[ Public virtual em5::DamageComponent methods           ]
	//[-------------------------------------------------------]
	public:
		virtual void onSetIntact() override;
		virtual void onSetDamagedLevel1() override;
		virtual void onSetDamagedLevel2() override;
		virtual void onSetDestroyed() override;
		virtual void onFireStarted(FreeplayEvent* freeplayEvent) override;
		virtual void onExplosion(FireComponent* fireComponent) override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onSetSimulating(bool simulating) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::VehicleComponent methods       ]
	//[-------------------------------------------------------]
	protected:
		virtual void restoreEntitiesFromPrefab(const qsf::Prototype& prototype, std::unordered_map<uint64, uint64>& prototypeIdMap) override;
		virtual void initializePrefabClearComponentExceptions() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onObjectRenderingStartup() override;
		virtual void onObjectRenderingShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::JobProxyMovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onUpdateMovableObjectRenderingJob(const qsf::JobArguments& jobArguments) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<uint64> EntityIdArray;	///< Array of entity IDs


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Send the vehicle damaged message just once
		*/
		void sendRoadVehicleDamagedMessage();
		void placeAllPassengerDeadAroundVehicle(FreeplayEvent* freeplayEvent);

		// Job updates
		void registerSimulationJobProxy();
		void updateSimulation(const qsf::JobArguments& jobArguments);

		// Wheels
		void initializeWheel(uint64 entityId);
		void updateWheels(const qsf::Time& timePassed);

		// Lights
		void updateLightsList(const std::vector<uint64>& lightIds, const qsf::Time& timePassed, bool update, bool show = false, bool force = false);
		void updateBrakeLightsState(const qsf::Time& timePassed);
		bool checkBrakeLights();

		// Swinging
		void checkSwing();
		void updateSwingAnimation(const qsf::Time& timePassed);

		// Helper
		void precalculateLightsToEntitiesMap() const;
		void precalculateWheelsToEntitiesMap() const;

		bool checkEnvironmentForHeadlight();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Weak pointers for important components inside the same entity
		qsf::WeakPtr<qsf::MovableComponent> mMovableComponent;

		// Wheels
		float			mFrontWheelDirection;	///< The angle of the front wheels
		EntityIdArray	mVehicleWheelIds;
		mutable boost::container::flat_map<WheelComponent::WheelType, std::vector<uint64>> mWheelTypeToEntityIdsMap;	///< Precalculated wheel entities with wheel type. No need to serialize.
		float			mChainAngle;

		// Lights
		EntityIdArray	mVehicleLightIds;
		mutable boost::container::flat_map<qsf::game::LightControllerComponent::LightPosition, std::vector<uint64>> mLightTypeToEntityIdsMap;	///< Precalculated light entities with light position, technicaly it is an qsf::game::LightPosition. No need to serialize.
		mutable bool	mLightTypeToEntityIdsMapInitialized;

		// Blinkers
		BlinkerState	mBlinkerState;			///< Tells us which blinkers are turned on
		qsf::Time		mBlinkerTime;			///< Blinker timing variable

		// Blue lights
		bool			mBlueLightActive;		///< If "true", the blue lights are turned on
		float			mBlueLightTime;			///< Blue light time

		// Brake lights
		bool			mIsBrakeLightActive;	///< If "true", the brake lights are turned on. No need to serialize.
		qsf::Time		mBrakeLightTimeout;		///< Timeout until the brake light is switched off again

		// Head lights
		bool			mIsHeadLightActive;		///< If "true", the head lights are turned on

		// Special
		uint64			mSpecialEntityId;		///< The ID of an object, specialized for this vehicle, eg. water cannon for TLF
		bool			mIsCommandable;			///< This caches the value of entityHelper::isCommandable() for performance reason

		// Particles
		EntityIdArray	mStateDamagedParticleIds;
		EntityIdArray	mStateHeavilyDamagedParticleIds;
		// TODO(mk) Remove explode particle from road vehicle
		EntityIdArray	mStateExplodedParticleIds;

		// Job proxies for regular update
		qsf::JobProxy mSimulationJobProxy;	///< Regular simulation job proxy; for updates once a game tick

		// Swinging effect
		qsf::Time mSwingTimeout;

		// Tow Car Help
		bool mTooBigToTow;

		bool mCanSpawnDriver;	///< If "true", the vehicle can spawn a driver if it gets damaged by fire

		qsf::DebugDrawProxy mDebugDrawProxy;	///< Only for debug


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

	public:
		EntityIdArrayInterface VehicleWheelsArray;
		EntityIdArrayInterface VehicleLightsArray;
		EntityIdArrayInterface StateDamagedParticles;
		EntityIdArrayInterface StateHeavilyDamagedParticles;
		EntityIdArrayInterface StateExplodedParticles;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/vehicle/RoadVehicleComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::RoadVehicleComponent)
