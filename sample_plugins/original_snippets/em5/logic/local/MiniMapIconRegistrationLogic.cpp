// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/objects/BoatDropPointComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/fire/component/ComplexFireComponent.h"
#include "em5/health/HealthComponent.h"
#include "em5/game/Game.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/gui/IngameHud.h"
#include "em5/gui/hud/MiniMapWindow.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/renderer/MiniMapRenderer.h"
#include "em5/EM5Helper.h"

#include <qsf_game/plugin/Messages.h>

#include <qsf/component/polygon/LiquidPolygonComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Locally used definitions                              ]
	//[-------------------------------------------------------]
	namespace
	{
		// All sizes expressed in pixels
		const float MINIMAP_ICON_SIZE_UNIT	= 16;
		const float MINIMAP_ICON_SIZE_BLINK	= 128;
		const float MINIMAP_ICON_SIZE_FIRE	= 16;
		const float MINIMAP_ICON_SIZE_EVENT	= 10;
	}


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	uint64 MiniMapIconRegistrationLogic::getHeadquarterEntityId()
	{
		// TODO(fw): Maybe a better solution would be defining a head quarter component that creates the icon

		// Search the HQ: It's the one object with the HQ mesh
		static const qsf::AssetProxy hqMesh("em5/mesh/default/building_headquarter");
		for (qsf::MeshComponent* meshComponent : qsf::ComponentMapQuery(QSF_MAINMAP).getAllInstances<qsf::MeshComponent>())
		{
			if (meshComponent->getMesh() == hqMesh)
			{
				return meshComponent->getEntityId();
			}
		}

		// No headquarter found
		return qsf::getUninitialized<uint64>();
	}

	MiniMapIconType MiniMapIconRegistrationLogic::translateCommandableMiniMapIconType(const qsf::StringHash& commandableMiniMapIconType)
	{
		static const boost::container::flat_map<uint32, MiniMapIconType> mapping =
		{
			// Persons (and dogs)
			{ CommandableComponent::MINIMAP_ICON_UNIT_AMBULANCE,			MiniMapIconType::UNIT_AMBULANCE },
			{ CommandableComponent::MINIMAP_ICON_UNIT_FIREFIGHTER,			MiniMapIconType::UNIT_FIREFIGHTER },
			{ CommandableComponent::MINIMAP_ICON_UNIT_POLICE,				MiniMapIconType::UNIT_POLICE },
			{ CommandableComponent::MINIMAP_ICON_UNIT_TECH,					MiniMapIconType::UNIT_TECH },
			{ CommandableComponent::MINIMAP_ICON_UNIT_AMBULANCE_DOG,		MiniMapIconType::UNIT_AMBULANCE_DOG },
			{ CommandableComponent::MINIMAP_ICON_UNIT_POLICE_DOG,			MiniMapIconType::UNIT_POLICE_DOG },
			{ CommandableComponent::MINIMAP_ICON_UNIT_MEDIEVAL,				MiniMapIconType::UNIT_MEDIEVAL },

			// Vehicles
			{ CommandableComponent::MINIMAP_ICON_UNIT_VEHICLE_AMBULANCE,	MiniMapIconType::UNIT_VEHICLE_AMBULANCE },
			{ CommandableComponent::MINIMAP_ICON_UNIT_VEHICLE_FIREFIGHTER,	MiniMapIconType::UNIT_VEHICLE_FIREFIGHTER },
			{ CommandableComponent::MINIMAP_ICON_UNIT_VEHICLE_POLICE,		MiniMapIconType::UNIT_VEHICLE_POLICE },
			{ CommandableComponent::MINIMAP_ICON_UNIT_VEHICLE_TECH,			MiniMapIconType::UNIT_VEHICLE_TECH }
		};

		const auto iterator = mapping.find(commandableMiniMapIconType);
		if (iterator == mapping.end())
		{
			return MiniMapIconType::UNDEFINED;
		}
		else
		{
			return iterator->second;
		}
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MiniMapIconRegistrationLogic::MiniMapIconRegistrationLogic() :
		mShowAllHarborIcons(false),
		mMiniMapRenderer(nullptr)
	{
		// Nothing here
	}

	MiniMapIconRegistrationLogic::~MiniMapIconRegistrationLogic()
	{
		// Nothing here
	}

	void MiniMapIconRegistrationLogic::onStartup()
	{
		MiniMapWindow* minimap = EM5_GUI.getIngameHud().getMiniMapWindow();
		mMiniMapRenderer = (nullptr != minimap) ? &minimap->getRenderer() : nullptr;

		mMessageProxySpawnUnit.registerAt(qsf::MessageConfiguration(Messages::EM5_SPAWN_UNIT), boost::bind(&MiniMapIconRegistrationLogic::onSpawnUnit, this, _1));
		mMessageProxyEnterVehicle.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_PERSON_ENTER_VEHICLE), boost::bind(&MiniMapIconRegistrationLogic::onRemoveIcon, this, _1));
		mMessageProxyPersonGetsRescuedFromCar.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_TOOKOUTSIDE_PERSON), boost::bind(&MiniMapIconRegistrationLogic::onPersonGetsRescuedFromCar, this, _1));

		// Fire
		mMessageProxyStartFire.registerAt(qsf::MessageConfiguration(Messages::EM5_OBJECT_START_BURNING), boost::bind(&MiniMapIconRegistrationLogic::onStartFire, this, _1));
		mMessageProxyStopFire.registerAt(qsf::MessageConfiguration(Messages::EM5_OBJECT_STOP_BURNING), boost::bind(&MiniMapIconRegistrationLogic::onStopFire, this, _1));

		// Injured person: Pickup by paramedic
		mMessageProxyPickupPersonByParamedic.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_PICKUP_PERSON_BY_PARAMEDIC), boost::bind(&MiniMapIconRegistrationLogic::onRemoveIcon, this, _1));

		// Gangster
		mMessageProxyGangsterEscaped.registerAt(qsf::MessageConfiguration(Messages::EM5_GANGSTER_ESCAPED), boost::bind(&MiniMapIconRegistrationLogic::onRemoveIcon, this, _1));
		mMessageProxyArrestGangster.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_ARREST), boost::bind(&MiniMapIconRegistrationLogic::onRemoveIcon, this, _1));
		// TODO(db): Is this still necessary?
		mMessageProxyPlacedInPolice.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_PERSON_PLACED_IN_POLICE), boost::bind(&MiniMapIconRegistrationLogic::onPlacePersonOnPolice, this, _1));

		mMessageProxyFindHiddenPerson.registerAt(qsf::MessageConfiguration(Messages::EM5_RESCUE_DOG_FOUND_PERSON), boost::bind(&MiniMapIconRegistrationLogic::onFindHiddenPerson, this, _1));
		mMessageProxyFindHiddenPersonWithSEKDrone.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_FIND_HIDDEN_PERSON_WITH_SEK_DRONE), boost::bind(&MiniMapIconRegistrationLogic::onFindHiddenPersonWithSEKDrone, this, _1));
		mMessageProxyPickedUpByTowCar.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_CAR_PICKED_UP_BY_TOWCAR), boost::bind(&MiniMapIconRegistrationLogic::onRemoveIcon, this, _1));

		// Load unload for fire boat and SEKR
		mMessageProxyLoadVehicleToTransporter.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_LOAD_VEHICLE_TO_TRANSPORTER), boost::bind(&MiniMapIconRegistrationLogic::onRemoveIcon, this, _1));
		mMessageProxyUnLoadVehicleFromTransporter.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_UNLOAD_VEHICLE_FROM_TRANSPORTER), boost::bind(&MiniMapIconRegistrationLogic::onSpawnUnit, this, _1));
		mMessageProxyOnSelectionChange.registerAt(qsf::MessageConfiguration(Messages::EM5_CHANGE_SELECTION), boost::bind(&MiniMapIconRegistrationLogic::onSelectionChange, this, _1));

		// Generic add/remove icon messages for those cases where icons were directly added in a multiplayer-incompatible way
		mMessageProxyMinimapAddIcon.registerAt(qsf::MessageConfiguration(Messages::EM5_MINIMAP_ADD_ENTITY_ICON), boost::bind(&MiniMapIconRegistrationLogic::onAddIcon, this, _1));
		mMessageProxyMinimapAddCirclePolygon.registerAt(qsf::MessageConfiguration(Messages::EM5_MINIMAP_ADD_ENTITY_CIRCLE_POLYGON), boost::bind(&MiniMapIconRegistrationLogic::onAddCirclePolygon, this, _1));
		mMessageProxyMinimapRemoveCirclePolygon.registerAt(qsf::MessageConfiguration(Messages::EM5_MINIMAP_REMOVE_ENTITY_CIRCLE_POLYGON), boost::bind(&MiniMapIconRegistrationLogic::onRemoveCirclePolygon, this, _1));
		mMessageProxyMinimapRemoveIcon.registerAt(qsf::MessageConfiguration(Messages::EM5_MINIMAP_REMOVE_ENTITY_ICON), boost::bind(&MiniMapIconRegistrationLogic::onRemoveIcon, this, _1));

		// Add place person messages
		// TODO(co) The comment above looks sad and alone, is there anything missing here to make it happy?

		{ // Add HQ icon
			const uint64 headquarterEntityId = getHeadquarterEntityId();
			if (qsf::isInitialized(headquarterEntityId))
			{
				addIconLocally(headquarterEntityId, MiniMapIconType::HQ, MINIMAP_ICON_SIZE_UNIT);	// Use default size
			}
		}
	}

	void MiniMapIconRegistrationLogic::onShutdown()
	{
		mMessageProxySpawnUnit.unregister();
		mMessageProxyDespawnUnit.unregister();
		mMessageProxyEnterVehicle.unregister();
		mMessageProxyEnterBuilding.unregister();
		mMessageProxyExitBuilding.unregister();
		mMessageProxyStartFire.unregister();
		mMessageProxyStopFire.unregister();
		mMessageProxyGangsterEscaped.unregister();
		mMessageProxyPlacedInPolice.unregister();
		mMessageProxyFindHiddenPerson.unregister();
		mMessageProxyFindHiddenPersonWithSEKDrone.unregister();
		mMessageProxyPickedUpByTowCar.unregister();
		mMessageProxyLoadVehicleToTransporter.unregister();
		mMessageProxyUnLoadVehicleFromTransporter.unregister();
		mMessageProxyPickupPersonByParamedic.unregister();
		mMessageProxyArrestGangster.unregister();
		mMessageProxyOnSelectionChange.unregister();
		mMessageProxyMinimapAddIcon.unregister();
		mMessageProxyMinimapAddCirclePolygon.unregister();
		mMessageProxyMinimapRemoveCirclePolygon.unregister();
		mMessageProxyMinimapRemoveIcon.unregister();
	}

	void MiniMapIconRegistrationLogic::setPathVisualizationEntityId(uint64 entityId)
	{
		if (mMiniMapRenderer != nullptr)
		{
			mMiniMapRenderer->setPathVisualizationEntityId(entityId);
		}
	}

	void MiniMapIconRegistrationLogic::registerPolygon(uint64 entityId, const qsf::Color4& color)
	{
		if (mMiniMapRenderer != nullptr)
		{
			mMiniMapRenderer->addPolygon(entityId, color, false);
		}
	}

	void MiniMapIconRegistrationLogic::registerPolygon(uint64 entityId, const qsf::Color4& color, const std::vector<glm::vec3>& vertices)
	{
		if (mMiniMapRenderer != nullptr)
		{
			mMiniMapRenderer->addPolygon(entityId, color, vertices);
		}
	}

	void MiniMapIconRegistrationLogic::registerCirclePolygon(uint64 entityId, float radius, uint32 points, const qsf::Color4& color, bool updatePosition) const
	{
		addCirclePolygon(entityId, radius, points, color, updatePosition);
	}

	void MiniMapIconRegistrationLogic::unregisterPolygon(uint64 entityId)
	{
		qsf::MessageParameters parameters;
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_MINIMAP_REMOVE_ENTITY_CIRCLE_POLYGON, entityId), parameters);
	}

	void MiniMapIconRegistrationLogic::registerIcon(uint64 entityId, MiniMapIconType minimapIconType, bool onlyForLocalPlayer) const
	{
		if (onlyForLocalPlayer)
			addIconLocally(entityId, minimapIconType, MINIMAP_ICON_SIZE_UNIT);
		else
			addIcon(entityId, minimapIconType, MINIMAP_ICON_SIZE_UNIT);
	}

	void MiniMapIconRegistrationLogic::registerEventIcon(uint64 entityId) const
	{
		addIcon(entityId, MiniMapIconType::EVENT, MINIMAP_ICON_SIZE_UNIT);
	}

	void MiniMapIconRegistrationLogic::registerHighlightIcon(const glm::vec3& worldPosition) const
	{
		if (mMiniMapRenderer != nullptr)
		{
			mMiniMapRenderer->addHighlightAtWorldPosition(worldPosition, MINIMAP_ICON_SIZE_BLINK);
		}
	}

	void MiniMapIconRegistrationLogic::registerHighlightIcon(uint64 entityId) const
	{
		addIconLocally(entityId, MiniMapIconType::BLINK, MINIMAP_ICON_SIZE_BLINK);
	}

	void MiniMapIconRegistrationLogic::registerBombIcon(uint64 entityId) const
	{
		addIconLocally(entityId, MiniMapIconType::BOMB, MINIMAP_ICON_SIZE_UNIT);
	}

	void MiniMapIconRegistrationLogic::registerGangsterIcon(uint64 entityId) const
	{
		addIcon(entityId, MiniMapIconType::GANGSTER, MINIMAP_ICON_SIZE_UNIT);
	}

	void MiniMapIconRegistrationLogic::registerSearchIcon(uint64 entityId) const
	{
		addIconLocally(entityId, MiniMapIconType::SEARCH, MINIMAP_ICON_SIZE_UNIT);
	}

	void MiniMapIconRegistrationLogic::unregisterIcon(uint64 entityId, bool onlyForLocalPlayer) const
	{
		if (onlyForLocalPlayer)
			removeIconLocally(entityId);
		else
			removeIcon(entityId);
	}

	void MiniMapIconRegistrationLogic::unregisterIcon(const qsf::Entity& entity, bool onlyForLocalPlayer) const
	{
		unregisterIcon(entity.getId(), onlyForLocalPlayer);
	}

	void MiniMapIconRegistrationLogic::registerUnitIcon(qsf::Entity& entity) const
	{
		CommandableComponent* commandableComponent = checkUnit(entity);
		if (nullptr != commandableComponent)
		{
			const MiniMapIconType miniMapIconType = translateCommandableMiniMapIconType(qsf::StringHash(commandableComponent->getMiniMapIconType()));
			if (miniMapIconType != MiniMapIconType::UNDEFINED)
			{
				addIconLocally(entity.getId(), miniMapIconType, MINIMAP_ICON_SIZE_UNIT);
			}
		}
	}

	void MiniMapIconRegistrationLogic::showAllHarborIcons(bool show)
	{
		if (mShowAllHarborIcons != show)
		{
			mShowAllHarborIcons = show;

			const auto& boatDropPointComponents = qsf::ComponentMapQuery(QSF_MAINMAP).getAllInstances<BoatDropPointComponent>();
			if (show)
			{
				for (BoatDropPointComponent* boatDropPointComponent : boatDropPointComponents)
				{
					addIconLocally(boatDropPointComponent->getEntityId(), MiniMapIconType::HARBOR, MINIMAP_ICON_SIZE_UNIT);
				}
			}
			else
			{
				for (BoatDropPointComponent* boatDropPointComponent : boatDropPointComponents)
				{
					removeIconLocally(boatDropPointComponent->getEntityId());
				}
			}
		}
	}

	void MiniMapIconRegistrationLogic::registerPingByWorldPosition(const glm::vec3& worldPosition, bool cyclesAnimation, bool playSound)
	{
		if (mMiniMapRenderer != nullptr)
		{
			mMiniMapRenderer->addPingAtWorldPosition(worldPosition, cyclesAnimation, playSound);
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void MiniMapIconRegistrationLogic::addIcon(uint64 entityId, MiniMapIconType iconType, float scaleOrMaxSize) const
	{
		qsf::MessageParameters parameters;
		parameters.setParameter("IconType", iconType);
		parameters.setParameter("IconScale", scaleOrMaxSize);
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_MINIMAP_ADD_ENTITY_ICON, entityId), parameters);
	}

	void MiniMapIconRegistrationLogic::addCirclePolygon(uint64 entityId, float radius, uint32 points, const qsf::Color4& color, bool updatePosition) const
	{
		qsf::MessageParameters parameters;
		parameters.setParameter("Radius", radius);
		parameters.setParameter("Points", points);
		parameters.setParameter("Color", color.toUInt32());
		parameters.setParameter("UpdatePosition", updatePosition);
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_MINIMAP_ADD_ENTITY_CIRCLE_POLYGON, entityId), parameters);
	}

	void MiniMapIconRegistrationLogic::removeIcon(uint64 entityId) const
	{
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_MINIMAP_REMOVE_ENTITY_ICON, entityId));
	}

	bool MiniMapIconRegistrationLogic::addIconLocally(uint64 entityId, MiniMapIconType type, float scaleOrMaxSize) const
	{
		return (mMiniMapRenderer != nullptr) ? mMiniMapRenderer->addIcon(entityId, type, scaleOrMaxSize) : false;
	}

	void MiniMapIconRegistrationLogic::addCirclePolygonLocally(uint64 entityId, float radius, uint32 points, const qsf::Color4& color, bool updatePosition) const
	{
		// Get the entity
		qsf::Entity* entity = QSF_MAINMAP.getEntityById(entityId);
		if (nullptr == entity)
			return;

		// Set nodes
		const float pi = glm::pi<float>();
		std::vector<qsf::Node> nodes(points);
		for (uint32 index = 0; index < points; ++index)
		{
			float angle = ((2 * pi) / points) * index;
			nodes[index].setPosition(glm::vec3(std::sin(angle) * radius, 0.0f, std::cos(angle) * radius));
		}
		entity->getOrCreateComponentSafe<qsf::PolygonComponent>().setNodes(nodes);

		// Add polygon icon
		if (mMiniMapRenderer != nullptr)
		{
			mMiniMapRenderer->addPolygon(entityId, color, updatePosition);
		}
	}

	void MiniMapIconRegistrationLogic::removeIconLocally(uint64 entityId) const
	{
		if (mMiniMapRenderer != nullptr)
		{
			mMiniMapRenderer->removeIcon(entityId);
		}
	}

	CommandableComponent* MiniMapIconRegistrationLogic::checkUnit(qsf::Entity& unitEntity) const
	{
		// If the entity has a container entity it's inside a vehicle/building and no icon is drawn
		EntityHelper entityHelper(unitEntity);
		if (entityHelper.getContainerEntity() != nullptr)
			return nullptr;

		CommandableComponent* commandableComponent = unitEntity.getComponent<CommandableComponent>();
		if (commandableComponent == nullptr)
			return nullptr;

		if (!commandableComponent->isSimulating())
			return nullptr;

		return commandableComponent;
	}

	bool MiniMapIconRegistrationLogic::checkFire(qsf::Entity* fireEntity) const
	{
		if (fireEntity == nullptr)
			return false;

		EntityHelper entityHelper(*fireEntity);

		// Check if the entity is not inside a container
		if (entityHelper.getContainerEntity() != nullptr)
			return false;

		if (entityHelper.isBurning())
			return true;

		return true;
	}

	bool MiniMapIconRegistrationLogic::checkIsHiddenPerson(qsf::Entity* entity) const
	{
		return (entity != nullptr) ? EntityHelper(*entity).isEntityHidden() : false;
	}

	bool MiniMapIconRegistrationLogic::checkInjuredPerson(qsf::Entity* entity) const
	{
		return (entity != nullptr) ? EntityHelper(*entity).isPersonInjured() : false;
	}

	// Message reactions
	void MiniMapIconRegistrationLogic::onSpawnUnit(const qsf::MessageParameters& parameters)
	{
		qsf::Entity* entity = QSF_MAINMAP.getEntityById(parameters.getFilter(1));
		if (nullptr != entity)
		{
			registerUnitIcon(*entity);
		}
	}

	void MiniMapIconRegistrationLogic::onStartFire(const qsf::MessageParameters& parameters)
	{
		uint64 entityId = parameters.getFilter(1);

		qsf::Entity* entity = QSF_MAINMAP.getEntityById(entityId);
		if (!checkFire(entity))
			return;

		// Unregister previous icons
		removeIconLocally(entityId);

		if (EntityHelper(*entity).isEntityBuilding())
		{
			addIconLocally(entityId, MiniMapIconType::BUILDING_ON_FIRE, MINIMAP_ICON_SIZE_FIRE);
		}
		else
		{
			addIconLocally(entityId, MiniMapIconType::FIRE, MINIMAP_ICON_SIZE_FIRE);
		}
	}

	void MiniMapIconRegistrationLogic::onStopFire(const qsf::MessageParameters& parameters)
	{
		const uint64 entityId = parameters.getFilter(1);

		qsf::Entity* entity = QSF_MAINMAP.getEntityById(entityId);
		if (!checkFire(entity))
			return;

		removeIconLocally(entityId);

		// Add new icon if the burning object was a vehicle and is now destroyed
		EntityHelper entityHelper(*entity);
		if (entityHelper.isVehicleDestroyed())
		{
			// Must be a vehicle (road/helicopter/ship vehicle)
			const VehicleComponent* vehicleComponent = entity->getComponent<VehicleComponent>();
			if (nullptr == vehicleComponent || vehicleComponent->getCanBeTowed())
			{
				addIconLocally(entityId, MiniMapIconType::DEFECT_VEHICLE, MINIMAP_ICON_SIZE_UNIT);
			}
		}
	}

	void MiniMapIconRegistrationLogic::onPersonGetsRescuedFromCar(const qsf::MessageParameters& parameters)
	{
		uint64 entityId = parameters.getFilter(1);

		qsf::Entity* entity = QSF_MAINMAP.getEntityById(entityId);
		if (nullptr == entity)
			return;

		// Unregister any icons for this entity in case the entity has already one (e.g. gangster)
		removeIconLocally(entityId);

		if (checkInjuredPerson(entity))
		{
			HealthComponent* healthComponent = entity->getComponent<HealthComponent>();
			if (nullptr != healthComponent)
			{
				// Since we explicitly removed the mini map icon above, we need to force an update even if there's no internal state change
				healthComponent->updateInjuredIcon(true);
			}
		}
	}

	void MiniMapIconRegistrationLogic::onPlacePersonOnPolice(const qsf::MessageParameters& parameters)
	{
		uint64 entityId = qsf::getUninitialized<uint64>();
		if (parameters.getParameter("personId", entityId))	// "actorId" currently not used
		{
			unregisterIcon(entityId);
		}
	}

	void MiniMapIconRegistrationLogic::onFindHiddenPerson(const qsf::MessageParameters& parameters)
	{
		uint64 entityId = parameters.getFilter(1);

		qsf::Entity* entity = QSF_MAINMAP.getEntityById(entityId);
		if (nullptr != entity)
		{
			registerPingByWorldPosition(EntityHelper(*entity).getPosition());

			if (checkInjuredPerson(entity))
			{
				addIconLocally(entityId, MiniMapIconType::VICTIM_INJURED, MINIMAP_ICON_SIZE_UNIT);
			}
		}
	}

	void MiniMapIconRegistrationLogic::onFindHiddenPersonWithSEKDrone(const qsf::MessageParameters& parameters)
	{
		const uint64 entityId = parameters.getFilter(1);
		qsf::Entity* entity = QSF_MAINMAP.getEntityById(entityId);
		if (nullptr != entity && EntityHelper(*entity).isGangsterPerson())
		{
			addIconLocally(entityId, MiniMapIconType::GANGSTER, MINIMAP_ICON_SIZE_UNIT);
		}
	}

	void MiniMapIconRegistrationLogic::onSelectionChange(const qsf::MessageParameters& parameters)
	{
		SelectionManager& selectionManager = EM5_GAME.getSelectionManager();

		bool isFireFighterBoat = false;
		bool isFireFighterBoatTransport = false;

		if (selectionManager.getSelectionSize() == 0)
		{
			// No boat or boat transport vehicle so unregister the harbor icons
			showAllHarborIcons(false);
			return;
		}

		for (uint64 selectedEntityId : selectionManager.getSelectedIdSet())
		{
			if (qsf::isInitialized(selectedEntityId))
			{
				qsf::Entity* selectedEntity = QSF_MAINMAP.getEntityById(selectedEntityId);
				if (nullptr != selectedEntity)
				{
					CommandableComponent* commandableComponent = selectedEntity->getComponent<CommandableComponent>();
					if (nullptr != commandableComponent)
					{
						if (commandableComponent->isFireFighterBoatTransport())
						{
							isFireFighterBoatTransport = true;
						}
						else if (commandableComponent->isFireFighterBoat())
						{
							isFireFighterBoat = true;
						}
					}
				}
			}
		}

		if (isFireFighterBoat || isFireFighterBoatTransport)
		{
			// If boat or boat transport is selected view the harbor icon
			showAllHarborIcons(true);
		}
		else
		{
			// No boat or boat transport vehicle so unregister the harbor icons
			showAllHarborIcons(false);
		}
	}

	void MiniMapIconRegistrationLogic::onAddIcon(const qsf::MessageParameters& parameters)
	{
		MiniMapIconType iconType = MiniMapIconType::QUESTION_MARK;
		float iconScale = MINIMAP_ICON_SIZE_UNIT;

		// No need to check if entity exists
		parameters.getParameter("IconType", iconType);
		parameters.getParameter("IconScale", iconScale);
		addIconLocally(parameters.getFilter(1), iconType, iconScale);
	}

	void MiniMapIconRegistrationLogic::onAddCirclePolygon(const qsf::MessageParameters& parameters)
	{
		float radius = qsf::getUninitialized<float>();
		uint32 points = qsf::getUninitialized<uint32>();
		qsf::Color4 color;
		uint32 colorAsUint32 = color.toUInt32();
		bool updatePosition = qsf::getUninitialized<bool>();

		// No need to check if entity exists
		parameters.getParameter("Radius", radius);
		parameters.getParameter("Points", points);
		parameters.getParameter("Color", colorAsUint32);
		parameters.getParameter("UpdatePosition", updatePosition);
		color.fromUInt32(colorAsUint32);
		addCirclePolygonLocally(parameters.getFilter(1), radius, points, color, updatePosition);
	}

	void MiniMapIconRegistrationLogic::onRemoveCirclePolygon(const qsf::MessageParameters& parameters)
	{
		if (nullptr != mMiniMapRenderer)
		{
			// No need to check if entity exists
			mMiniMapRenderer->removePolygon(parameters.getFilter(1));
		}
	}

	void MiniMapIconRegistrationLogic::onRemoveIcon(const qsf::MessageParameters& parameters)
	{
		// No need to check if entity exists
		removeIconLocally(parameters.getFilter(1));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
