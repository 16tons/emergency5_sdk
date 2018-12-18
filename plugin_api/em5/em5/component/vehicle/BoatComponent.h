// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/vehicle/VehicleComponent.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class WaterCannonComponent;
	class SpawnPointComponent;
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
	*    EMERGENCY 5 boat component class
	*/
	class EM5_API_EXPORT BoatComponent : public em5::VehicleComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::BoatComponent" unique component ID


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
		explicit BoatComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BoatComponent();

		/**
		*  @brief
		*    Get/Set the ID of the lake (connected water territory) where this boat is inside
		*/
		uint64 getLakeId() const;
		void setLakeId(uint64 lakeId);

		/**
		*  @brief
		*    Enable/disable all lights
		*/
		void enableLights(bool enable);

		/**
		*  @brief
		*    Enable/Disable PropellerParticles
		*/
		bool isPropellerParticlesEnabled() const;
		void enablePropellerParticles(bool enable);

		/**
		*  @brief
		*    Get/set radar entity ID
		*/
		uint64 getRadarEntityId() const;
		void setRadarEntityId(uint64 craneEntityId);

		/**
		*  @brief
		*    Enable/disable the radar
		*/
		bool isRadarEnabled() const;
		void enableRadar(bool enable);

		/**
		*  @brief
		*    Get water cannon
		*/
		WaterCannonComponent* getWaterCannonComponent() const;

		/**
		*  @brief
		*    Get spawn point IDs
		*/
		const EntityIdArray& getSpawnPointIds() const;

		/**
		*  @brief
		*    Get a random spawn point
		*/
		SpawnPointComponent* getRandomSpawnPointComponent() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;
		virtual bool onStartup() override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::DamageComponent methods        ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetDestroyed() override;
		virtual void onFireStarted(FreeplayEvent* freeplayEvent) override;
		virtual void onFireStopped() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<uint64> EntityIdArray;	///< Array of entity IDs


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateSinkAnimation(const qsf::JobArguments& arguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64				mLakeId;			///< ID of the lake
		uint64				mRadarEntityId;		///< Radar entity ID

		// Lights
		EntityIdArray		mVehicleLightIds;
		bool				mLightEnabled;

		// Particles
		EntityIdArray		mPropellerParticleIds;
		bool				mPropellerParticleEnabled;

		bool				mRadarEnabled;

		// Spawn points
		EntityIdArray		mDrowningSpawnPointIds;

		// Job proxy
		qsf::JobProxy		mSinkAnimationJobProxy;
		float				mSinkAnimationSecondsPassed;
		AudioProxy			mAudioProxy;

		// Signal sounds
		AudioProxy			mSirenSFX;				///< SFX asset to play when siren is active
		bool				mIsSirenSFXSet;			///< If "true", the siren SFX is set


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

	public:
		EntityIdArrayInterface VehicleLightsArray;
		EntityIdArrayInterface DrowningSpawnPointArray;		///< Array of the linked spawn points, for drowning persons
		EntityIdArrayInterface PropellerParticlesArray;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::BoatComponent)
