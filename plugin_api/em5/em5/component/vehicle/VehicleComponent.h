// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/damage/DamageComponent.h"
#include "em5/component/door/DoorComponent.h"

#include <qsf_game/component/base/PrototypeContainer.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FreeplayEvent;
}
namespace qsf
{
	class Map;
	class Entity;
	class Prototype;
	class LinkComponent;
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
	*    Vehicle component base class
	*/
	class EM5_API_EXPORT VehicleComponent : public DamageComponent, public qsf::game::PrototypeContainer
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::VehicleComponent" unique component ID

		// Door categories
		static const uint32 CATEGORY_PASSENGER;		///< "0" unique identifier for the passenger door category
		static const uint32 CATEGORY_PERSONNEL;		///< "1" unique identifier for the personnel door category
		static const uint32 CATEGORY_VEHICLE;		///< "2" unique identifier for the vehicle door category -- TODO(fw): What is this for actually?

		typedef std::vector<uint64> EntityIdArray;		///< Array of entity IDs
		typedef boost::container::flat_map<DoorComponent::DoorType, std::vector<uint64>> DoorMap;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		inline explicit VehicleComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~VehicleComponent();

		/**
		*  @brief
		*    Get/set special child entity ID
		*/
		inline uint64 getSpecialEntity() const;
		void setSpecialEntity(uint64 specialEntityId);

		/**
		*  @brief
		*    Get/set the mainly used door type, used for entering/leaving the vehicle
		*/
		inline DoorComponent::DoorType getMainDoorType() const;
		void setMainDoorType(DoorComponent::DoorType mainDoorType);

		//[-------------------------------------------------------]
		//[ Doors                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*   Open/close the doors with the given from the vehicle
		*
		*  @param[in] doorType
		*    Door type to open/close
		*  @param[in] open
		*    State of the door
		*/
		void openDoor(DoorComponent::DoorType doorType, bool open);

		/**
		*  @brief
		*   Check all doors of the type are in the given state
		*
		*  @param[in] doorType
		*    Door type to check
		*  @param[in] doorState
		*    State of the door
		*/
		bool areAllDoorsOfTypeInState(DoorComponent::DoorType doorType, DoorComponent::DoorState doorState, float percent = 1.0f) const;

		bool getFirstDoorTransformByType(uint32 doorType, qsf::Transform& outDoorTransform) const;

		qsf::Entity* findNearestDoor(DoorComponent::DoorType doorType, const glm::vec3& targetPoint) const;

		inline const std::vector<uint64>& getVehicleDoors() const;

		const std::vector<uint64>& getVehicleDoorIdsByType(DoorComponent::DoorType doorType) const;
		uint64 getDriverVehicleDoorId() const;

		void closeAllDoors();

		//[-------------------------------------------------------]
		//[ Gameplay                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Let persons flee from this burning vehicle
		*/
		void personsFleeFromFire(bool allowSpawning);

		/**
		*  @brief
		*    Trap persons inside this vehicle
		*/
		void trapPersons(FreeplayEvent* freeplayEvent = nullptr, const std::vector<uint32>& injuryPool = std::vector<uint32>());

		//[-------------------------------------------------------]
		//[ Damage                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get vehicle life energy
		*/
		inline float getLifeEnergy() const;

		/**
		*  @brief
		*    Apply damage to the vehicle. Or repair with negative damageValues.
		*/
		void applyDamage(float damageValue, FreeplayEvent* freeplayEvent = nullptr);

		/**
		*  @brief
		*    Check if vehicle can be repaired
		*/
		bool canBeRepaired() const;

		/**
		*  @brief
		*    Return "true" if the vehicle is allowed to move
		*/
		inline bool isMovable() const;

		/**
		*  @brief
		*    Check if the vehicle can be cut using the rescue scissors
		*/
		bool getCanBeCut() const;

		/**
		*  @brief
		*    Set whether the vehicle can be cut using the rescue scissors
		*/
		void setCanBeCut(bool canBeCut, FreeplayEvent* freeplayEvent = nullptr);

		/**
		*  @brief
		*    Set whether the vehicle can be repaired by an engineer
		*/
		void setCanBeRepaired(bool canBeRepaired, FreeplayEvent* freeplayEvent = nullptr);

		/**
		*  @brief
		*    Gets the duration needed to cut the vehicle open
		*/
		qsf::Time getCutDuration() const;

		/**
		*  @brief
		*    Configures the duration needed to cut the vehicle open
		*/
		void setCutDuration(const qsf::Time& duration);

		/**
		*  @brief
		*    Check if the vehicle can be picked up by the towcar
		*/
		inline bool getCanBeTowed() const;

		/**
		*  @brief
		*    Check if the vehicle's "CAN_BE_TOWED" flag is set (this is only a part of the check whether the vehicle can be towed)
		*/
		inline bool getCanBeTowedFlag() const;

