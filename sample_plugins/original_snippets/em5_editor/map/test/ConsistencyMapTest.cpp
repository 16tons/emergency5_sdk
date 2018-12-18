// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/map/test/ConsistencyMapTest.h"

#include <em5/component/vehicle/RoadVehicleComponent.h>
#include <em5/component/damage/DamageComponent.h>
#include <em5/component/door/DoorComponent.h>
#include <em5/component/building/BuildingComponent.h>
#include <em5/fire/component/FireComponent.h>
#include <em5/fire/component/ComplexFireComponent.h>

#include <qsf_game/component/person/PersonComponent.h>
#include <qsf_game/component/vehicle/VehicleComponent.h>

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/polygon/PolygonComponent.h>
#include <qsf/physics/collision/BulletTriangleMeshCollisionComponent.h>
#include <qsf/localization/LocalizationSystem.h>

#include <boost/format.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		const uint32 ConsistencyMapTest::PLUGINABLE_ID = qsf::StringHash("em5::editor::ConsistencyMapTest");


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		ConsistencyMapTest::ConsistencyMapTest(qsf::editor::MapTestManager* mapTestManager) :
			MapTest(mapTestManager)
		{
			// Nothing to do in here
		}

		ConsistencyMapTest::~ConsistencyMapTest()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::MapTest methods                ]
		//[-------------------------------------------------------]
		void ConsistencyMapTest::run()
		{
			const qsf::Map& map = getMap();
			qsf::ComponentMapQuery componentMapQuery(map);

			// Vehicle
			for (RoadVehicleComponent* roadVehicleComponent : componentMapQuery.getAllInstances<RoadVehicleComponent>())
			{
				for (uint64 doorEntityId : roadVehicleComponent->getVehicleDoors())
				{
					const qsf::Entity* entity = getMap().getEntityById(doorEntityId);
					if (nullptr != entity)
					{
						// Check if the door has the door component
						const DoorComponent* doorComponent = entity->getComponent<DoorComponent>();
						if (nullptr == doorComponent)
						{
							// Text: "Entity %1% has a road vehicle component but no doors linked to it"
							print(qsf::LogMessage::WARNING, (boost::format(QSF_TRANSLATE_STRING(QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_DOORCOMPONENT"))) % entity->getId()).str(), entity);
						}
					}
				}
			}

			// Fire
			for (FireComponent* fireComponent : componentMapQuery.getAllInstances<FireComponent>())
			{
				qsf::Entity& entity = fireComponent->getEntity();

				{ // Request from the level designers: Ignore entities with "_combined" inside the name
					qsf::MetadataComponent* metadataComponent = entity.getComponent<qsf::MetadataComponent>();
					if (nullptr != metadataComponent && metadataComponent->getName().find("_combined") != std::string::npos)
					{
						// Check for out-of-date editing
						if (fireComponent->FireParticlesArray.size() > 0)
						{
							// Text: "Entity %1% is a combined object, but has a fire component with fire particles"
							print(qsf::LogMessage::ERROR, (boost::format(QSF_TRANSLATE_STRING(QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_COMBINEDWITHFIREPARTICLES"))) % entity.getId()).str(), &entity);
						}

						// Ignore this entity
						continue;
					}
				}

				{
					qsf::Prototype& prototype = qsf::LinkComponent::getTopmostAncestorPrototype(entity, qsf::LinkComponent::SELECT_PARENT);
					if (nullptr == prototype.getComponent<DamageComponent>())
					{
						// Text: "Entity %1% has a fire component but no damage component at the topmost entity"
						print(qsf::LogMessage::ERROR, (boost::format(QSF_TRANSLATE_STRING(QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_NODAMAGECOMPONENT"))) % entity.getId()).str(), &entity);
					}
				}

				{ // In case this fire component is part of a complex fire, it does not need to have linked particle effects
					bool partOfComplexFire = false;

					qsf::LinkComponent* linkComponent = entity.getComponent<qsf::LinkComponent>();
					if (nullptr != linkComponent)
					{
						qsf::LinkComponent* parentLinkComponent = linkComponent->getParentLinkComponent();
						if (nullptr != parentLinkComponent)
						{
							ComplexFireComponent* parentComplexFireComponent = parentLinkComponent->getEntity().getComponent<ComplexFireComponent>();
							if (nullptr != parentComplexFireComponent)
							{
								const std::vector<uint64>& entityIds = parentComplexFireComponent->FireEntityIdArray.getVariable();
								if (std::find(entityIds.begin(), entityIds.end(), entity.getId()) != entityIds.end())
								{
									partOfComplexFire = true;
								}
							}
						}
					}

					if (!partOfComplexFire)
					{
						if (fireComponent->FireParticlesArray.size())
						{
							// Check whether or not there's at least one entity inside the list which does not exist
							bool atLeastOneThere = false;
							for (uint64 linkedEntityId : fireComponent->FireParticlesArray.getVariable())
							{
								// TODO(co) Would be nice if we could make a "tell about each zombie"-check. At the moment, we get too many hits with this.
								/*
								if (nullptr == map.getEntityById(linkedEntityId))
								{
									// Text: "Entity %1% has a fire component and links to entity %2% as fire particle, an entity which does not exist"
									print(qsf::LogMessage::ERROR, (boost::format(QSF_TRANSLATE_STRING(QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_FIREPARTICLEENTITYDOESNOTEXIST"))) % entity.getId() % linkedEntityId).str(), &entity);
								}
								*/

								if (nullptr != map.getEntityById(linkedEntityId))
								{
									atLeastOneThere = true;
									break;
								}
							}

							if (!atLeastOneThere)
							{
								// Text: "Entity %1% has a fire component, but links only to no longer existing fire particle entities"
								print(qsf::LogMessage::ERROR, (boost::format(QSF_TRANSLATE_STRING(QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_NOEXISTINGFIREPARTICLES"))) % entity.getId()).str(), &entity);
							}
						}
						else
						{
							// Text: "Entity %1% has a fire component, is not part of a complex fire, but no fire particles are linked"
							print(qsf::LogMessage::ERROR, (boost::format(QSF_TRANSLATE_STRING(QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_NOFIREPARTICLES"))) % entity.getId()).str(), &entity);
						}
					}
				}
			}
			for (ComplexFireComponent* complexFireComponent : componentMapQuery.getAllInstances<ComplexFireComponent>())
			{
				qsf::Entity& entity = complexFireComponent->getEntity();

				{ // Request from the level designers: Ignore entities with "_combined" inside the name
					qsf::MetadataComponent* metadataComponent = entity.getComponent<qsf::MetadataComponent>();
					if (nullptr != metadataComponent && metadataComponent->getName().find("_combined") != std::string::npos)
					{
						// Ignore this entity
						continue;
					}
				}

				{
					qsf::Prototype& prototype = qsf::LinkComponent::getTopmostAncestorPrototype(entity, qsf::LinkComponent::SELECT_PARENT);
					if (nullptr == prototype.getComponent<DamageComponent>())
					{
						// Text: "Entity %1% has a fire component but no damage component at the topmost entity"
						print(qsf::LogMessage::ERROR, (boost::format(QSF_TRANSLATE_STRING(QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_NODAMAGECOMPONENT"))) % entity.getId()).str(), &entity);
					}
				}

				if (complexFireComponent->FireEntityIdArray.size())
				{
					// Check whether or not there's at least one entity inside the list which does not exist
					bool atLeastOneThere = false;
					for (uint64 linkedEntityId : complexFireComponent->FireEntityIdArray.getVariable())
					{
						// TODO(co) Would be nice if we could make a "tell about each zombie"-check. At the moment, we get too many hits with this.
						/*
						if (nullptr == map.getEntityById(linkedEntityId))
						{
							// Text: "Entity %1% has a complex fire component and links to entity %2% as fire entity, an entity which does not exist"
							print(qsf::LogMessage::ERROR, (boost::format(QSF_TRANSLATE_STRING(QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_COMPLEXFIREENTITYDOESNOTEXIST"))) % entity.getId() % linkedEntityId).str(), &entity);
						}
						*/

						if (nullptr != map.getEntityById(linkedEntityId))
						{
							atLeastOneThere = true;
							break;
						}
					}

					if (!atLeastOneThere)
					{
						// Text: "Entity %1% has a complex fire component, but links only to no longer existing fire entities"
						print(qsf::LogMessage::ERROR, (boost::format(QSF_TRANSLATE_STRING(QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_NOEXISTINGCOMPLEXFIREENTITY"))) % entity.getId()).str(), &entity);
					}
				}
				else
				{
					// Text: "Entity %1% has a complex fire component but no fire entities are linked"
					print(qsf::LogMessage::ERROR, (boost::format(QSF_TRANSLATE_STRING(QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_NOCOMPLEXFIREENTITIES"))) % entity.getId()).str(), &entity);
				}

				if (complexFireComponent->FireParticleIdArray.size())
				{
					// Check whether or not there's at least one entity inside the list which does not exist
					bool atLeastOneThere = false;
					for (uint64 linkedEntityId : complexFireComponent->FireParticleIdArray.getVariable())
					{
						// TODO(co) Would be nice if we could make a "tell about each zombie"-check. At the moment, we get too many hits with this.
						/*
						if (nullptr == map.getEntityById(linkedEntityId))
						{
							// Text: "Entity %1% has a complex fire component and links to entity %2% as fire particle, an entity which does not exist"
							print(qsf::LogMessage::ERROR, (boost::format(QSF_TRANSLATE_STRING(QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_COMPLEXFIREPARTICLEENTITYDOESNOTEXIST"))) % entity.getId() % linkedEntityId).str(), &entity);
						}
						*/

						if (nullptr != map.getEntityById(linkedEntityId))
						{
							atLeastOneThere = true;
							break;
						}
					}

					if (!atLeastOneThere)
					{
						// Text: "Entity %1% has a complex fire component, but links only to no longer existing fire particle entities"
						print(qsf::LogMessage::ERROR, (boost::format(QSF_TRANSLATE_STRING(QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_NOEXISTINGCOMPLEXFIREPARTICLES"))) % entity.getId()).str(), &entity);
					}
				}
				else
				{
					// Text: "Entity %1% has a complex fire component but no fire particles are linked"
					print(qsf::LogMessage::ERROR, (boost::format(QSF_TRANSLATE_STRING(QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_NOCOMPLEXFIREPARTICLES"))) % entity.getId()).str(), &entity);
				}
			}

			// Damage component
			for (DamageComponent* damageComponent : componentMapQuery.getAllInstances<DamageComponent>())
			{
				if (damageComponent->getDamageState() != DamageComponent::DAMAGESTATE_INTACT)
				{
					// Text: "Entity %1% has a damage component which is not initially set to intact. Is this intentional?"
					const qsf::Entity& entity = damageComponent->getEntity();
					print(qsf::LogMessage::WARNING, (boost::format(QSF_TRANSLATE_STRING(QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_DAMAGECOMPONENTNOTINTACT"))) % entity.getId()).str(), &entity);
				}
			}

			{ // Door components of entities in a door slot of buildings should not have an offset, they can be placed anywhere anyway
				for (BuildingComponent* buildingComponent : componentMapQuery.getAllInstances<BuildingComponent>())
				{
					qsf::Entity* doorEntity = map.getEntityById(buildingComponent->getMainDoorId());
					if (nullptr != doorEntity)
					{
						DoorComponent* doorComponent = doorEntity->getComponent<DoorComponent>();
						if (nullptr != doorComponent)
						{
							if (doorComponent->getDoorOffset() != qsf::Math::GLM_VEC3_ZERO)
							{
								// Text: "Entity with ID %1% has a door in a door slot of a building component with a useless door offset not being zero"
								const qsf::Entity& entity = buildingComponent->getEntity();
								print(qsf::LogMessage::WARNING, (boost::format(QSF_TRANSLATE_STRING(QT_TR_NOOP("ID_QSFEDITOR_MAPTEST_CONSISTENCY_BUILDINGWITHDOOROFFSET"))) % entity.getId()).str(), &entity);
							}
						}
					}
				}
			}

			// Navigation component
			for (qsf::ai::NavigationComponent* navigationComponent : componentMapQuery.getAllInstances<qsf::ai::NavigationComponent>())
			{
				// Ignore disabled navigation components
				if (navigationComponent->isActive())
				{
					qsf::Entity& entity = navigationComponent->getEntity();

					// There must be a person or vehicle component
					//  -> Note that "em5::VehicleComponent" is not derived from "qsf::game::VehicleComponent", as this would make the inheritance hierarchy there fairly complicated
					if (nullptr == entity.getComponent<qsf::game::PersonComponent>() &&
						nullptr == entity.getComponent<qsf::game::VehicleComponent>() &&
						nullptr == entity.getComponent<em5::VehicleComponent>())
					{
						// Text: "Entity %1% has a navigation component but no person or vehicle component"
						print(qsf::LogMessage::ERROR, (boost::format(QSF_TRANSLATE_STRING(QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_AI_NOREQUIREDCOMPONENT"))) % entity.getId()).str(), &entity);
					}

					// No polygon components allowed
					if (nullptr != entity.getComponent<qsf::PolygonComponent>())
					{
						// Text: "Entity %1% has a navigation component and also a polygon component"
						print(qsf::LogMessage::ERROR, (boost::format(QSF_TRANSLATE_STRING(QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_AI_POLYGONCOMPONENT"))) % entity.getId()).str(), &entity);
					}

					// No bullet triangle mesh collision components allowed
					if (nullptr != entity.getComponent<qsf::BulletTriangleMeshCollisionComponent>())
					{
						// Text: "Entity %1% has a navigation component and also a bullet triangle mesh collision component"
						print(qsf::LogMessage::ERROR, (boost::format(QSF_TRANSLATE_STRING(QT_TR_NOOP("ID_EM5EDITOR_MAPTEST_AI_BULLETTRIANGLEMESHCOLLISIONCOMPONENT"))) % entity.getId()).str(), &entity);
					}
				}
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
