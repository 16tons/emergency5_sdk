// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/effect/CommandTargetComponent.h"
#include "em5/component/effect/CommandTargetManager.h"
#include "em5/command/Command.h"
#include "em5/game/Game.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/gui/IngameHud.h"
#include "em5/EM5Helper.h"

#include <qsf/component/base/TransformComponent.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/renderer/helper/OgreCameraHelper.h>
#include <qsf/renderer/component/CameraComponent.h>
#include <qsf/settings/RendererSettingsGroup.h>
#include <qsf/math/Convert.h>
#include <qsf/map/Entity.h>
#include <qsf/gui/GuiContext.h>

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreViewport.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreManualObject2.h>

#include <Rocket/Core/Context.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CommandTargetComponent::COMPONENT_ID = qsf::StringHash("em5::CommandTargetComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CommandTargetComponent::CommandTargetComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mCommand(nullptr),
		mOgreManualObject(nullptr)
	{
		// Nothing to do in here
	}

	CommandTargetComponent::~CommandTargetComponent()
	{
		// Nothing to do in here
	}

	void CommandTargetComponent::setCommand(const Command* command)
	{
		const bool hasChanged = (mCommand != command);

		mCommand = command;

		if (isSimulating() && isRunning())
		{
			updateShownIcon(hasChanged);
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void CommandTargetComponent::onShutdown()
	{
		if (nullptr != mOgreManualObject)
		{
			destroyOgreManualObject();
		}
	}

	void CommandTargetComponent::onSetSimulating(bool simulating)
	{
		if (simulating && isRunning())
		{
			updateShownIcon(false);
		}
		else if (nullptr != mOgreManualObject)
		{
			destroyOgreManualObject();
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void CommandTargetComponent::updateShownIcon(bool forceRecreation)
	{
		// Destroy the old manual object in any case -- this method should only be called when there is an actual change
		if (nullptr == mCommand || forceRecreation)
		{
			if (nullptr != mOgreManualObject)
			{
				destroyOgreManualObject();
			}
		}

		if (nullptr != mCommand)
		{
			// Get icon position in world
			const glm::vec3 iconWorldPosition = getIconWorldPosition();

			// Calculate screen space position and size from world space
			glm::vec2 clipSpacePosition;
			glm::vec2 clipSpaceSize;
			const bool visible = projectToScreen(iconWorldPosition, glm::vec2(64.0f, 64.0f), clipSpacePosition, clipSpaceSize);

			if (visible)
			{
				// Need to create a new OGRE manual object?
				if (nullptr == mOgreManualObject)
				{
					mOgreManualObject = EM5_GUI.getIngameHud().getCommandTargetManager().rentOgreManualObject(mCommand->getIconSettings().mMouseIconName, clipSpaceSize);
				}

				// Set or update position
				mOgreManualObject->getParentSceneNode()->setPosition(clipSpacePosition.x, clipSpacePosition.y, 0.0f);
			}
			else if (nullptr != mOgreManualObject)
			{
				destroyOgreManualObject();
			}
		}
	}

	glm::vec3 CommandTargetComponent::getIconWorldPosition() const
	{
		qsf::Entity& targetEntity = getEntity();

		// Get center position from OGRE bounding box
		const qsf::MeshComponent* meshComponent = targetEntity.getComponent<qsf::MeshComponent>();
		if (nullptr != meshComponent)
		{
			const Ogre::v1::Entity* ogreEntity = meshComponent->getOgreEntity();
			if (nullptr != ogreEntity)
			{
				return qsf::Convert::getGlmVec3(ogreEntity->getWorldAabb().mCenter);
			}
		}

		// Fallback: Use entity position
		return targetEntity.getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();
	}

	bool CommandTargetComponent::projectToScreen(const glm::vec3& worldSpacePosition, const glm::vec2& screenSpaceSize, glm::vec2& outClipSpacePosition, glm::vec2& outClipSpaceSize) const
	{
		const qsf::CameraComponent* cameraComponent = EM5_GAME.getMainCamera();
		if (nullptr != cameraComponent)
		{
			const Ogre::Camera* ogreCamera = cameraComponent->getOgreCamera();
			if (nullptr != ogreCamera)
			{
				const Rocket::Core::Vector2i& dimensions = EM5_GUI.getGuiContext().getRocketCoreContext().GetDimensions();
				const int width = dimensions.x;
				const int height = dimensions.y;
				QSF_CHECK(width > 0, "OGRE viewport width is " << width, return false);
				QSF_CHECK(height > 0, "OGRE viewport height is " << height, return false);

				outClipSpaceSize = glm::vec2(screenSpaceSize.x * 2.0f / width, screenSpaceSize.y * 2.0f / height);

				// Distance check: Don't show icons too far away
				const Ogre::Real squaredDistance = ogreCamera->getDerivedPosition().squaredDistance(qsf::Convert::getOgreVector3(worldSpacePosition));
				if (squaredDistance < 100.0f * 100.0f)
				{
					// Project position into clip space; it's not visible if somewhere behind camera
					if (qsf::OgreCameraHelper::projectWorldToClipSpace(*ogreCamera, worldSpacePosition, outClipSpacePosition))
					{
						// Since we're using "qsf_unlit_mesh" we need to take care of renderer API depended clip space flipping in here
						if (qsf::RendererSettingsGroup::getInstanceSafe().getRendererSystem() == qsf::RendererSettingsGroup::OPENGL_SYSTEM)
						{
							outClipSpacePosition.y = -outClipSpacePosition.y;
						}

						const glm::vec2 clipSpaceMin = outClipSpacePosition + glm::vec2(0.0f, -outClipSpaceSize.y);
						const glm::vec2 clipSpaceMax = outClipSpacePosition + glm::vec2(outClipSpaceSize.x, 0.0f);

						// 2D clip test
						if (clipSpaceMax.x >= -1.0f && clipSpaceMin.x <= 1.0f &&
							clipSpaceMax.y >= -1.0f && clipSpaceMin.y <= 1.0f)
						{
							return true;
						}
					}
				}
			}
		}

		// Not visible
		return false;
	}

	void CommandTargetComponent::destroyOgreManualObject()
	{
		QSF_ASSERT(nullptr != mOgreManualObject, "There's no OGRE manual object instance", QSF_REACT_NONE);
		EM5_GUI.getIngameHud().getCommandTargetManager().returnRentedOgreManualObject(*mOgreManualObject);
		mOgreManualObject = nullptr;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