		/**
		*  @brief
		*    Set whether the vehicle can be picked up by the towcar
		*/
		void setCanBeTowed(bool canBeTowed, FreeplayEvent* freeplayEvent = nullptr);

		/**
		*  @brief
		*    Check if the vehicle is set as totally untowable
		*/
		inline bool getCanReallyNotBeTowed() const;

		/**
		*  @brief
		*    Set whether the vehicle can be picked up by the towcar
		*/
		void setCanReallyNotBeTowed(bool canReallyNotBeTowed, FreeplayEvent* freeplayEvent = nullptr);

		/**
		*  @brief
		*    Check if the vehicle can and must be recovered by the crane vehicle
		*/
		inline bool getMustBeRecovered() const;

		/**
		*  @brief
		*    Set whether the vehicle can and must be recovered by the crane vehicle
		*/
		void setMustBeRecovered(bool mustBeRecovered, FreeplayEvent* freeplayEvent = nullptr);

		void hideParticleChildrenAtMapSave();

		/**
		*  @brief
		*    Set/Get if this vehicle can be damaged or not
		*/
		bool isInvincible() const;
		void setInvincible(bool invincible);

		inline bool getIsParkingEntity() const;
		inline void setIsParkingEntity(bool isParkingEntity);

		/**
		*  @brief
		*    Stop possible actions
		*/
		void stopActions() const;

		void precalculateDoorsToEntitiesMap() const;

		//[-------------------------------------------------------]
		//[ Resistances                                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the fire resistance
		*/
		const float getFireResistance() const;

		/**
		*  @brief
		*    Returns the damage resistance
		*/
		const float getDamageResistance() const;

		// Get the resistances as string
		std::string getResistancesAsString() const;

		// Set the resistances by string
		void setResistancesByString(const std::string& resistances);

		//[-------------------------------------------------------]
		//[ Emergency operations                                  ]
		//[-------------------------------------------------------]
		inline bool getIsOnEmergencyOperations() const;
		inline void setIsOnEmergencyOperations(bool enable);

		inline const qsf::AssetProxy& getSignalAudioAsset() const;
		inline void setSignalAudioAsset(const qsf::AssetProxy& signalSound);

		inline bool isSirenEnabled() const;
		void setSirenEnabled(bool sirenEnabled);

		/**
		*  @brief
		*    Play siren
		*/
		void playSirenOnce();
		void playSirenLoop();
		void stopSiren();
		void stopSirenLoop();


	//[-------------------------------------------------------]
	//[ Public virtual methods                                ]
	//[-------------------------------------------------------]
	public:
		virtual bool getDoorPositions(DoorComponent::DoorType doorType, std::vector<qsf::Transform>& outDoorTransform, const qsf::Transform& worldTransform) const;
		virtual bool getDoorPosition(uint32 doorIndex, glm::vec3& outDoorPosition) const;


	//[-------------------------------------------------------]
	//[ Public virtual em5::DamageComponent methods           ]
	//[-------------------------------------------------------]
	public:
		virtual void receiveDamage(float amount, damagetype::DamageType damageType, qsf::Component* sender = nullptr) override;

		/**
		*  @brief
		*    Check if vehicle can be reset by cleanup
		*/
		virtual bool isReadyForCleanup() const override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onSetSimulating(bool simulating) override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual qsf::Component* getMovableObjectRenderingListenerComponent() override;
		virtual void onObjectRenderingStartup() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::JobProxyMovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onUpdateMovableObjectRenderingJob(const qsf::JobArguments& jobArguments) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::DamageComponent methods        ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetDamageState(DamageState damageState) override;
		virtual void onSetIntact() override;
		virtual void onSetDamagedLevel1() override;
		virtual void onSetDamagedLevel2() override;
		virtual void onSetDestroyed() override;
		virtual void onSetFireLifePercentage(float fireLifePercentage) override;
		virtual void onFireStarted(FreeplayEvent* freeplayEvent) override;
		virtual void onFireStopped() override;
		virtual void onExplosion(FireComponent* fireComponent) override;


	//[-------------------------------------------------------]
	//[ Protected virtual methods                             ]
	//[-------------------------------------------------------]
	protected:
		virtual void restoreEntitiesFromPrefab(const qsf::Prototype& prototype, std::unordered_map<uint64, uint64>& prototypeIdMap);
		virtual void initializePrefabClearComponentExceptions();


	//[-------------------------------------------------------]
	//[ Protected definitions                                 ]
	//[-------------------------------------------------------]
	protected:
		static std::vector<uint64> EMPTY_ENTITY_VECTOR;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		enum InternalFlags
		{
			CAN_REALLY_NOT_BE_TOWED,	///< The vehicle can not be towed under any circumstances.
			CAN_BE_CUT,					///< Can be cut open with the fireman's rescue scissors
			CAN_BE_TOWED,				///< Can be picked up by the tow car
			MUST_BE_RECOVERED,			///< Has to be recovered by the crane vehicle before any other interaction
			CAN_BE_REPAIRED				///< Can be repaired by an engineer
		};
		typedef qsf::EnumBitmask<uint8, InternalFlags> InternalFlagSet;

