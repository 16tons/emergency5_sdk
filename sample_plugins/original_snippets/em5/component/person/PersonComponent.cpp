// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/action/base/BeStunnedAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/em4Router/actor/EPerson.h"
#include "em5/base/PersonNameGenerator.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/game/Game.h"
#include "em5/game/animation/PersonAnimationController.h"
#include "em5/game/minimap/MinimapHelper.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/gangster/GangsterTypeManager.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/network/NetworkManager.h"
#include "em5/plugin/GameCounters.h"
#include "em5/plugin/Jobs.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/em4Router/RouterComponent.h>
#include <qsf_ai/navigation/em4Router/RouterScene.h>
#include <qsf_ai/navigation/em4Router/wrapper/EM3Singletons.h>

#include <qsf/component/base/MetadataComponent.h>	// Only for setting of a name
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/spatial/SpatialPartitionMemberComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/logic/gamelogic/GameLogicSystem.h>
#include <qsf/logic/gamelogic/GameLogicManager.h>
#include <qsf/map/Map.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/Random.h>	// Only for setting of a name
#include <qsf/math/Math.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/physics/collision/BulletBoxCollisionComponent.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/serialization/binary/StlTypeSerialization.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PersonComponent::COMPONENT_ID = qsf::StringHash("em5::PersonComponent");

	const std::string PERSONTYPE_MALE   = "male";
	const std::string PERSONTYPE_FEMALE = "female";
	const std::string PERSONTYPE_CHILD  = "child";
	const std::string PERSONTYPE_SENIOR = "senior";
	const std::string PERSONTYPE_ANIMAL = "animal";

	const std::string PERSONTYPE_UPPERCASE_MALE   = "MALE";
	const std::string PERSONTYPE_UPPERCASE_FEMALE = "FEMALE";
	const std::string PERSONTYPE_UPPERCASE_CHILD  = "CHILD";
	const std::string PERSONTYPE_UPPERCASE_SENIOR = "SENIOR";
	const std::string PERSONTYPE_UPPERCASE_ANIMAL = "ANIMAL";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PersonComponent::PersonComponent(qsf::Prototype* prototype) :
		qsf::game::PersonComponent(prototype),
		mAge(AGE_ADULT),
		mHomeBuildingId(qsf::getUninitialized<uint64>()),
		mBodyMass(75.0f),
		mPersonAnimationController(nullptr),
		mPreventUpdateAnimations(0),
		mBoundingBoxType(BoundingBoxType::BOX_UNKNOWN)
	{
		// Nothing here
	}

	PersonComponent::~PersonComponent()
	{
		// Nothing here
	}

	void PersonComponent::setStunned(uint64 initiatorId, const qsf::Time& timeStunned, bool stunnedFromPepper, bool stunnedFromDogAttack)
	{
		// Start the stun action
		qsf::ActionComponent& actionComponent = getEntity().getOrCreateComponentSafe<qsf::ActionComponent>();
		BeStunnedAction* beStunnedAction = actionComponent.getAction<BeStunnedAction>();
		if (nullptr == beStunnedAction)
		{
			beStunnedAction = &actionComponent.pushAction<BeStunnedAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT);
		}

		// Initialize or re-initialize the stun time
		beStunnedAction->init(initiatorId, timeStunned, (stunnedFromPepper ? BeStunnedAction::StunnedType_Pepperspray : (stunnedFromDogAttack ? BeStunnedAction::StunnedType_DogAttack : BeStunnedAction::StunnedType_Flashgrenade)));
	}

	void PersonComponent::setGangsterFlag(bool value)
	{
		if (mPersonFlags.isSet(FLAG_MARKED_AS_GANGSTER) != value)
		{
			mPersonFlags.set(FLAG_MARKED_AS_GANGSTER, value);

			if (isSimulating())
			{
				if (value)
				{
					GameCounters::incrementGameCounter(GameCounters::GANGSTERS);
				}
				else
				{
					GameCounters::decrementGameCounter(GameCounters::GANGSTERS);
				}

				// Update shown icon
				StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_GANGSTER, value);
			}
		}
	}

	void PersonComponent::preventUpdateAnimations(bool prevent)
	{
		if (prevent)
		{
			++mPreventUpdateAnimations;
		}
		else
		{
			--mPreventUpdateAnimations;
			if (mPreventUpdateAnimations == 0 && isRunning())
			{
				mPersonAnimationController->resetAnimationState();
			}
			QSF_ASSERT(mPreventUpdateAnimations >= 0, "preventUpdateAnimations was released more often than it was set.", QSF_REACT_NONE);
		}
	}

	void PersonComponent::clearAnimation()
	{
		if (isRunning())
		{
			mPersonAnimationController->resetAnimationState();
		}
	}

	GangsterBaseLogic* PersonComponent::changeGangsterType(const qsf::StringHash& newGangsterTypeId)
	{
		const GangsterType* gangsterType = EM5_GAME.getGangsterTypeManager().getGangsterTypeById(newGangsterTypeId);
		QSF_CHECK(nullptr != gangsterType, "Gangster type of ID " << newGangsterTypeId << " not found, cannot turn person into gangster", return nullptr);

		// Change to the new gangster type
		GangsterBaseLogic* newGangsterLogic = changeGangsterLogicInternal(gangsterType->getLogicClassId());
		if (nullptr == newGangsterLogic)
			return nullptr;

		// Apply settings from the gangster type definition, overwriting defaults in the logic's constructor
		newGangsterLogic->applyGangsterType(*gangsterType);

		return newGangsterLogic;
	}

	GangsterBaseLogic* PersonComponent::changeGangsterLogic(const qsf::StringHash& newLogicId)
	{
		return changeGangsterLogicInternal(newLogicId);
	}

	const std::string& PersonComponent::getPersonTypeAsString(bool uppercase) const
	{
		if (getAge() == PersonComponent::AGE_CHILD)
		{
			return uppercase ? PERSONTYPE_UPPERCASE_CHILD : PERSONTYPE_CHILD;
		}
		else if (getAge() == PersonComponent::AGE_SENIOR)
		{
			return uppercase ? PERSONTYPE_UPPERCASE_SENIOR : PERSONTYPE_SENIOR;
		}
		else if (getGender() == PersonComponent::GENDER_ANIMAL)
		{
			return uppercase ? PERSONTYPE_UPPERCASE_ANIMAL : PERSONTYPE_ANIMAL;
		}
		else if (getGender() == PersonComponent::GENDER_FEMALE)
		{
			// For adult and young person check gender, take female for females
			return uppercase ? PERSONTYPE_UPPERCASE_FEMALE : PERSONTYPE_FEMALE;
		}

		return uppercase ? PERSONTYPE_UPPERCASE_MALE : PERSONTYPE_MALE;
	}

	void PersonComponent::giveRandomName()
	{
		qsf::MetadataComponent* metadataComponent = getEntity().getComponent<qsf::MetadataComponent>();
		if (nullptr != metadataComponent)
		{
#if 0
			// The old, hard-coded way for getting names
			// Listing from "Polizei 2013", extended
			// TODO(co) We might want to update those with the EMERGENCY 5 team member names
			// DO NOT USE NON ASCII CHARS (e.g Umlauts ä ö ü) HERE! -> They must be proper encoded as utf8
			// UTF-8 codes for the umlauts be used inside string constants as octal escape sequence (otherwise we would create numbers which are bigger then a char can hold) and in "()" the pure utf8 codepoints
			// ä: \303\244 (C3 A4)
			// ö: \303\266 (C3 B6)
			// ü: \303\274 (C3 BC)
			// Ä: \303\204 (C3 84)
			// Ö: \303\226 (C3 96)
			// Ü: \303\234 (C3 9C)
			// ß: \303\237 (C3 9F)
			const std::string forenames[54] = { "Leon", "Lukas", "Ben", "Tobias", "Jonas", "Paul", "Till", "Maximilian", "Karl", "Felix", "Tim", "Gabriel", "Max", "Bastian", "Philipp", "Niklas", "Julian", "Moritz", "Jan", "David", "Fabian", "Alexander", "Simon", "Christian", "Tom", "Niko", "Ralph", "Erik", "Andreas", "Florian", "Michael", "Nils", "Henri", "Nick", "Holger", "Raphael", "Benjamin", "Carsten", "Jonathan", "Hannes", "Volker", "Jason", "Anton", "Emil", "Johannes", "Leo", "Mika", "Sebastian", "Dominik", "Daniel", "Manuel", "Christopher", "Arend", "Norman" };
			const std::string surnames[54] = { "M\303\274ller", "Schmidt", "Schneider", "Fischer", "Weber", "Meyer", "Wagner", "Becker", "Schulz", "Hoffmann", "Sch\303\244fer", "Koch", "Bauer", "Richter", "Klein", "Wolf", "Schr\303\266der", "Neumann", "Schwarz", "Zimmermann", "Braun", "Kr\303\274ger", "Hofmann", "Hartmann", "Lange", "Vogel", "Werner", "Keller", "Krause", "Berger", "Lehmann", "Lorenz", "Winkler", "Baumann", "K\303\266hler", "Herrmann", "K\303\266nig", "Walter", "Albrecht", "Huber", "Kaiser", "Fuchs", "Peters", "Lang", "Scholz", "M\303\266ller", "Wei\303\237", "Jung", "Hahn", "Schubert", "Ofenberg", "Wezel", "Wendel", "Kaiser" };
			metadataComponent->setName(forenames[qsf::Random::getRandomUint(0, 53)] + " " + surnames[qsf::Random::getRandomUint(0, 53)]);
#else
			// Use the translation system for the names
			metadataComponent->setName(PersonNameGenerator::getPersonName());
#endif
		}
	}

	void PersonComponent::giveRandomNameHistoric()
	{
		qsf::MetadataComponent* metadataComponent = getEntity().getComponent<qsf::MetadataComponent>();
		if (nullptr != metadataComponent)
		{
			metadataComponent->setName(PersonNameGenerator::getPersonNameHistoric());
		}
	}

	void PersonComponent::forceIdleAnimationUpdate()
	{
		if (isRunning())
		{
			mPersonAnimationController->forceIdleAnimationUpdate();
		}
	}

	void PersonComponent::forceAnimationUpdate()
	{
		if (isRunning())
		{
			mPersonAnimationController->updateAnimation();
		}
	}

	void PersonComponent::setPersonAnimationController(PersonAnimationController& personAnimationController)
	{
		delete mPersonAnimationController;
		mPersonAnimationController = &personAnimationController;
	}

	PersonComponent::BoundingBoxType PersonComponent::getBoundingBoxType() const
	{
		return mBoundingBoxType;
	}

	void PersonComponent::setBoundingBoxType(BoundingBoxType type)
	{
		if (mBoundingBoxType == type)
			return;

		// Set the new value
		mBoundingBoxType = type;

		qsf::MeshComponent* meshComponent = getEntity().getComponent<qsf::MeshComponent>();
		if (nullptr == meshComponent)
			return;

		if (!mCollisionEntity.valid())
		{
			// Create new linked entity, we have to delete it
			mCollisionEntity = getEntity().getMap().createEntity();
		}

		switch (type)
		{
			// TODO(mk) We have to set the bullet collision boxes too.
			case BOX_DROWNING:
				meshComponent->setOgreEntityBoundingBox(glm::vec3(-0.4f, -0.5f, -1.1f), glm::vec3(0.4f, 0.5f, 1.0f));	// TODO(fw): Bounding box could be optimized...
				break;

			case BOX_LYING:
			{
				meshComponent->setOgreEntityBoundingBox(glm::vec3(-0.4f, 0.0f, -0.1f), glm::vec3(0.4f, 0.5f, 2.0f));	// TODO(fw): Bounding box could be optimized...

				// The first alternative correctly sets the router environment bounding box, the second does not
#if 1
				qsf::ai::RouterComponent* routerComponent = qsf::ai::EM3::Scene->getOrCreateRouterComponent(getEntity());
				if (nullptr != routerComponent)
				{
					qsf::ai::EActor* actor = routerComponent->getActor();
					if (nullptr != actor && actor->GetType() == qsf::ai::EAT_PERSON)
					{
						EPerson* person = static_cast<EPerson*>(actor);
						person->setBoundingBox(glm::vec3(0.0f, 0.25f, 0.95f), glm::vec3(0.4f, 0.25f, 1.05f));

						if (nullptr == EntityHelper(getEntity()).getContainerEntity())
						{
							routerComponent->recreateCollisionBox();
						}
					}
				}
#else
				if (mCollisionEntity.valid())
				{
					// Place the collision entity at the optical center of the person
					qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();

					const glm::quat rotationInXZPlane = qsf::Math::getRotationByUpVector(qsf::CoordinateSystem::getUp(), transformComponent.getRotation());
					const glm::vec3 offsetFromLeg = rotationInXZPlane * qsf::CoordinateSystem::getIn() * -0.9f;	// Offset from leg to head
					mCollisionEntity->getOrCreateComponentSafe<qsf::TransformComponent>().setPosition(transformComponent.getPosition() + offsetFromLeg);

					qsf::LinkComponent& linkComponent = mCollisionEntity->getOrCreateComponentSafe<qsf::LinkComponent>();
					linkComponent.linkToParent(getEntity());
					linkComponent.setLocalRotation(glm::quat());	// Reset the local rotation

					// Give the collision to the helper entity
					qsf::BulletBoxCollisionComponent& bulletBoxCollisionComponent = mCollisionEntity->getOrCreateComponentSafe<qsf::BulletBoxCollisionComponent>();
					bulletBoxCollisionComponent.setHalfExtents(glm::vec3(0.3f, 0.4f, 0.8f));

					qsf::BulletBoxCollisionComponent* oldCollisionBoxComponent = getEntity().getComponent<qsf::BulletBoxCollisionComponent>();
					if (nullptr != oldCollisionBoxComponent)
					{
						// Copy all old values to the new collisionComponent
						bulletBoxCollisionComponent.setCollisionFilterGroup(oldCollisionBoxComponent->getCollisionFilterGroup());
						bulletBoxCollisionComponent.setCollisionMask(oldCollisionBoxComponent->getCollisionMask());
						bulletBoxCollisionComponent.setCollisionFlags(oldCollisionBoxComponent->getCollisionFlags());
						// Destroy the collision for the person
						getEntity().destroyComponent<qsf::BulletBoxCollisionComponent>();
					}
				}

				// Tell the AI about the new collision geometry
				qsf::ai::RouterComponent* routerComponent = getEntity().getComponent<qsf::ai::RouterComponent>();
				if (nullptr != routerComponent)
				{
					routerComponent->recreateCollisionBox();
				}
#endif
				break;
			}

			case BOX_KNEEING:
				meshComponent->setOgreEntityBoundingBox(glm::vec3(-0.4f, 0.0f, -0.4f), glm::vec3(0.4f, 1.0f, 0.4f));	// TODO(fw): Bounding box could be optimized...
				break;

			case BOX_STANDING:
				meshComponent->setOgreEntityBoundingBox(glm::vec3(-0.4f, 0.0f, -0.4f), glm::vec3(0.4f, 2.0f, 0.4f));		// TODO(fw): Bounding box could be optimized...
				break;

			default:
				QSF_ASSERT(false, "Setting other bounding box types is currently not supported", QSF_REACT_THROW);
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void PersonComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint8>(mAge);

		bool flagIsDrunk = false;
		bool flagIsGangster = false;

		if (serializer.isReading())
		{
			serializer.serialize(flagIsDrunk);
			serializer.serialize(flagIsGangster);
			mPersonFlags.set(FLAG_DRUNK, flagIsDrunk);
			mPersonFlags.set(FLAG_MARKED_AS_GANGSTER, flagIsGangster);
		}
		else
		{
			flagIsDrunk = mPersonFlags.isSet(FLAG_DRUNK);
			flagIsGangster = mPersonFlags.isSet(FLAG_MARKED_AS_GANGSTER);
			serializer.serialize(flagIsDrunk);
			serializer.serialize(flagIsGangster);
		}

		serializer.serialize(mBodyMass);
		serializer.serialize(mHomeBuildingId);
	}

	bool PersonComponent::onStartup()
	{
		// Register in spatial partition system, optimizing range searches for this kind of component
		qsf::SpatialPartitionMemberComponent::registerComponentInPartition(*this);

		// Create the person animation controller
		{
			// Send out a message so a mod can add its own person animation controller instance
			qsf::MessageParameters messageParameters;
			messageParameters.setParameter("personComponent", this);
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_CREATE_PERSON_ANIMATION_CONTROLLER, getEntityId()), messageParameters);

			// If none was set by the mod, use the default
			if (nullptr == mPersonAnimationController)
			{
				mPersonAnimationController = new PersonAnimationController(getEntity());
			}
		}

		// Done
		return true;
	}

	void PersonComponent::onShutdown()
	{
		// Unregister in spatial partition system
		qsf::SpatialPartitionMemberComponent::unregisterComponentInPartition(*this);

		// Destroy our collision object
		if (mCollisionEntity.valid())
		{
			getEntity().getMap().destroyObjectById(mCollisionEntity->getId());
		}

		// Destroy the person animation controller
		delete mPersonAnimationController;
		mPersonAnimationController = nullptr;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void PersonComponent::onSetSimulating(bool simulating)
	{
		if (simulating)
		{
			// Register job only when not on multiplayer client
			if (nullptr == EM5_NETWORK.getMultiplayerClient())
			{
				qsf::jobs::JobConfiguration jobConfiguration;
				jobConfiguration.mTimeBetweenCalls = qsf::Time::fromMilliseconds(1000);
				mAnimatingJobProxy.registerAt(Jobs::ANIMATION_PERSON, boost::bind(&PersonComponent::updateAnimation, this, _1), jobConfiguration);
			}
			registerMovableObjectRenderingListener();

			CommandableComponent* commandableComponent = getEntity().getComponent<CommandableComponent>();
			if (nullptr != commandableComponent)
			{
				const Player* localPlayer = PlayerManager::getLocalPlayer();
				const uint32 playerId = commandableComponent->getPlayerId();
				if (nullptr != localPlayer && (localPlayer->getPlayerIndex() == playerId || qsf::isUninitialized(playerId)))
				{
					commandableComponent->delayedShineThrough();
				}
			}

			updateShaderHeatValue();

			// Update icon
			if (mPersonFlags.isSet(FLAG_MARKED_AS_GANGSTER))
			{
				GameCounters::incrementGameCounter(GameCounters::GANGSTERS);
				StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_GANGSTER);
			}

			giveRandomName();
		}
		else
		{
			// We are going out of simulation business check if we need to decrement a game counter
			// -> Lookout! Do only perform the following work if we know for sure that we previously were in simulation mode (else we get e.g. inside the editor a crash in here).
			if (isSimulating() && mPersonFlags.isSet(FLAG_MARKED_AS_GANGSTER))
			{
				GameCounters::decrementGameCounter(GameCounters::GANGSTERS);
				MinimapHelper::unregisterIcon(getEntity());
			}

			// Unregister job
			mAnimatingJobProxy.unregister();
			unregisterMovableObjectRenderingListener();
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	qsf::Component* PersonComponent::getMovableObjectRenderingListenerComponent()
	{
		return getEntity().getComponent<qsf::RendererComponent>();
	}

	void PersonComponent::onObjectRenderingStartup()
	{
		// Call the base implementation
		JobProxyMovableObjectRenderingListener::onObjectRenderingStartup();

		// Set the heat for the heat renderer, using ogre custom parameter 20 (we need to do this every time the object turns visible)
		updateShaderHeatValue();

		// Put the animation job proxy into berserk-mode
		mAnimatingJobProxy.changeTimeBetweenCalls(qsf::Time::fromMilliseconds(0));
	}

	void PersonComponent::onObjectRenderingShutdown()
	{
		// Put the animation job proxy into sleepy-mode
		mAnimatingJobProxy.changeTimeBetweenCalls(qsf::Time::fromMilliseconds(1000));

		// Call the base implementation
		JobProxyMovableObjectRenderingListener::onObjectRenderingShutdown();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void PersonComponent::updateAnimation(const qsf::JobArguments& jobArguments)
	{
		// Don't automatically animate entities in vehicles/buildings
		if (isRunning() && mPreventUpdateAnimations <= 0 && nullptr == EntityHelper(getEntity()).getContainerEntity())
		{
			mPersonAnimationController->updateAnimation();
		}
	}

	void PersonComponent::updateShaderHeatValue()
	{
		// TODO(co) Update EMERGENCY 5 heat renderer
		/*
		const qsf::MeshComponent* meshComponent = getEntity().getComponent<qsf::MeshComponent>();
		if (nullptr != meshComponent)
		{
			const float heat = getGangsterFlag() ? 1.0f : 0.75f;
			meshComponent->setOgreCustomParameter(20, glm::vec4(heat, 0.0f, 0.0f, 0.0f));
		}
		*/
	}

	GangsterBaseLogic* PersonComponent::changeGangsterLogicInternal(uint32 newGangsterLogicId)
	{
		// Query the game logic component
		qsf::GameLogicComponent* gameLogicComponent = getEntity().getOrCreateComponent<qsf::GameLogicComponent>();

		GangsterBaseLogic::WeaponFlagSet lastGangsterWeapons;
		uint32 lastLogic = qsf::getUninitialized<uint32>();

		GangsterBaseLogic* oldGangsterLogic = gameLogicComponent->getGameLogic<GangsterBaseLogic>();
		if (nullptr != oldGangsterLogic)
		{
			// There already exists a logic that inherits from GangsterBaseLogic, we remove that one

			// Store its ID
			lastLogic = oldGangsterLogic->getTypeId();

			// Remember the gangster's weapons
			lastGangsterWeapons = oldGangsterLogic->getEquippedWeaponTypes();

			// Hide currently equipped weapon
			oldGangsterLogic->hideCurrentWeapon();

			// Create the game logic system if there is none
			const qsf::GameLogicSystem& gameLogicSystem = getEntity().getMap().getCoreEntity().getOrCreateComponentSafe<qsf::GameLogicSystem>();

			// Remove that (gangster) logic
			gameLogicSystem.getManager().removeLogic(*oldGangsterLogic);
		}

		// Create new (gangster) logic
		GangsterBaseLogic* newGangsterLogic = dynamic_cast<GangsterBaseLogic*>(gameLogicComponent->createGameLogicByTypeId(newGangsterLogicId));
		QSF_CHECK(newGangsterLogic != nullptr, "GangsterLogic could not be inserted!", return nullptr);

		if (qsf::isInitialized(lastLogic))
		{
			// "Copy" weapons
			newGangsterLogic->setEquippedWeaponTypes(lastGangsterWeapons);

			// Keep track of the last gangster logic, e.g. needed by the "Killer Taskforce" types
			newGangsterLogic->setLastGangsterLogic(lastLogic);
		}

		return newGangsterLogic;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
