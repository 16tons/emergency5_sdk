// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/fire/BoatFireEventFactory.h"
#include "em5/event/standard/fire/BoatFireEvent.h"
#include "em5/event/standard/fire/helper/FireEventCandidateSearchHelper.h"
#include "em5/event/EventHelper.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/game/units/UnitPool.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/map/layer/Layer.h>
#include <qsf/map/layer/LayerManager.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	// Event tags
	namespace detail
	{
		const std::string EVENT_TAG_BOAT = "BOAT";
		const std::string EVENT_TAG_CONTAINER_SHIP = "CONTAINER_SHIP";
		const std::string LAYER_BOAT_FIRE = "WATER_RESCUE"; // With subfolder "WATER_RESCUE_1" ...
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BoatFireEventFactory::BoatFireEventFactory() :
		mPersonsOnDeckChance(0.5f),
		mShipOnFireChance(1.0f),
		mUseOnlyShipsFromLayer(false)
	{
		// Nothing here
	}

	BoatFireEventFactory::~BoatFireEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* BoatFireEventFactory::tryTriggerEvent()
	{
		FreeplayEvent* result = nullptr;
		bool personsOnDeck = qsf::Random::getRandomChance(mPersonsOnDeckChance);
		bool bigShip = false;	// Different event variants have different ways to set this value
		bool shipBurning = qsf::Random::getRandomChance(mShipOnFireChance);	// The boat fire event also has persons in water, so the boat is not always forced to burn

		if (mUseOnlyShipsFromLayer)
		{
			// Here we search only ships in layer

			// If base layer name is not set, set it to default
			if (mBaseLayer.empty())
			{
				mBaseLayer = detail::LAYER_BOAT_FIRE;
			}

			qsf::Layer* eventLayer = findCandidateLayer();
			if (nullptr == eventLayer)
			{
				setTriggerFailReason("Could not find event layer " + mEventLayer);
				return nullptr;
			}

			bigShip = isBigShip(*eventLayer);

			// Create the event instance
			BoatFireEvent& freeplayEvent = createAndInitializeEvent<BoatFireEvent>();

			// Initialize the event
			freeplayEvent.initWithLayer(eventLayer);
			result = &freeplayEvent;
		}
		else
		{
			// Use the ship entities how are already in the map
			// Find a candidate entity
			qsf::Entity* targetEntity = findCandidate();
			if (nullptr == targetEntity)
			{
				setTriggerFailReason("Could not find boat entity with event tag " + getDifficultyTags());
				return nullptr;
			}

			bigShip = isBigShip(*targetEntity);

			// Create the event instance
			BoatFireEvent& freeplayEvent = createAndInitializeEvent<BoatFireEvent>();

			// Initialize the event
			freeplayEvent.setTargetEntity(*targetEntity);
			result = &freeplayEvent;
		}

		if (result != nullptr)
		{
			// 1 = small ship + no persons on deck; 2 = small + persons on deck; 3 = big ship + no persons on deck; 4 = big ship + persons on deck
			int variationIndex = bigShip ? (personsOnDeck ? 4 : 3) : (personsOnDeck ? 2 : 1);
			deserializeFreeplayVariantByIndex(*result, variationIndex);

			static_cast<BoatFireEvent*>(result)->setBurnShipAtStartup(shipBurning);
		}

		// Done
		return result;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* BoatFireEventFactory::findCandidate() const
	{
		// Start the helper
		FireEventCandidateSearchHelper fireEventCandidateSearchHelper;
		fireEventCandidateSearchHelper.setDifficultyTagsByString(mDifficultyTags);

		// Get a listing of all "BOAT" tagged entities
		const auto& tagComponentsInMap = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(detail::EVENT_TAG_BOAT), QSF_MAINMAP);
		ComponentsRandomEnumerator<qsf::game::EventTagComponent> randomEnumerator(tagComponentsInMap);

		while (qsf::game::EventTagComponent* eventTagComponent = randomEnumerator.getNext())
		{
			// Check if difficulty tags are set
			if (!fireEventCandidateSearchHelper.hasDifficultyTag(*eventTagComponent))
				continue;

			// Get the entity
			qsf::Entity& entity = eventTagComponent->getEntity();
			if (checkCandidate(entity))
				return &entity;
		}

		// None found
		return nullptr;
	}

	bool BoatFireEventFactory::checkCandidate(qsf::Entity& entity) const
	{
		const EntityHelper entityHelper(entity);

		// Check if entity is a valid event target
		if (!entityHelper.isEntityValidEventTarget())
			return false;

		// Is boat damaged?
		if (entityHelper.isEntityDamaged())
			return false;

		// Check if boat can be set on fire
		if (!entityHelper.canBeSetOnFireNow())
			return false;

		return true;
	}

	bool BoatFireEventFactory::isBigShip(qsf::Entity& entity) const
	{
		return entity.getOrCreateComponentSafe<qsf::game::EventTagComponent>().hasTag(detail::EVENT_TAG_CONTAINER_SHIP);
	}

	bool BoatFireEventFactory::isBigShip(qsf::Layer& eventLayer)
	{
		qsf::Map& layerBufferMap = eventLayer.getInternalBufferMap();
		auto entities = layerBufferMap.getEntities();
		for (qsf::Entity* targetEntity : entities)
		{
			if (nullptr != targetEntity)
			{
				EntityHelper targetEntityHelper(*targetEntity);
				if (targetEntity->getComponent<BoatComponent>() != nullptr &&
					(targetEntityHelper.checkEntityHasEventTag(detail::EVENT_TAG_BOAT) || targetEntityHelper.checkEntityHasEventTag(detail::EVENT_TAG_CONTAINER_SHIP)))
				{
					return isBigShip(*targetEntity);
				}
			}
		}
		return false;
	}

	bool BoatFireEventFactory::checkIfNeededUnitsAreAvailable() const
	{
		Player* localPlayer = PlayerManager::getLocalPlayer();
		if (nullptr == localPlayer)
			return false;

		// Check if rescue firefighter boat transport is available
		UnitPool& unitPool = localPlayer->getUnitPool();
		if (unitPool.getNumberMaxAvailableByName(CommandableComponent::UNITTAG_FIREFIGHTERS_BOAT_TRANSPORT) <= 0)
		{
			return false;
		}

		// Check if rescue FLB is available
		if (unitPool.getNumberMaxAvailableByName(CommandableComponent::UNITTAG_FIREFIGHTERS_FLB) <= 0)
		{
			return false;
		}

		return true;
	}

	bool BoatFireEventFactory::checkCandidateLayer(const qsf::Layer& layer) const
	{
		// TODO(mk) Function is copied from TrafficAccident01EventFactory::checkCandidate
		if (layer.isActive())
			return false;

		// Check if objects to spawn are collision free
		if (!BoatFireEvent::canSpawnEntitiesCollisionFree(layer, QSF_MAINMAP))
			return false;

		// Players must not see layer activation
		if (EventHelper::isLayerVisible(layer))
			return false;

		return true;
	}

	qsf::Layer* BoatFireEventFactory::findCandidateLayer() const
	{
		// Use standard editing for event location layers
		return getCandidateEventLayer(mEventLayer, mBaseLayer, boost::bind(&BoatFireEventFactory::checkCandidateLayer, this, _1));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