		inline InternalFlagSet& getInternalFlags();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		// Damage
		void setVehiclePrefab(const qsf::AssetProxy& prefabAssetProxy);
		void cloneEntitiesFromPrefabByComponentId(qsf::Map& map, const qsf::Entity& parentEntity, const std::vector<uint32>& componentIds, const qsf::LinkComponent& prototypeLinkComponent, std::unordered_map<uint64, uint64>& prototypeIdMap);
		void cloneEntitiesFromPrefab(qsf::Map& map, const qsf::Entity& parentEntity, const std::vector<uint64>& sourceEntityIds, std::vector<uint64>& destinationEntityIds, const qsf::LinkComponent& prototypeLinkComponent, std::unordered_map<uint64, uint64>& prototypeIdMap);
		void setDamageOverlaysActive(bool smutActive, bool emissiveActive);
		void switchToDamagePrefab(bool prefabActive);
		void switchFromCombinedToNormalPrefab();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		EntityIdArray	mVehicleDoorsIds;

		// Damage
		std::vector<uint32> mComponentExceptionIds;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Helper
		void updateHighlightJob(const qsf::JobArguments& arguments);

		// Damage
		void applyDamageLevel1MaterialToIntactMesh(bool materialActive);

		// Mesh
		void updateShaderHeatValue(float heat);

		// Hint
		void showDamageHint(float oldLifeEnergy) const;

		// Life bar
		void showHitpointBar();

		void playSirenInternal(bool loop);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum VehiclePrefabState
		{
			VehiclePrefabState_Normal,
			VehiclePrefabState_Destroyed,
			VehiclePrefabState_Combined
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		EntityIdArray		mDamagedLevel1ParticleIds;
		EntityIdArray		mDamagedLevel2ParticleIds;
		EntityIdArray		mDestroyedParticleIds;
		mutable DoorMap		mDoorTypeToEntityIdsMap;	///< Precalculated door entities with door type. No need to serialize.
		qsf::Time			mCutDuration;				///< duration of the CutRoadVehicleAction for this vehicle

		// Player vehicle logic
		bool				mIsOnEmergencyOperations;	///< Flag whether the entity is moving on emergency operations ("Einsatzfahrt"), didn't find a better name

		InternalFlagSet		mInternalFlags;				///< Set of internal flags, see enum "InternalFlags"

		// Highlighting
		qsf::JobProxy		mHighlightJobProxy;

		// Damage
		float				mLifeEnergy;				///< Vehicle life energy before it starts burning by damage like shots, between 0.0f and 1.0f; fire energy does not affect this, but only the fire component
		bool				mDamageLevel1MaterialActive;
		bool				mDamagePrefabActive;
		std::string			mPrefabLocalAssetName;
		std::string			mDamagePrefabLocalAssetName;
		float				mFireResistance;
		float				mDamageResistance;
		bool				mInvincible;
		bool				mIsParkingEntity;			///< "true" if the entity is not moving and at map start in the map, important for map cleanups
		VehiclePrefabState	mCurrentPrefabState;

		// Siren sound
		qsf::AssetProxy		mSignalSound;				///< The asset of the signal sound
		AudioProxy			mSirenSFX;					///< SFX asset to play when siren is active
		bool				mIsSirenEnabled;			///< If "true", the siren may be played, otherwise it is permanently off
		bool				mIsSirenSFXSet;				///< If "true", the siren SFX is set

		// Special
		uint64				mSpecialEntityId;			///< The ID of a child object, specialized for this vehicle, eg. water cannon for TLF
		DoorComponent::DoorType	mMainDoorType;			///< The door type is used for entering/ exit the vehicle. In em5 units entering the "codriver door" by default.

		// Rendering
		float				mSecondsPassedBetweenUpdates;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

	public:
		QSF_CAMP_ARRAY_INTERFACE_CLASS_WITH_VARIABLE(EntityIdArrayInterface, VehicleComponent, std::vector<uint64>, uint64);
		EntityIdArrayInterface VehicleDoorsArray;
		EntityIdArrayInterface DamagedLevel1Particles;	///< Array of the linked entities, hidden in damage state
		EntityIdArrayInterface DamagedLevel2Particles;	///< Array of the linked entities, hidden in damage state
		EntityIdArrayInterface DestroyedParticles;		///< Array of the linked entities, hidden in destroyed state


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/vehicle/VehicleComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::VehicleComponent)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(em5::VehicleComponent::EntityIdArrayInterface)
