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
	*    EMERGENCY 5 freeplay event "boat fire"
	*/
	class EM5_API_EXPORT BoatFireEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class BoatFireEventFactory;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::BoatFireEvent" unique freeplay event ID

		typedef std::vector<qsf::Entity*> EntityArray;


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
		BoatFireEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BoatFireEvent();

		void initWithLayer(qsf::Layer* layer);

		/**
		*  @brief
		*    Get the ID of the target entity
		*/
		uint64 getTargetEntityId() const;

		/**
		*  @brief
		*    Set the target entity
		*/
		void setTargetEntity(const qsf::Entity& entity);

		void setBurnShipAtStartup(bool burnShipAtStartup);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PersonsInWater, uint32, mPersonsInWater);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PersonsOnDeck, uint32, mPersonsOnDeck);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onFailure(EventResult& eventResult) override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;
		virtual void hintCallback(Observer& hintObserver) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startObjectives(const qsf::Entity& targetEntity);
		void startHintObservers(const qsf::Entity& targetEntity);
		void onPersonRescued(const qsf::MessageParameters& parameters);
		void onBoatDestroyed(const qsf::MessageParameters& parameters);

		bool startupFromLayer(qsf::Layer& eventLayer);
		void findSpawnPoints();
		void spawnPersonsForRescue();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64				mTargetEntityId;	///< ID of the inflamed boat
		uint32				mPersonsInWater;	///< Number of persons which spawn in water, this is limited by the edited spawn points ("Kutter" = 2, "Frachstschiff" = 4)
		uint32				mPersonsOnDeck;		///< Number of persons which spawn in water, this is limited by the edited spawn points
		EntityArray			mSpawnPointsInWater;
		EntityArray			mSpawnPointsOnDeck;
		uint32				mEventLayerId;		///< Is set in case we initialize the event from a layer, otherwise nullptr
		bool				mBurnShipAtStartup;

		// Intern
		qsf::MessageProxy	mPersonRescueByHeliMessageProxy;	///< Message proxy, that listens if the person is rescued
		qsf::MessageProxy	mPersonRescueByDiverMessageProxy;	///< Message proxy, that listens if the person is rescued
		qsf::MessageProxy	mBoatDestroyedMessageProxy;			///< Message proxy, that the target boat is been destroyed


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::BoatFireEvent)
