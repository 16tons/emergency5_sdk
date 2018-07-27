// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/vehicle/VehicleComponent.h"


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
	*    EMERGENCY 5 helicopter component class
	*/
	class EM5_API_EXPORT HelicopterComponent : public em5::VehicleComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::HelicopterComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void ignoreMarkerSelectionEntityId(const qsf::Entity& entity, boost::container::flat_set<uint64>& ignoreEntityIds);


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
		explicit HelicopterComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HelicopterComponent();

		void setGroundMarkerEnabled(bool light, bool selector);
		qsf::Entity* getMarkerSelectionEntity() const;

		/**
		*  @brief
		*    Get if lights are activated
		*/
		bool getLightEnabled() const;

		/**
		*  @brief
		*    Set all lights on/off
		*
		*  @param[in] light
		*    If we have activated the light or not
		*/
		void setLightEnabled(bool light);

		const qsf::AssetProxy& getDustEffect() const;
		void setDustEffect(const qsf::AssetProxy& dustEffect);

		const qsf::AssetProxy& getGroundMarkerAsset() const;
		void setGroundMarkerAsset(const qsf::AssetProxy& groundMarkerAsset);

		uint64 getGroundLightEntityId() const;
		void setGroundLightEntityId(uint64 groundLightEntitId);

		const glm::vec3& getCollisionExtentsOnGround() const;
		void setCollisionExtentsOnGround(const glm::vec3& collisionExtentsOnGround);

		/**
		*  @brief
		*    Get the if the rotor is active
		*/
		bool isFlying() const;

		/**
		*  @brief
		*    Set the rotor to rotate
		*/
		void setFlying(bool flying);

		/**
		*  @brief
		*   Force the helicopter to instantly fly or not, used to spawn a flying helicopter
		*/
		void forceFlying(bool rotorActive);

		bool isRotorAtFullSpeed() const;
		bool isRotorStopped() const;

		/**
		*  @brief
		*   "true" if the helicopter lands after movement automatically on ground
		*/
		bool getCanLandOnGround() const;
		void setCanLandOnGround(bool canLandOnGround);

		/**
		*  @brief
		*    Start all dust effects
		*/
		void startDustEffects();
		void stopDustEffects();

		/**
		*  @brief
		*    Start/Stop the helicopter swinging
		*/
		void startSwing();
		void stopSwing();

		void setCollisionShapeAir();
		void setCollisionShapeGround();

		/**
		*  @brief
		*    Get the if the helicopter is starting or landing
		*
		*  @note
		*    This information is currently only needed for multiplay
		*/
		bool isStartingOrLanding() const;

		/**
		*  @brief
		*    Set if the helicopter is starting or landing
		*
		*  @note
		*    This information is currently only needed for multiplay
		*/
		void setIsStartingOrLanding(bool startingOrLading);


	//[-------------------------------------------------------]
	//[ Public virtual em5::DamageComponent methods           ]
	//[-------------------------------------------------------]
	public:
		virtual void onSetDamagedLevel2() override;
		virtual void onSetDestroyed() override;
		virtual void onFireStarted(FreeplayEvent* freeplayEvent) override;
		virtual bool checkReceiveDamage(damagetype::DamageType damageType, qsf::Component* sender) override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onShutdown() override;
		virtual void onSetSimulating(bool simulating) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::VehicleComponent methods       ]
	//[-------------------------------------------------------]
	protected:
		virtual void restoreEntitiesFromPrefab(const qsf::Prototype& prototype, std::unordered_map<uint64, uint64>& prototypeIdMap) override;
		virtual void initializePrefabClearComponentExceptions() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<uint64> EntityIdArray;	///< Array of entity IDs


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Job updates
		void updateAnimation(const qsf::JobArguments& jobArguments);
		void updateRotor(const qsf::Time& timePassed);
		void updateGroundMarker();

		// Helper
		void enableLights(bool enable);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		EntityIdArray	mVehicleRotorIds;
		qsf::AssetProxy	mDustEffectAsset;
		qsf::AssetProxy	mGroundMarkerAsset;
		uint64			mGroundLightEntityId;
		glm::vec3		mCollisionExtentsOnGround;
		glm::vec3		mCollisionExtentsInAir;

		// Lights
		EntityIdArray	mVehicleLightIds;
		bool			mLightEnabled;

		// Rotor
		bool			mIsFlying;

		bool			mCanLandOnGround;

		// Needed for multiplay
		bool			mIsStartingOrLanding;

		// Audio proxy
		AudioProxy		mAudioProxy;

		// Marker and selection
		qsf::WeakPtr<qsf::Entity>	mMarkerLightEntity;
		qsf::WeakPtr<qsf::Entity>	mMarkerSelectionEntity;
		qsf::WeakPtr<qsf::Entity>	mDustEffectEntity;
		bool						mMarkerLightEnabled;
		bool						mMarkerSelectorEnabled;

		// Job proxies for regular update
		qsf::JobProxy	mAnimationJobProxy;	///< Regular animation job proxy; for updates once a frame


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	public:
		EntityIdArrayInterface VehicleRotorArray;
		EntityIdArrayInterface VehicleLightsArray;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::HelicopterComponent)
