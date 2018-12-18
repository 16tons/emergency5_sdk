// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/map/test/CollisionMapTest.h"

#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/job/JobArguments.h>
#include <qsf/physics/PhysicsWorldComponent.h>
#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/physics/collision/BulletCollisionComponent.h>

#include <btBulletDynamicsCommon.h>

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
		const uint32 CollisionMapTest::PLUGINABLE_ID = qsf::StringHash("em5::editor::CollisionMapTest");


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		CollisionMapTest::CollisionMapTest(qsf::editor::MapTestManager* mapTestManager) :
			MapTest(mapTestManager)
		{
			// Nothing to do in here
		}

		CollisionMapTest::~CollisionMapTest()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::MapTest methods                ]
		//[-------------------------------------------------------]
		void CollisionMapTest::run()
		{
			// TODO(tl): Static collisions are not a problem, the info could be used to detect collisions we could merge, but for the moment we should ignore it
			/*{ // Out-of-the-box static collisions inside the map
				qsf::PhysicsWorldComponent* physicsWorldComponent = getMap().getCoreEntity().getComponent<qsf::PhysicsWorldComponent>();
				if (nullptr != physicsWorldComponent)
				{
					// Perform a physics simulation step to gather the collisions
					physicsWorldComponent->updateJob(qsf::JobArguments());

					// Evaluate the collision
					for (auto collisionElement : physicsWorldComponent->getCurrentCollisions())
					{
						const btCollisionObject* orig = collisionElement.first;
						const btCollisionObject* obj = collisionElement.second;
						if (!orig || !obj || !obj->getUserPointer())
							continue;

						const qsf::BulletCollisionComponent* coll1 = static_cast<const qsf::BulletCollisionComponent*>(orig->getUserPointer());
						const qsf::BulletCollisionComponent* coll2 = static_cast<const qsf::BulletCollisionComponent*>(obj->getUserPointer());
						if (nullptr == coll1 || nullptr == coll2)
							continue;

						const qsf::MovableComponent* movable2 = coll2->getEntity().getComponent<qsf::MovableComponent>();
						if (movable2)
							continue;

						// Text: "Collision between static entities %1% and %2% detected, please review the collision editing of the entities"
						const qsf::Entity& entity1 = coll1->getEntity();
						const qsf::Entity& entity2 = coll2->getEntity();
						print(qsf::LogMessage::ERROR, (boost::format(tr("ID_EM5EDITOR_MAPTEST_COLLISION_STATIC").toUtf8()) % entity1.getId() % entity2.getId()).str(), &entity1);
					}
				}
			}*/

			{ // Invalid edited collision mask and collision filter group according to https://xwiki.promotion-software.de/xwiki/bin/view/EM5/Collision+Flags
				qsf::ComponentMapQuery componentMapQuery(getMap());

				// Bullet collision component
				for (qsf::BulletCollisionComponent* bulletCollisionComponent : componentMapQuery.getAllInstances<qsf::BulletCollisionComponent>())
				{
					// EMERGENCY 5 sets own Bullet collision component default values
					bool collisionFilterGroupBlamed = false;
					const short collisionFilterGroup = bulletCollisionComponent->getCollisionFilterGroup();
					if (std::numeric_limits<short>::max() != qsf::BulletCollisionComponent::mDefaultCollisionFilterGroup && collisionFilterGroup == std::numeric_limits<short>::max())
					{
						// Text: "The collision filter group of the bullet collision component of entity %1% has QSF default value %2% but should probably be %3%"
						print(qsf::LogMessage::ERROR, (boost::format(tr("ID_EM5EDITOR_MAPTEST_COLLISION_COLLISIONFILTERGROUP").toUtf8()) % bulletCollisionComponent->getEntityId() % std::numeric_limits<short>::max() % qsf::BulletCollisionComponent::mDefaultCollisionFilterGroup).str(), &bulletCollisionComponent->getEntity());
						collisionFilterGroupBlamed = true;
					}
					bool collisionMaskBlamed = false;
					const short collisionMask = bulletCollisionComponent->getCollisionMask();
					if (std::numeric_limits<short>::max() != qsf::BulletCollisionComponent::mDefaultCollisionMask && collisionMask == std::numeric_limits<short>::max())
					{
						// Text: "The collision mask of the bullet collision component of entity %1% has QSF default value %2% but should probably be %3%"
						print(qsf::LogMessage::ERROR, (boost::format(tr("ID_EM5EDITOR_MAPTEST_COLLISION_COLLISIONMASK").toUtf8()) % bulletCollisionComponent->getEntityId() % std::numeric_limits<short>::max() % qsf::BulletCollisionComponent::mDefaultCollisionMask).str(), &bulletCollisionComponent->getEntity());
						collisionMaskBlamed = true;
					}

					// Check common collision filter group values
					if (!collisionFilterGroupBlamed)
					{
						switch (collisionFilterGroup)
						{
							case 20:
							case 1556:
							case 1540:
							case 6144:
							case 70:
							case 0:		// TODO(co) Did I understand the Wiki information correctly?
							case 192:	// TODO(co) Did I understand the Wiki information correctly?
							case 448:	// TODO(co) Did I understand the Wiki information correctly?
								// Documented at https://xwiki.promotion-software.de/xwiki/bin/view/EM5/Collision+Flags - so it's ok
								break;

							default:
								// Hm, unusual value, might be an editing error

								// Text: "The collision filter group value of the bullet collision component of entity %1% is unusual, you might want to review it"
								print(qsf::LogMessage::ERROR, (boost::format(tr("ID_EM5EDITOR_MAPTEST_COLLISION_UNUSALCOLLISIONFILTERGROUP").toUtf8()) % bulletCollisionComponent->getEntityId()).str(), &bulletCollisionComponent->getEntity());
								break;
						}
					}

					// Check common collision mask values
					if (!collisionMaskBlamed)
					{
						switch (collisionMask)
						{
							case 37:
							case 5:
							case 3:
							case 16:
							case 2048:
							case 64:
							case 192:
							case 448:
							case 4100:
							case 512:
							case 1024:
							case 66:	// TODO(co) Did I understand the Wiki information correctly?
							case 2:		// TODO(co) Did I understand the Wiki information correctly?
								// Documented at https://xwiki.promotion-software.de/xwiki/bin/view/EM5/Collision+Flags - so it's ok
								break;

							default:
								// Hm, unusual value, might be an editing error

								// Text: "The collision mask value of the bullet collision component of entity %1% is unusual, you might want to review it"
								print(qsf::LogMessage::ERROR, (boost::format(tr("ID_EM5EDITOR_MAPTEST_COLLISION_UNUSALCOLLISIONMASK").toUtf8()) % bulletCollisionComponent->getEntityId()).str(), &bulletCollisionComponent->getEntity());
								break;
						}
					}
				}
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
