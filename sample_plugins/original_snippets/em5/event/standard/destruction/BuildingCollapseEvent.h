// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include <qsf/base/PropertyHelper.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/time/Time.h>


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
	*    EMERGENCY 5 freeplay event "9.4 building collapse"
	*/
	class EM5_API_EXPORT BuildingCollapseEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::BuildingCollapseEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BuildingCollapseEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BuildingCollapseEvent();

		/**
		*  @brief
		*    Initialization used by factory
		*
		*  @param[in] buildingId
		*    Entity ID for the building which should collapse
		*  @param[in] eventTagDebris
		*    Event-tag used for debris
		*  @param[in] eventTagExplosion
		*    Event-tag used for explosion effect
		*/
		void init(uint64 buildingId, const std::string& eventTagDebris, const std::string& eventTagExplosion, bool spawnPersonsUnderDebris);

		//[-------------------------------------------------------]
		//[ Public function to set/get with CAMP                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Time between explosion and spawning of debris
		*/
		float getDebrisSpawnTime() const;
		void setDebrisSpawnTime(float seconds);

		/**
		*  @brief
		*    Time between explosion and destruction of buildling
		*/
		float getDamageBuildingTime() const;
		void setDamageBuildingTime(float seconds);

		/**
		*  @brief
		*    Radius of explosion. Object inside (except persons, vehicles, houses) start burning.
		*/
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(ExplosionRadius, float, mExplosionRadius)

		/**
		*  @brief
		*    Time until Hint1 is played (player does nothing)
		*/
		float getHint01WaitTime() const;
		void setHint01WaitTime(float seconds);

		/**
		*  @brief
		*    First injured with less life-energy than that triggers a hint
		*/
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(Hint02LifeEnergy, float, mHint2LifeEnergy)


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual void hintCallback(Observer& hintObserver) override;
		virtual bool checkEventConfiguration() override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onStartRunning(const qsf::MessageParameters& parameters);
		void onDamageBuilding(const qsf::MessageParameters& parameters);
		void onUnbury(const qsf::MessageParameters& parameters);

		bool collectEventTagEntities(qsf::Entity& buildingEntity);
		bool explodeBuilding(qsf::Entity& buildingEntity);
		void startFires(const qsf::Entity& buildingEntity);
		void spawnDebrisAndInjured(qsf::Entity& buildingEntity);
		void spawnRubble(qsf::Entity& buildingEntity);
		qsf::Entity* getPersonPlaceholderLinked(qsf::Entity& entity) const;	///< returns person linked to this entity or nullptr when there isn't one.
		std::string getAdultCivilianPrefabName() const;
		void usePlaceholderTransform(qsf::Entity& entity, const qsf::Entity& placeholder) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::MessageProxy		  mTimerMessageProxy;
		uint32					  mDamageTimerId;
		uint32					  mDebrisTimerId;
		qsf::MessageProxy		  mCraneLiftMessageProxy;
		uint64					  mTargetBuildingId;			///< Building to collapse
		uint64					  mExplosionId;					///< ID of the explosion particle entity
		float					  mExplosionRadius;				///< The radius of the explosion
		qsf::Time				  mDamageBuildingTime;			///< Time between explosion and building destruction. Can't start at same time as explosion effects vanish when the building state changes.
		qsf::Time				  mDebrisSpawnTime;				///< Time between building destruction and spawning of debris.
		qsf::Time				  mHint01WaitTime;				///< Time until player get's a hint to do something
		float					  mHint2LifeEnergy;				///< First injured with less life-energy than that triggers a hint
		glm::vec3				  mExplosionCenter;				///< The eye of the storm
		uint32					  mMinDebris;					///< Minimal number of debris entities to spawn
		uint32					  mMaxDebris;					///< Maximal number of debris entities to spawn
		std::vector<uint64>		  mDebris;						///< Entity-ID of debris which is not yet removed
		std::vector<uint64>		  mVictims;						///< Entity-ID of original event victims
		std::string				  mEventTagDebris;				///< Tag for debris with injured persons
		std::string				  mEventTagExplosion;			///< Tag for explosion
		bool					  mSpawnPersonsUnderDebris;		///< When true persons are spawned under debris, otherwise injuired persons without debris are spawned
		qsf::Entity*			  mExplosion;					///< Explosion entity attached to the house
		std::vector<qsf::Entity*> mDebrisPlaceholders;			///< Debris attached to the house (debug objects to have the position for the real debris)
		uint32					  mPlayerActivityObserverId;	///< Observer to check if the player starts reacting on the event


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::BuildingCollapseEvent)
