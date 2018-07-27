// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/audio/AudioProxy.h"
#include "em5/component/damage/SimpleDamageComponent.h"
#include "em5/gangster/GangsterPool.h"

#include <qsf_game/component/base/PrototypeContainer.h>

#include <qsf/base/PropertyHelper.h>
#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/math/EnumBitmask.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/reflection/type/CampGlmVec3.h>
#include <qsf/reflection/type/CampQsfAssetProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class BuildingClippingManager;
	class DoorComponent;
	class FreeplayEvent;
	namespace multiplayer
	{
		class BuildingData;
	}
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
	*    EMERGENCY 5 building component class
	*/
	class EM5_API_EXPORT BuildingComponent : public SimpleDamageComponent, public qsf::game::PrototypeContainer
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class BuildingLightTimeControlComponent;	///< For controlling the lights of the building (linked childs)
		friend class BuildingClippingManager;			///< Needs to call "startClipping()", "continueClipping()" and "endClipping()"
		friend class multiplayer::BuildingData;			///< Needs to access/modify the mPersonInWindowEntityId member


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "em5::BuildingComponent" unique component ID
		static const uint32 CLIPPING_ENABLED;	///< "ClippingEnabled" unique class property ID inside the class


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
		inline explicit BuildingComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~BuildingComponent();

		/**
		*  @brief
		*    Return whether or not building clipping is enabled
		*
		*  @return
		*    "true" if building clipping is enabled, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool getClippingEnabled() const;

		/**
		*  @brief
		*    Set whether or not building clipping is enabled
		*
		*  @param[in] clippingEnabled
		*    "true" if building clipping is enabled, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setClippingEnabled(bool clippingEnabled);

		/**
		*  @brief
		*    Handle the case if persons are inside the building while it catches fire
		*/
		void onCaughtFire();

		/**
		*  @brief
		*    Handle the case if the building stopped burning
		*
		*  @param[in] destroyed
		*    Indicates if the building was destroyed
		*/
		void onStoppedFire(bool destroyed);

		/**
		*  @brief
		*    Handle that a person wants to wave out of a window
		*/
		bool letPersonWaveOutOfWindow(qsf::Entity& personEntity, FreeplayEvent* freeplayEvent, float chance = 1.0f);

		/**
		*  @brief
		*    Stop person to wave out of the window
		*/
		void stopPersonWaveOutOfWindow();

		/**
		*  @brief
		*    Get the person who is waving out of the window
		*
		*  @return
		*    Returns entity of the person, can be a null pointer (if !isPersonWavingOutOfWindow)
		*/
		qsf::Entity* getPersonWavingOutOfWindow() const;

		/**
		*  @brief
		*    Returns the number of persons trapped in the building.
		*    This value is just for setup, in case a fire starts and has nothing to do with the persons actually inside
		*/
		inline uint16 getSetupNumberOfTrappedPersons() const;

		/**
		*  @brief
		*    Returns the number of persons trapped in the building which are injured.
		*    This value is just for setup, in case a fire starts and has nothing to do with the persons actually inside
		*/
		inline uint16 getSetupNumberOfTrappedInjuredPersons() const;

		/**
		*  @brief
		*    Check if a person is waving out of the window
		*/
		inline bool isPersonWavingOutOfWindow() const;

		/**
		*  @brief
		*    Setup the kind and number of persons inside the building
		*
		*  @param[in] minPersonCount
		*    At least this number of persons gets trapped
		*  @param[in] maxPersonCount
		*    Maximal this number of persons gets trapped
		*  @param[in] rescueTime
		*    The time to rescue the persons before they die
		*/
		bool isContainingTrappedPersons() const;
		void setupTrappedPersons(uint16 minPersonCount, uint16 maxPersonCount, bool createPersons = true, const qsf::Time& rescueTime = qsf::Time::fromSeconds(300.0f));
		void setupTrappedInjuredPersons(uint16 minHealthyPersonCount, uint16 maxHealthyPersonCount, uint16 minInjuredPersonCount, uint16 maxInjuredPersonCount, bool blockedDoor = true, bool createPersons = true, const qsf::Time& rescueTime = qsf::Time::fromSeconds(300.0f));

		bool isContainingGangsters() const;
		bool isContainingHostages() const;

		/**
		*  @brief
		*    Create persons and trap them in the building. Call the setup functions before to configure the type and number of trapped persons
		*    If there is no right configuration, then no person is created
		*/
		void createTrappedPerson(FreeplayEvent& freeplayEvent);

		qsf::Time getRemainingTimeForTrappedPersons() const;

		/**
		*  @brief
		*    Create the timer for the trapped persons
		*/
		void setTrappedPersonTimer(qsf::Time time);

		/**
		*  @brief
		*    Untraps the trapped persons inside the building
		*
		*  @note
		*    This method only untraps persons which were trapped via "trapPersons()"-call
		*/
		void untrapPersons();

		void untrapPerson_SingleInjured();
		void untrapPersonsInternal(bool singleInjuredPerson);
		bool untrapPersonInternal(qsf::Entity& personToUntrap, qsf::Time fleeWaitTime);
		void updateIcons();

		/**
		*  @brief
		*    Return the door position of the building
		*
		*  @return
		*    Returns the door position
		*/
		glm::vec3 getDoorPosition() const;

		/**
		*  @brief
		*    Return the door rotation of the building
		*
		*  @return
		*    Returns the door rotation
		*/
		glm::quat getDoorRotation() const;

		/**
		*  @brief
		*    Return the main door transformation of the building
		*
		*  @return
		*    The main door transformation
		*/
		qsf::Transform getDoorTransform() const;

		/**
		*  @brief
		*    Find a place in front of the main door at a certain distance
		*/
		bool getWorldPositionInFrontOfDoor(float distance, glm::vec3& position, glm::quat& rotation) const;

		/**
		*  @brief
		*   Check if the building has a door
		*/
		bool hasDoor() const;

		// Getters and setters for single entity slots
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(MainDoorId, uint64, mDoors.mMainDoorId);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(EventWindowId, uint64, mEventWindowId);

		// Getter and setter for building explosion center (for the fake explosion for the event: fire after explosion in building)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(ExplosionCenter, const glm::vec3&, mExplosionCenter);
		glm::vec3 getExplosionCenterWorldSpacePosition() const;
		void setExplosionCenterWorldSpacePosition(const glm::vec3& worldSpacePosition);

		// Evacuate feature
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(EvacuateBuildingId, uint64, mEvacuateBuildingId);	// Connected to CAMP
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TrappedPersonsInjuryId, uint32, mTrappedPersonsInjuryId);

		/**
		*  @brief
		*    Set if the main door is damaged or not
		*
		*  @note
		*    - This method is currently only called by the BreakBuildingDoorOpenAction
		*/
		void setMainDoorDamaged(bool isDamaged);

		void hideParticleChildrenAtMapSave();

		inline const qsf::AssetProxy& getDamagedLevel1Material() const;
		void setDamagedLevel1Material(const qsf::AssetProxy& assetProxy);

		inline const qsf::AssetProxy& getIntactMaterial() const;
		void setIntactMaterial(const qsf::AssetProxy& assetProxy);

		inline float getCollapseDampeningFactor() const;
		inline void setCollapseDampeningFactor(float collapseDampeningFactor);

		inline float getCollapseDelay() const;
		inline void setCollapseDelay(float collapseDelay);

		inline float getCollapseShowRuinsDelay() const;
		inline void setCollapseShowRuinsDelay(float collapseShowRuinsDelay);

		inline bool getIsDoorBlocked() const;
		inline void setIsDoorBlocked(bool isDoorBlocked);

		inline qsf::Time getEboxRepairTime() const;
		inline void setEboxRepairTime(qsf::Time time);

		inline qsf::Time getFirefighterRescueTime() const;
		inline void setFirefighterRescueTime(qsf::Time time);

		/**
		*  @brief
		*    Collapse rubble are slots on the building which can be shown after a building collapse; it will be hidden again automatic in cleanup()
		*
		*  @remarks
		*    Collapse rubble objects are not shown by usual gameplay, but has always to be triggered by events using this special kind of editing
		*/
		void setCollapseRubbleVisible(bool show);

		void spawnGangsterInside(int numberOfGangsters, int numberOfHostages, int numberOfInjuredHostages, GangsterPool gangsterPoolType = GangsterPool::DEFAULT_EXTENDED);

		inline bool isClipped() const;

		/**
		*  @brief
		*    Set the damage state to destroy but don't play a collapse animation
		*/
		void setDestroyedWithoutAnimation(FreeplayEvent* freeplayEvent);

		/**
		*  @brief
		*    Reset all attached Eboxes
		*/
		void resetEboxes();


	//[-------------------------------------------------------]
	//[ Public virtual em5::DamageComponent methods           ]
	//[-------------------------------------------------------]
	public:
		virtual bool isBurning() const override;


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
		virtual void onFireDamageReceived(float amount, qsf::Component* sender, bool damageJustStarted) override;
		virtual void onExplosion(FireComponent* fireComponent) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onSetSimulating(bool simulating) override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onObjectRenderingStartup() override;
		virtual void onObjectRendering(const qsf::Component& component, const Ogre::Camera& ogreCamera) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum InternalFlags
		{
			SHOW_EXPLOSION,	///< Set it to start the animation of the explosion
			MESH_ACTIVE		///< Set if the mesh is shown
		};
		typedef qsf::EnumBitmask<uint8, InternalFlags> InternalFlagSet;

		typedef std::vector<uint64> EntityIdArray;		///< Array of entity IDs


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Job updates
		void updateCollapseAnimation(const qsf::JobArguments& jobArguments);
		void updateClippingAnimation(const qsf::JobArguments& jobArguments);

		// Lights
		void setLightsActive(bool active, bool force = false);
		void setLightControllerActive(bool active);

		// Spawn points
		void setSpawnsActive(bool active);

		// Door
		DoorComponent* getMainDoorComponent(qsf::Entity** outBestEntityFound) const;

		// Mesh
		void showChildMeshes(EntityIdArray& entityIdArray, bool show);
		void setDamageOverlaysActive(bool smutActive, bool emissiveActive);

		void killTrappedPersons(const qsf::MessageParameters& parameters = qsf::MessageParameters());
		void enableParticle(uint64 particleEntityId, bool enable);
		void enableParticle(qsf::Entity* particleEntity, bool enable);

		void applyDamageLevel1MaterialToIntactMesh(bool materialActive);

		void sendBuildingGetsDamagedMessage();

		void setHiddenSelfOnly(bool hidden);

		// Building clipping
		void startClipping();
		void continueClipping();
		void endClipping();
		void hideFire(bool hide);

		// Ebox blocking extinguish
		void updateFireExtinguishBlocked();
		void prepareEboxes();
		void onEboxRepaired(const qsf::MessageParameters& parameters, uint64 eboxId);

		// Debug
		void showDebug();
		void updateDebugDraw();
		void registerDebugDraw();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		struct DoorData
		{
			// Connected to CAMP
			uint64 mMainDoorId;

			inline DoorData();
		}
		mDoors;

		struct LightData
		{
			// Connected to CAMP
			EntityIdArray mLightIds;		///< Array of light entity IDs
			bool		  mLightsActive;	///< "true" when light components are actually active right now

			inline LightData();
		}
		mLights;

		qsf::AssetProxy		mDamagedLevel1Material;
		qsf::AssetProxy		mIntactMaterial;

		EntityIdArray		mDamagedLevel1EntityIds;
		EntityIdArray		mDamagedLevel2EntityIds;
		EntityIdArray		mDestroyedEntityIds;
		EntityIdArray		mCollapseRubbleEntityIds;
		EntityIdArray		mEboxEntityIds;
		uint64				mEvacuateBuildingId;

		InternalFlagSet		mInternalFlags;				///< Set of internal flags, see enum "InternalFlags"

		bool				mClippingEnabled;
		uint64				mEventWindowId;				///< ID of the entity that defines the position of the waving person
		uint64				mPersonInWindowEntityId;	///< ID of the entity that waves out of the window
		glm::vec3			mExplosionCenter;			///< Center for explosion in the fire by explosion building event

		qsf::Time			mEboxRepairTime;			///< Time it takes to repair eboxes
		qsf::Time			mFirefighterRescueTime;		///< Time it takes the firefighter inside the building to rescue a person

		// Internal state data
		bool				mCanCreateTrappedPersons;			///< Indicates if the trapped persons should be created
		uint16				mSetupNumberOfPersonsTrapped;		///< The number of persons to be trapped and contained, in case of fire
		uint16				mSetupNumberOfInjuredPersonsTrapped;///< The number of injured persons to be trapped and contained, in case of fire
		uint32				mTrappedPersonsInjuryId;				///< If used (i.e. value not "qsf::getUninitialized<uint32>()"), this injury ID is used for the trapped persons
		uint32				mTrappedPersonsDieTimerId;			///< Timer which lets the trapped person die when the timer signals
		uint64				mDoorParticleEffectId;
		bool				mIsDoorBlocked;						///< "true" if player units need to break the door with an axis, before they can enter
		bool				mMainDoorIsDamaged;					///< "true" if the door is optical damaged and a particle effect is running
		qsf::Time			mTrappedPersonsRescueTime;			///< Time to free the trapped persons before they die. The default is 300 sec.
		qsf::MessageProxy	mMessageProxyKillTrappedPersons;	///< Message proxy instance for the game timer message when the trapped persons should be murdered

		uint64				mCollapseAnimationEntityId;
		float				mCollapseAnimationYDistance;
		float				mCollapseElapsedTime;
		float				mCollapseDampeningFactor;
		float				mCollapseDelay;
		float				mCollapseShowRuinsDelay;
		float				mCollapseDelayTimeout;
		bool				mCollapseComplete;
		bool				mDamageLevel1MaterialActive;

		BuildingClippingManager*	mBuildingClippingManager;
		bool						mUseCameraClipping;
		bool						mDirtyMaterialAabb;
		float						mCurrentCameraHeight;
		float						mCurrentFadeOffset;

		// Check if a ebox was repaired
		std::vector<qsf::MessageProxy>	mMessageProxyEboxRepaired;	///< We react on finishing the repair of our eboxes

		// Audio proxy for destruction
		AudioProxy mAudioProxy;

		// Job proxies for regular update
		qsf::JobProxy		mAnimationJobProxy;			///< Regular animation job proxy; for updates once a frame
		qsf::JobProxy		mClippingAnimationJobProxy;	///< Regular animation job proxy; for updates once a frame

		// Debug
		qsf::JobProxy mDebugJobProxy;
		qsf::DebugDrawProxy		mDebugDrawProxy;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

	public:
		QSF_CAMP_ARRAY_INTERFACE_CLASS_WITH_VARIABLE(EntityIdArrayInterface, BuildingComponent, std::vector<uint64>, uint64);
		EntityIdArrayInterface LightArray;						///< Array of the lights
		EntityIdArrayInterface DamagedLevel1EntitiesArray;		///< Array of the linked entities, hidden in damage state
		EntityIdArrayInterface DamagedLevel2EntitiesArray;		///< Array of the linked entities, hidden in damage state
		EntityIdArrayInterface DestroyedEntitiesArray;			///< Array of the linked entities, hidden in destroyed state
		EntityIdArrayInterface CollapseRubbleArray;				///< Array of the linked entities displayed after building collapsed
		EntityIdArrayInterface EboxEntitiesArray;				///< Array of the linked entities which block this buildings fire from being extinguished


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/building/BuildingComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::BuildingComponent)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(em5::BuildingComponent::EntityIdArrayInterface)
