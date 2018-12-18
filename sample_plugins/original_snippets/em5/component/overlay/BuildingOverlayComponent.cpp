// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/overlay/BuildingOverlayComponent.h"
#include "em5/gui/hud/overlay/BuildingOverlayWindowManager.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/gui/IngameHud.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/game/Game.h"
#include "em5/game/units/OrderInfoManager.h"
#include "em5/EM5Helper.h"

#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/math/Math.h>
#include <qsf/job/JobArguments.h>

#include <Rocket/Core/ElementDocument.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 BuildingOverlayComponent::COMPONENT_ID = qsf::StringHash("em5::BuildingOverlayComponent");


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	void BuildingOverlayComponent::showProgress(qsf::Entity& buildingEntity, uint32 numberOfRequiredProviders, const qsf::Entity& providerEntity, float progress)
	{
		// Get or create the building overlay component instance
		BuildingOverlayComponent& buildingOverlayComponent = buildingEntity.getOrCreateComponentSafe<BuildingOverlayComponent>();

		// Find the provider entity inside the list
		buildingOverlayComponent.registerOrUpdateProvider(providerEntity.getId(), progress);

		// Update the number of required providers
		buildingOverlayComponent.setNumberOfRequiredProviders(numberOfRequiredProviders);

		// Sanity check
		QSF_ASSERT(numberOfRequiredProviders == 0 || buildingOverlayComponent.getRegisteredProviders().size() <= numberOfRequiredProviders, "em5::BuildingOverlayComponent::showProgress: Too many registered provider entities detected", QSF_REACT_NONE);
	}

	void BuildingOverlayComponent::hideOverlay(qsf::Entity& buildingEntity, const qsf::Entity& providerEntity)
	{
		// Get the building overlay component instance
		BuildingOverlayComponent* buildingOverlayComponent = buildingEntity.getComponent<BuildingOverlayComponent>();
		if (nullptr != buildingOverlayComponent)
		{
			// Do nothing if component was shut down already
			if (buildingOverlayComponent->isRunning())
			{
				// Find the provider entity inside the list
				buildingOverlayComponent->unregisterProvider(providerEntity.getId());
				if (buildingOverlayComponent->isEmpty())
				{
					// Remove component if it's not used any more
					buildingOverlayComponent->triggerDelayedDestruction();
				}
			}
		}
		else
		{
			// Disabled this assert because many gameplay actions hide the overlay multiple times
			// QSF_ASSERT(false, "em5::BuildingOverlayComponent::hideOverlay: Provider entity failed to unregister because the building has no building overlay component", QSF_REACT_NONE);
		}
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BuildingOverlayComponent::BuildingOverlayComponent(qsf::Prototype* prototype) :
		OverlayComponent(prototype, EM5_GUI.getIngameHud().getBuildingOverlayWindowManager()),
		mNumberOfRequiredProviders(0),
		mDirty(GUI_NOT_DIRTY)
	{
		// Nothing here
	}

	void BuildingOverlayComponent::registerOrUpdateProvider(uint64 providerEntityId, float progress)
	{
		Providers::iterator iterator = std::find_if(mRegisteredProviders.begin(), mRegisteredProviders.end(), [=](const BuildingOverlayProvider& provider) { return (provider.entityId == providerEntityId); });
		if (iterator != mRegisteredProviders.end())
		{
			// The provider is already registered, just update the progress
			iterator->progress = progress;
			setDirtyState(GUI_PROGRESS_DIRTY);
		}
		else
		{
			// Register new provider
			mRegisteredProviders.emplace_back(BuildingOverlayProvider(providerEntityId, progress));
			setDirtyState(GUI_LAYOUT_DIRTY);
			checkEmptyStateChanged();
		}
	}

	void BuildingOverlayComponent::unregisterProvider(uint64 providerEntityId)
	{
		Providers::iterator iterator = std::find_if(mRegisteredProviders.begin(), mRegisteredProviders.end(), [=](const BuildingOverlayProvider& provider) { return (provider.entityId == providerEntityId); });
		if (iterator != mRegisteredProviders.end())
		{
			// Remove the provider entity from the list
			mRegisteredProviders.erase(iterator);
			setDirtyState(GUI_LAYOUT_DIRTY);
			checkEmptyStateChanged();
		}
		else
		{
			QSF_ASSERT(false, "em5::BuildingOverlayComponent::hideOverlay: Provider entity failed to unregister because it's not registered inside the building overlay component", QSF_REACT_NONE);
		}
	}

	void BuildingOverlayComponent::setNumberOfRequiredProviders(uint32 numberOfRequiredProviders)
	{
		// Update the number of required providers
		if (mNumberOfRequiredProviders != numberOfRequiredProviders)
		{
			mNumberOfRequiredProviders = numberOfRequiredProviders;
			setDirtyState(GUI_LAYOUT_DIRTY);
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::OverlayComponent methods       ]
	//[-------------------------------------------------------]
	bool BuildingOverlayComponent::isContentEmpty() const
	{
		// The number of required providers is not interesting in here
		return mRegisteredProviders.empty();
	}

	void BuildingOverlayComponent::updateContent(Rocket::Core::ElementDocument& document)
	{
		switch (mDirty)
		{
			case GUI_NOT_DIRTY:
			{
				// Here was some blinking before, but we chose to remove it again
				break;
			}

			case GUI_LAYOUT_DIRTY:
			{
				// We need to rebuild the GUI from scratch
				const qsf::Map& map = getEntity().getMap();
				const size_t numberOfRegisteredProviders = mRegisteredProviders.size();
				const size_t numberOfProvidersShown = (mNumberOfRequiredProviders == 0) ? numberOfRegisteredProviders : mNumberOfRequiredProviders;

				// Create the number of requested providers
				Rocket::Core::Element* element = document.GetElementById("unitboxes");
				if (nullptr != element)
				{
					std::stringstream stringStream;
					int xPosition = -static_cast<int>(numberOfProvidersShown) * 16;
					for (size_t providerIndex = 0; providerIndex < numberOfProvidersShown; ++providerIndex, xPosition += 32)
					{
						const size_t currentId = providerIndex + 1;
						stringStream << "<div class=\"unit_container\" style=\"left:" << xPosition << "px;\">";
							stringStream << "<div class=\"unit_healthbar\" id=\"unit_healthbar" << currentId << "\">";
								stringStream << "<div class=\"life_back\" id=\"life_back" << currentId << "\">";
									stringStream << "<div class=\"lifebar\" id=\"lifebar" << currentId << "\" />";
								stringStream << "</div>";
							stringStream << "</div>";
							stringStream << "<div onclick=\"unit\" id=\"unit" << currentId << "\" class=\"box\" />";
						stringStream << "</div>";
					}
					element->SetInnerRML(stringStream.str().c_str());
				}

				// Setup providers
				for (size_t providerIndex = 0; providerIndex < numberOfProvidersShown; ++providerIndex)
				{
					// Get the libRocket element for the personal avatar health-bar
					Rocket::Core::Element* element = document.GetElementById(("unit_healthbar" + std::to_string(providerIndex + 1)).c_str());
					if (nullptr != element)
					{
						element->SetProperty("visibility", (providerIndex < numberOfRegisteredProviders) ? "visible" : "hidden");
					}

					// Get the libRocket element for the personal avatar
					element = document.GetElementById(("unit" + std::to_string(providerIndex + 1)).c_str());
					if (nullptr != element)
					{
						// Set the inner RML of the libRocket element
						std::string innerRml;
						if (providerIndex < numberOfRegisteredProviders)
						{
							// Required personal is already inside the building
							const BuildingOverlayProvider& provider = mRegisteredProviders[providerIndex];
							const qsf::Entity* providerEntity = map.getEntityById(provider.entityId);
							if (nullptr != providerEntity)
							{
								// Set personal avatar image
								innerRml = getPersonalInnerRmlString(*providerEntity);
							}
							else
							{
								// Error! This should never ever happen!
								QSF_ASSERT(false, "em5::BuildingOverlayComponent::updateContent: Invalid provider entity ID detected", QSF_REACT_NONE);
								innerRml = "<img width=\"30px\" height=\"26px\" src=\"../asset/mini_icons_avatars/not_available_slot.dds\" />";
							}
						}
						else
						{
							// Required personal still missing
							innerRml = "<img width=\"30px\" height=\"26px\" src=\"../asset/mini_icons_avatars/empty_slot.dds\" />";
						}
						element->SetInnerRML(innerRml.c_str());

						// Ensure the personal slot is visible
						element->SetProperty("visibility", "visible");
					}
				}

				// Done, we're no longer dirty
				mDirty = GUI_NOT_DIRTY;

				// Fall through by intent so the progress bar gets updated as well
			}

			case GUI_PROGRESS_DIRTY:
			{
				// Just perform a lightweight incremental update
				const size_t numberOfRegisteredProviders = mRegisteredProviders.size();
				for (size_t providerIndex = 0; providerIndex < mNumberOfRequiredProviders; ++providerIndex)
				{
					Rocket::Core::Element* lifeBarElement = document.GetElementById(("lifebar" + std::to_string(providerIndex + 1)).c_str());
					Rocket::Core::Element* lifeBarBackElement = document.GetElementById(("life_back" + std::to_string(providerIndex + 1)).c_str());
					if (nullptr != lifeBarElement && nullptr != lifeBarBackElement)
					{
						if (providerIndex < mNumberOfRequiredProviders)
						{
							const float progress = (providerIndex < numberOfRegisteredProviders) ? mRegisteredProviders[providerIndex].progress : 0.0f;
							QSF_ASSERT(progress >= 0.0f && progress <= 1.0f, "em5::BuildingOverlayComponent::updateContent(): Progress is outside the 0..1 interval", QSF_REACT_NONE);
							lifeBarElement->SetProperty("display", "block");
							lifeBarElement->SetProperty("width", std::to_string(30.0f * glm::clamp(progress, 0.0f, 1.0f)).c_str());
							lifeBarBackElement->SetProperty("display", "block");
						}
						else
						{
							lifeBarElement->RemoveProperty("display");
							lifeBarBackElement->RemoveProperty("display");
						}
					}
				}

				// Done, we're no longer dirty
				mDirty = GUI_NOT_DIRTY;
				break;
			}
		}
	}

	const glm::vec3& BuildingOverlayComponent::getContentWorldSpaceOffset() const
	{
		return qsf::Math::GLM_VEC3_ZERO;
	}

	const glm::vec2& BuildingOverlayComponent::getContentWindowSize() const
	{
		return qsf::Math::GLM_VEC2_ZERO;
	}

	const glm::vec2& BuildingOverlayComponent::getContentWindowOffset() const
	{
		return qsf::Math::GLM_VEC2_ZERO;
	}

	void BuildingOverlayComponent::updateAnimation(const qsf::JobArguments& jobArguments)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void BuildingOverlayComponent::setDirtyState(Dirty dirty)
	{
		mDirty = std::max(mDirty, dirty);
		if (GUI_LAYOUT_DIRTY == mDirty)
		{
			makeDirty();
		}
	}

	std::string BuildingOverlayComponent::getPersonalInnerRmlString(const qsf::Entity& providerEntity) const
	{
		static const std::string iconStyleAndPath = "<img width=\"30px\" height=\"26px\" src=\"../asset/mini_icons_avatars/";

		const CommandableComponent* commandableComponent = providerEntity.getComponent<CommandableComponent>();
		if (nullptr != commandableComponent)
		{

			const OrderInfo* orderInfo = EM5_GAME.getOrderInfoManager().findElement(commandableComponent->getUnitType());
			if (nullptr != orderInfo)
			{
				return iconStyleAndPath + orderInfo->getAvatarIcon() + ".dds\" />";
			}
		}

		// Fallback: Use civilian icon (used for ME310)
		return iconStyleAndPath + "civil_casual_male05.dds\" />";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
