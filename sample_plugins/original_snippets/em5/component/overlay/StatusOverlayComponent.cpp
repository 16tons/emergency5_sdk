// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/gui/IngameHud.h"
#include "em5/gui/hud/overlay/StatusOverlayWindow.h"
#include "em5/gui/hud/overlay/StatusOverlayWindowManager.h"
#include "em5/game/minimap/MinimapHelper.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/EM5Helper.h"

#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/base/MetadataComponent.h>
#include <qsf/job/JobArguments.h>
#include <qsf/map/Entity.h>
#include <qsf/renderer/mesh/MeshComponent.h>

#include <Rocket/Core/ElementDocument.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Detail implementation                                 ]
	//[-------------------------------------------------------]
	namespace detail
	{

		// Extend this to support more cases in which a minimap icon is directly associated with a status icon (which is the case often, but not always)
		static boost::container::flat_map<StatusOverlayComponent::StatusIconType, MiniMapIconType> gMiniMapIconByStatusIcon =
		{
			// Define helper macro
			#define STATUS_TO_MINIMAP(statusIcon, miniMapIcon) std::make_pair(StatusOverlayComponent::statusIcon, MiniMapIconType::miniMapIcon)

			// Mapping
			// TODO(co) STATUS_ICON_ARSONIST
			// TODO(co) STATUS_ICON_ATTACKED
			STATUS_TO_MINIMAP(STATUS_ICON_BOMB,							BOMB),
			STATUS_TO_MINIMAP(STATUS_ICON_CHEMICAL_CONTAMINATION,		CONTAMINATION_CHEMICAL),
			STATUS_TO_MINIMAP(STATUS_ICON_CONDITION_RESCUE_AXE,			RESCUE_AXE),
			STATUS_TO_MINIMAP(STATUS_ICON_CONDITION_RESCUE_DLK,			VICTIM_RESCUE_DLK),
			STATUS_TO_MINIMAP(STATUS_ICON_CONDITION_RESCUE_HELI,		VICTIM_RESCUE_HELI),
			STATUS_TO_MINIMAP(STATUS_ICON_DEAD,							DEAD),
			STATUS_TO_MINIMAP(STATUS_ICON_DEFECT,						DEFECT),
			STATUS_TO_MINIMAP(STATUS_ICON_DEFECT_VEHICLE,				DEFECT_VEHICLE),
			// TODO(co) STATUS_ICON_EVENT_VARIOUS
			STATUS_TO_MINIMAP(STATUS_ICON_FANATIC,						FANATIC),
			STATUS_TO_MINIMAP(STATUS_ICON_GAPER,						GAPER),
			STATUS_TO_MINIMAP(STATUS_ICON_GANGSTER,						GANGSTER),
			STATUS_TO_MINIMAP(STATUS_ICON_GANGSTER_IN_VEHICLE,			GANGSTER_IN_CAR),
			// TODO(co) STATUS_ICON_GUNFIRE
			// TODO(co) STATUS_ICON_HEAT_HAZARD
			// TODO(co) STATUS_ICON_INFECTED
			// TODO(co) STATUS_ICON_NPC
			STATUS_TO_MINIMAP(STATUS_ICON_POSSIBLY_INFECTED,			CONTAMINATION_CHEMICAL),
			// TODO(co) STATUS_ICON_POSSIBLY_INFECTED_BUILDING
			// TODO(co) STATUS_ICON_SEARCH
			STATUS_TO_MINIMAP(STATUS_ICON_SEARCH_VARIOUS,				SEARCH_VARIOUS),
			STATUS_TO_MINIMAP(STATUS_ICON_RADIOACTIVE_CONTAMINATION,	CONTAMINATION_NUCLEAR),
			STATUS_TO_MINIMAP(STATUS_ICON_BIOLOGICAL_CONTAMINATION,		CONTAMINATED_BIOLOGICAL),
			STATUS_TO_MINIMAP(STATUS_ICON_SICK,							VICTIM_INJURED),
			STATUS_TO_MINIMAP(STATUS_ICON_SICK_CONTAGIOUS,				VICTIM_INJURED_RED),
			// TODO(co) STATUS_ICON_SMOKE_POISONING
			STATUS_TO_MINIMAP(STATUS_ICON_STAKE,						STAKE),
			STATUS_TO_MINIMAP(STATUS_ICON_TREE,							BROKEN_TREE),
			STATUS_TO_MINIMAP(STATUS_ICON_WARBLERTWEET,					WARBLERTWEET),
			STATUS_TO_MINIMAP(STATUS_ICON_WELL,							WELL),
			STATUS_TO_MINIMAP(STATUS_ICON_WITCH,						WITCH),
			// TODO(co) STATUS_ICON_WRENCH

			// Undefine helper macro
			#undef STATUS_TO_MINIMAP
		};

		MiniMapIconType getMiniMapIconByStatusIcon(StatusOverlayComponent::StatusIconType statusIconType)
		{
			const auto iterator = gMiniMapIconByStatusIcon.find(statusIconType);
			return (iterator == gMiniMapIconByStatusIcon.end()) ? MiniMapIconType::UNDEFINED : iterator->second;
		}

		bool isNotParamedicInvisibleDummyMesh(qsf::MeshComponent& meshComponent)
		{
			// TODO(fw): This is a hack for the paramedics team, they have an invisible dummy mesh in their main entity
			//  -> Global asset ID 167866 - Local asset name "em5/mesh/default/dummy_mesh" - Local asset ID 2405884623
			return meshComponent.getMesh().getGlobalAssetId() != 167866;
		}
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 StatusOverlayComponent::COMPONENT_ID		= qsf::StringHash("em5::StatusOverlayComponent");

	const uint32 StatusOverlayComponent::STATUS_BAR_WIDTH		= 80;		// Value in pixel
	const uint32 StatusOverlayComponent::STATUS_BAR_ARROW_WIDTH	= 8;		// Value in pixel
	const glm::vec2 StatusOverlayComponent::WINDOW_SIZE(80.0f, 25.0f);		// Value in pixel
	const glm::vec2 StatusOverlayComponent::WINDOW_OFFSET(0.0f, 20.0f);		// Value in pixel

	const glm::vec3 StatusOverlayComponent::HEALTHBAR_WORLD_SPACE_OFFSET(0.0f, 1.0f, 0.0f);	// Offset in world space

	const float StatusOverlayComponent::STATUS_BAR_ARROW_BLINK_PERIOD		= 1.0f;		// Blink period in seconds
	const float StatusOverlayComponent::STATUS_BAR_FULL_TO_LOW_PERCENT		= 0.70f;	// Percentage where the healthbar changes
	const float StatusOverlayComponent::STATUS_BAR_FULL_TO_VERY_LOW_PERCENT	= 0.30f;	// Percentage where the healthbar changes

	const std::string StatusOverlayComponent::STATUS_FIRE_BAR_COLOR_FULL("#FA0F0FCF");	// Dark-red firebar
	const std::string StatusOverlayComponent::STATUS_FIRE_BAR_COLOR_LOW("#FA5D0FCF");	// Dark-orange firebar

	const std::string StatusOverlayComponent::STATUS_HEALTH_BAR_COLOR_FULL("#13CD45CF");	// Blue-green healthbar
	const std::string StatusOverlayComponent::STATUS_HEALTH_BAR_COLOR_LOW("#3FCD13CF");		// Yellow-green healthbar

	const std::string StatusOverlayComponent::STATUS_LIFE_BAR_COLOR_FULL("#D3FA0FCF");		// Yellow-green lifebar
	const std::string StatusOverlayComponent::STATUS_LIFE_BAR_COLOR_LOW("#FAAC0FCF");		// Light-orange lifebar

	const std::string StatusOverlayComponent::STATUS_DEATH_BAR_COLOR_FULL("#A05203CF");		// Brown deathbar
	const std::string StatusOverlayComponent::STATUS_DEATH_BAR_COLOR_LOW("#3C1F01CF");		// Brown-black deathbar

	//const std::string StatusOverlayComponent::STATUS_PROGRESS_BAR_COLOR_FULL("#0F0FFACF");	// Blue progressbar (not used any more)
	const std::string StatusOverlayComponent::STATUS_PROGRESS_BAR_COLOR_LOW("#0FACFACF");	// Light-blue progressbar

	const std::string StatusOverlayComponent::STATUS_HEALTH_VEHICLE_BAR_COLOR_FULL("#00FD00CF");
	const std::string StatusOverlayComponent::STATUS_HEALTH_VEHICLE_BAR_COLOR_LOW("#FAAC0FCF");
	const std::string StatusOverlayComponent::STATUS_HEALTH_VEHICLE_BAR_COLOR_VERY_LOW("#FF3311CF");

	StatusOverlayComponent::IconInfoMap StatusOverlayComponent::mIconInfoMap;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	qsf::Entity& StatusOverlayComponent::getEntityOrChildWithMesh(qsf::Entity& entity)
	{
		qsf::MeshComponent* meshComponent = entity.getComponent<qsf::MeshComponent>();
		if (nullptr != meshComponent &&
			meshComponent->getVisibilityFlags() != 0 &&
			qsf::isInitialized(meshComponent->getMesh()) &&
			detail::isNotParamedicInvisibleDummyMesh(*meshComponent))
		{
			return entity;
		}

		const qsf::LinkComponent* linkComponent = entity.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			// Special handling for paramedic team
			const bool isParamedic = UnitHelper(entity).isAmbulanceParamedics();

			// Also special handling for broken trees
			BuryComponent* buryComponent = entity.getComponent<BuryComponent>();

			for (qsf::LinkComponent* childLink : linkComponent->getChildLinks())
			{
				qsf::Entity& childEntity = childLink->getEntity();

				if (isParamedic)
				{
					// In case of an paramedic team we use the stretcher child entity as source for the overlay
					// TODO(sw) Might it be better to create a component which returns holds pointer to the entities of the paramedic team for easier access?
					const qsf::MetadataComponent* metadataComponent = childEntity.getOrCreateComponent<qsf::MetadataComponent>();
					if (metadataComponent->getTags() == "stretcher")
						return childEntity;
				}
				else
				{
					qsf::MeshComponent* meshComponent = childEntity.getComponent<qsf::MeshComponent>();
					if (nullptr != meshComponent && meshComponent->getVisibilityFlags() != 0 && detail::isNotParamedicInvisibleDummyMesh(*meshComponent))
					{
						bool rejectThisChild = false;
						if (nullptr != buryComponent)
						{
							for (uint64 buriedEntityId : buryComponent->BuriedEntitiesArray.getVariable())
							{
								if (buriedEntityId == childEntity.getId())
								{
									rejectThisChild = true;
									break;
								}
							}
						}

						if (!rejectThisChild)
						{
							return childEntity;
						}
					}
				}
			}
		}
		return entity;	// We found no children so just take our parent entity
	}

	void StatusOverlayComponent::showBarOverlay(qsf::Entity& overlayOwnerEntity, BarType barType, float progress)
	{
		StatusOverlayComponent::getEntityOrChildWithMesh(overlayOwnerEntity).getOrCreateComponentSafe<StatusOverlayComponent>().showBarOverlay(barType, progress);
	}

	void StatusOverlayComponent::hideBarOverlay(qsf::Entity& overlayOwnerEntity, BarType barType)
	{
		StatusOverlayComponent* statusOverlayComponent = StatusOverlayComponent::getEntityOrChildWithMesh(overlayOwnerEntity).getComponent<StatusOverlayComponent>();
		if (nullptr != statusOverlayComponent)
		{
			statusOverlayComponent->hideBarOverlay(barType);
			if (statusOverlayComponent->isContentEmpty())
			{
				// Remove component if it's not used any more
				statusOverlayComponent->triggerDelayedDestruction();
			}
		}
	}

	void StatusOverlayComponent::showIcon(qsf::Entity& overlayOwnerEntity, StatusIconType statusIconType, bool show)
	{
		qsf::Entity& entityWithMesh = StatusOverlayComponent::getEntityOrChildWithMesh(overlayOwnerEntity);

		if (show)
		{
			entityWithMesh.getOrCreateComponentSafe<StatusOverlayComponent>().showIcon(statusIconType);
		}
		else
		{
			StatusOverlayComponent::hideIcon(entityWithMesh, statusIconType);
		}
	}

	void StatusOverlayComponent::hideIcon(qsf::Entity& overlayOwnerEntity, StatusIconType statusIconType)
	{
		qsf::Entity& entityWithMesh = StatusOverlayComponent::getEntityOrChildWithMesh(overlayOwnerEntity);

		StatusOverlayComponent* statusOverlayComponent = entityWithMesh.getComponent<StatusOverlayComponent>();
		if (nullptr != statusOverlayComponent)
		{
			statusOverlayComponent->hideIcon(statusIconType);
			if (statusOverlayComponent->isContentEmpty())
			{
				// Remove component if it's not used any more
				statusOverlayComponent->triggerDelayedDestruction();
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	StatusOverlayComponent::StatusOverlayComponent(qsf::Prototype* prototype) :
		OverlayComponent(prototype, EM5_GUI.getIngameHud().getStatusOverlayWindowManager()),
		mBarBackgroundsVisible(false)
	{
		if (mIconInfoMap.empty())
		{
			initializeIconInfoMap();
		}
	}

	void StatusOverlayComponent::showBarOverlay(BarType barType, float progress)
	{
		// In case the last bar or icon gets replaced right now, stop the delayed destruction
		avoidDelayedDestruction();

		BarMap::iterator iterator = mBarMap.find(barType);
		if (iterator == mBarMap.end())
		{
			Bar bar;
			bar.mProgress = glm::clamp(progress, 0.0f, 1.0f);	// Don't show negative or too long bars
			bar.mLastProgress = 1.0f;	// Not initialized
			bar.mShownProgress = -1.0f;	// Not initialized
			bar.mNumberOfArrows = -1;	// Not initialized
			bar.mDirection = -1;
			mBarMap.emplace(barType, bar);
		}
		else
		{
			iterator->second.mLastProgress = iterator->second.mProgress;
			iterator->second.mDirection = (iterator->second.mLastProgress == progress) ? 0 : ((iterator->second.mLastProgress > progress) ? 1 : -1);
			iterator->second.mProgress = progress;
		}

		checkEmptyStateChanged();
	}

	void StatusOverlayComponent::hideBarOverlay(BarType barType)
	{
		BarMap::iterator iterator = mBarMap.find(barType);
		if (iterator != mBarMap.end())
		{
			mBarMap.erase(iterator);
		}

		checkEmptyStateChanged();
	}

	void StatusOverlayComponent::showIcon(StatusIconType statusIconType, bool show)
	{
		QSF_CHECK(mOverlayWindowManager.valid(), "Invalid status overlay window manager weak pointer", return);
		OverlayWindow* overlayWindow = mOverlayWindowManager->getOverlayWindowInstance(getEntityId());
		if (show)
		{
			// In case the last bar or icon gets replaced right now, stop the delayed destruction
			avoidDelayedDestruction();

			// Add status icon
			if (!mStatusIconFlags.isSet(statusIconType))
			{
				const IconInfo& iconInfo = mIconInfoMap[statusIconType];
				const Rocket::Core::String iconClass = iconInfo.mStyleSheetClass.c_str();
				if (!iconClass.Empty())
				{
					// Search the "statusicons" element in the .rml file and add information
					Rocket::Core::ElementDocument* rocketCoreElementDocument = overlayWindow->getRocketCoreElementDocument();
					Rocket::Core::Element* statusIconsElement = rocketCoreElementDocument->GetElementById("statusicons");
					if (nullptr != statusIconsElement)
					{
						// What size to use for the icon depends on entity type
						const char* sizeAsString = "36px";
						if (nullptr != getEntity().getComponentById("em5::BuildingComponent"))
						{
							// Buildings get larger icons (48px is original resolution)
							sizeAsString = "48px";
						}

						// Add new element
						Rocket::Core::Element* iconElement = rocketCoreElementDocument->CreateElement("div");	// Initial reference count is one
						iconElement->SetClass(iconClass, true);
						iconElement->SetClass("statusIcon", true);	// We need to activate both classes
						iconElement->SetId(iconClass);
						iconElement->SetAttribute("tooltip", iconInfo.mTooltipId.c_str());
						iconElement->SetProperty("width", sizeAsString);
						iconElement->SetProperty("height", sizeAsString);
						statusIconsElement->AppendChild(iconElement);	// "Rocket::Core::Element::AppendChild()" adds a reference count
						iconElement->RemoveReference();	// Free our reference

						centerStatusIcons(*statusIconsElement);
					}
				}
			}

			// Add mini-map icon as well, if we use this automatism for this specific status icon type
			const MiniMapIconType miniMapIconType = detail::getMiniMapIconByStatusIcon(statusIconType);
			if (miniMapIconType != MiniMapIconType::UNDEFINED)
			{
				MinimapHelper::registerIcon(getEntity(), miniMapIconType, false);
			}
		}
		else
		{
			const IconInfo& iconInfo = mIconInfoMap[statusIconType];
			const Rocket::Core::String iconClass = iconInfo.mStyleSheetClass.c_str();
			if (!iconClass.Empty())
			{
				// Remove status icon
				Rocket::Core::Element* statusIconsElement = overlayWindow->getRocketCoreElementDocument()->GetElementById("statusicons");
				if (nullptr != statusIconsElement)
				{
					Rocket::Core::Element* statusIconElement = statusIconsElement->GetElementById(iconClass);
					if (nullptr != statusIconElement)
					{
						statusIconsElement->RemoveChild(statusIconElement);

						centerStatusIcons(*statusIconsElement);
					}
				}
			}

			// Remove mini-map icon as well, if we use this automatism for this specific status icon type
			const MiniMapIconType miniMapIconType = detail::getMiniMapIconByStatusIcon(statusIconType);
			if (miniMapIconType != MiniMapIconType::UNDEFINED)
			{
				// TODO(fw): Do we need to add a check whether or not this icon is already set (and not another one maybe)?
				MinimapHelper::unregisterIcon(getEntity());
			}
		}

		mStatusIconFlags.set(statusIconType, show);

		checkEmptyStateChanged();
	}

	void StatusOverlayComponent::hideIcon(StatusIconType statusIconType)
	{
		StatusOverlayComponent::showIcon(statusIconType, false);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::OverlayComponent methods       ]
	//[-------------------------------------------------------]
	bool StatusOverlayComponent::isContentEmpty() const
	{
		return (mBarMap.empty() && !mStatusIconFlags.anySet());
	}

	void StatusOverlayComponent::updateContent(Rocket::Core::ElementDocument& document)
	{
		const bool barsVisible = !mBarMap.empty();

		// Update bar backgrounds
		if (mBarBackgroundsVisible != barsVisible)
		{
			mBarBackgroundsVisible = barsVisible;

			// Healthbar-back
			Rocket::Core::Element* healthBarBackElement = document.GetElementById("healthbar_back");
			Rocket::Core::Element* lifeBarBackElement = document.GetElementById("life_back");
			if (nullptr != healthBarBackElement && nullptr != lifeBarBackElement)
			{
				if (mBarBackgroundsVisible)
				{
					// Show the bar
					// The default progress (set via rcss) is display: none -> we must set here the property
					healthBarBackElement->SetProperty("display", "block");
				}
				else
				{
					// Hide bar
					// The default progress (set via rcss) is display: none -> we can remove here the property
					healthBarBackElement->RemoveProperty("display");
					lifeBarBackElement->RemoveProperty("display");
				}
			}
		}

		if (barsVisible)
		{
			Rocket::Core::Element* healthBarElement = document.GetElementById("healthbar");

			// Update the bars
			for (BarMap::value_type& iterator : mBarMap)
			{
				const BarType& barType = iterator.first;
				Bar& bar = iterator.second;
				bool enforceArrowUpdate = false;

				const bool hasDifference = (bar.mShownProgress != bar.mProgress) && (bar.mProgress <= 0.0f || bar.mProgress >= 1.0f || fabsf(bar.mShownProgress - bar.mProgress) >= 0.01f);
				if (hasDifference)
				{
					// Make sure the bar progress is inside the valid interval
					bar.mProgress = glm::clamp(bar.mProgress, 0.0f, 1.0f);

					bar.mShownProgress = bar.mProgress;

					const int statusBarWidth = static_cast<uint32>(glm::ceil(bar.mProgress * static_cast<float>(STATUS_BAR_WIDTH)));

					std::string healthbarColor;
					const bool full = (bar.mProgress >= STATUS_BAR_FULL_TO_LOW_PERCENT);

					Rocket::Core::Element* lifeBarElement = nullptr;
					Rocket::Core::Element* lifeBarBackElement = nullptr;

					switch (barType)
					{
						case BAR_TYPE_FIRE:
							healthbarColor = full ? STATUS_FIRE_BAR_COLOR_FULL : STATUS_FIRE_BAR_COLOR_LOW;
							break;

						case BAR_TYPE_HEALTH:
							healthbarColor = full ? STATUS_HEALTH_BAR_COLOR_FULL : STATUS_HEALTH_BAR_COLOR_LOW;
							break;

						case BAR_TYPE_SMALL_HEALTH:
							lifeBarElement = document.GetElementById("lifebar");
							lifeBarBackElement = document.GetElementById("life_back");
							healthbarColor = full ? STATUS_HEALTH_VEHICLE_BAR_COLOR_FULL :
											 (bar.mProgress >= STATUS_BAR_FULL_TO_VERY_LOW_PERCENT) ? STATUS_HEALTH_VEHICLE_BAR_COLOR_LOW : STATUS_HEALTH_VEHICLE_BAR_COLOR_VERY_LOW;
							break;

						case BAR_TYPE_LIFE:
							// Life bar should be full only when at 100%
							healthbarColor = (bar.mProgress > 0.99f) ? STATUS_LIFE_BAR_COLOR_FULL : STATUS_LIFE_BAR_COLOR_LOW;
							break;

						case BAR_TYPE_DEATH:
							healthbarColor = full ? STATUS_DEATH_BAR_COLOR_FULL : STATUS_DEATH_BAR_COLOR_LOW;
							break;

						case BAR_TYPE_PROGRESS:
							healthbarColor = STATUS_PROGRESS_BAR_COLOR_LOW;		// Full color not used, looks stupid if only shown above a certain value
							break;

						case _BAR_COUNT:
							// Nothing to do in this case
							break;
					}

					if (nullptr != lifeBarElement)
					{
						const int liveBarWidth = static_cast<uint32>(glm::ceil(bar.mProgress * static_cast<float>(70.0f)));
						const std::string lifeBarWidthAsString = std::to_string(liveBarWidth) + "px";

						lifeBarBackElement->SetProperty("display", "block");
						lifeBarElement->SetProperty("display", "block");
						lifeBarElement->SetProperty("background-color", healthbarColor.c_str());
						lifeBarElement->SetProperty("width", lifeBarWidthAsString.c_str());
					}
					else if (nullptr != healthBarElement)
					{
						const std::string statusBarWidthAsString = std::to_string(statusBarWidth) + "px";

						healthBarElement->SetProperty("display", "block");
						healthBarElement->SetProperty("width", statusBarWidthAsString.c_str());
						healthBarElement->SetProperty("background-color", healthbarColor.c_str());

						// Update number of visible arrows
						const int numberOfArrows = (bar.mProgress < 1.0f) ? (statusBarWidth / STATUS_BAR_ARROW_WIDTH) : 0;
						if (bar.mNumberOfArrows != numberOfArrows)
						{
							// Enforce immediate update of new arrows
							if (bar.mNumberOfArrows < numberOfArrows)
							{
								enforceArrowUpdate = true;
							}

							bar.mNumberOfArrows = numberOfArrows;

							for (int i = 0; i < healthBarElement->GetNumChildren(); ++i)
							{
								Rocket::Core::Element* imgElement = healthBarElement->GetChild(i);
								if (nullptr != imgElement)
								{
									if (i < numberOfArrows && bar.mDirection != 0)
									{
										imgElement->SetProperty("display", "inline");
									}
									else
									{
										imgElement->SetProperty("display", "none");
									}
								}
							}
						}
					}
				}

				if (mTimePassed - mLastUpdateTime >= qsf::Time::fromSeconds(0.1f) || enforceArrowUpdate)
				{
					mLastUpdateTime = mTimePassed;

					// Update arrow animation
					if (nullptr != healthBarElement && barType != BAR_TYPE_SMALL_HEALTH && bar.mDirection != 0 && bar.mNumberOfArrows > 0)
					{
						for (int i = 0; i < bar.mNumberOfArrows; ++i)
						{
							Rocket::Core::Element* imgElement = healthBarElement->GetChild(i);
							if (nullptr != imgElement)
							{
								// Calculate the number of the image we want to use
								const float percentOfPeriod = fmod(mTimePassed.getSeconds() / STATUS_BAR_ARROW_BLINK_PERIOD, 1.0f);
								const uint32 number = 5 + (static_cast<uint32>(glm::floor(10.0f * percentOfPeriod)) + i * (bar.mDirection > 0 ? 1 : -1)) % 10;
								const uint32 caretNumber = (number > 10) ? (20 - number) : number;

								static std::string caretImage;
								caretImage = (bar.mDirection > 0) ? "caret/left_" : "caret/right_";
								caretImage += std::to_string(caretNumber);
								caretImage += "0.dds";

								imgElement->SetAttribute("src", caretImage.c_str());
							}
						}
					}
				}
			}
		}
	}

	const glm::vec3& StatusOverlayComponent::getContentWorldSpaceOffset() const
	{
		return HEALTHBAR_WORLD_SPACE_OFFSET;
	}

	const glm::vec2& StatusOverlayComponent::getContentWindowSize() const
	{
		return WINDOW_SIZE;
	}

	const glm::vec2& StatusOverlayComponent::getContentWindowOffset() const
	{
		return WINDOW_OFFSET;
	}

	void StatusOverlayComponent::updateAnimation(const qsf::JobArguments& jobArguments)
	{
		mTimePassed += jobArguments.getTimePassed();
	}


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	void StatusOverlayComponent::initializeIconInfoMap()
	{
		mIconInfoMap.clear();

		// Create the definitions of the icons. The stylesheetclass (ssc) parameter defines the class, used in the .rml file (defined and used in the "StatusOverlayWindow" class), what is already defined
		// Here we map the enum, ssc and the tooltip together
		// To create new icon types, define the ssc classes first
		// The image paths are defined in "styles.rcss"
		#define INSERT_ICON_INFO(iconType, styleSheetClass, tooltipId) \
		{ \
			IconInfo info; \
			info.mStyleSheetClass = styleSheetClass; \
			info.mTooltipId = tooltipId; \
			mIconInfoMap.emplace(IconInfoMap::value_type(iconType, info)); \
		}

		INSERT_ICON_INFO(STATUS_ICON_ARSONIST,					"statusIcon_arsonist",					"ID_EM5_OVERLAY_COMPONENT_ICON_ARSONIST");
		INSERT_ICON_INFO(STATUS_ICON_ATTACKED,					"statusIcon_attacked",					"ID_EM5_OVERLAY_COMPONENT_ICON_ATTACKED");
		INSERT_ICON_INFO(STATUS_ICON_RADIOACTIVE_CONTAMINATION,	"statusIcon_radioactive",				"ID_EM5_OVERLAY_COMPONENT_ICON_RADIOACTIVE_CONTAMINATION");
		INSERT_ICON_INFO(STATUS_ICON_CHEMICAL_CONTAMINATION,	"statusIcon_chemical",					"ID_EM5_OVERLAY_COMPONENT_ICON_CHEMICAL_CONTAMINATION");
		INSERT_ICON_INFO(STATUS_ICON_BIOLOGICAL_CONTAMINATION,	"statusIcon_biohazard",					"ID_EM5_OVERLAY_COMPONENT_ICON_BIOLOGICAL_CONTAMINATION");
		INSERT_ICON_INFO(STATUS_ICON_BOMB,						"statusIcon_bomb",						"");
		INSERT_ICON_INFO(STATUS_ICON_CONDITION_RESCUE_AXE,		"statusIcon_condition_rescue_axe",		"ID_EM5_OVERLAY_COMPONENT_ICON_CONDITION_RESCUE_AXE");
		INSERT_ICON_INFO(STATUS_ICON_CONDITION_RESCUE_DLK,		"statusIcon_condition_rescue_dlk",		"ID_EM5_OVERLAY_COMPONENT_ICON_CONDITION_RESCUE_DLK");
		INSERT_ICON_INFO(STATUS_ICON_CONDITION_RESCUE_HELI,		"statusIcon_condition_rescue_heli",		"ID_EM5_OVERLAY_COMPONENT_ICON_CONDITION_RESCUE_HELI");
		INSERT_ICON_INFO(STATUS_ICON_DEAD,						"statusIcon_dead",						"");
		INSERT_ICON_INFO(STATUS_ICON_DEFECT,					"statusIcon_defect",					"");
		INSERT_ICON_INFO(STATUS_ICON_DEFECT_VEHICLE,			"statusIcon_defect_vehicle",			"ID_EM5_OVERLAY_COMPONENT_ICON_VEHICLE_DEFECT");
		INSERT_ICON_INFO(STATUS_ICON_EVENT_VARIOUS,				"statusIcon_event_various",				"");
		INSERT_ICON_INFO(STATUS_ICON_FANATIC,					"statusIcon_fanatic",					"");
		INSERT_ICON_INFO(STATUS_ICON_GAPER,						"statusIcon_gaper",						"");
		INSERT_ICON_INFO(STATUS_ICON_GANGSTER,					"statusIcon_gangster",					"ID_EM5_OVERLAY_COMPONENT_ICON_GANGSTER");
		INSERT_ICON_INFO(STATUS_ICON_GANGSTER_IN_VEHICLE,		"statusIcon_gangster_in_vehicle",		"ID_EM5_OVERLAY_COMPONENT_ICON_GANGSTER_IN_VEHICLE");
		INSERT_ICON_INFO(STATUS_ICON_GUNFIRE,					"statusIcon_gunfire",					"ID_EM5_OVERLAY_COMPONENT_ICON_GUNFIRE");
		INSERT_ICON_INFO(STATUS_ICON_HEAT_HAZARD,				"statusIcon_heathazard",				"ID_EM5_OVERLAY_COMPONENT_ICON_HAZARD");
		INSERT_ICON_INFO(STATUS_ICON_INFECTED,					"statusIcon_infected",					"");
		INSERT_ICON_INFO(STATUS_ICON_NPC,						"statusIcon_npc",						"");
		INSERT_ICON_INFO(STATUS_ICON_POSSIBLY_INFECTED,			"statusIcon_possibly_infected",			"ID_EM5_OVERLAY_COMPONENT_ICON_POSSIBLE_INFECTED");
		INSERT_ICON_INFO(STATUS_ICON_POSSIBLY_INFECTED_BUILDING,"statusIcon_possibly_infected_building","");
		INSERT_ICON_INFO(STATUS_ICON_SEARCH,					"statusIcon_search",					"");
		INSERT_ICON_INFO(STATUS_ICON_SEARCH_VARIOUS,			"statusIcon_search_various",			"");
		INSERT_ICON_INFO(STATUS_ICON_SICK,						"statusIcon_sick",						"ID_EM5_OVERLAY_COMPONENT_ICON_SICK");
		INSERT_ICON_INFO(STATUS_ICON_SICK_CONTAGIOUS,			"statusIcon_sick_contagious",			"ID_EM5_OVERLAY_COMPONENT_ICON_SICK_CONTAGIOUS");
		INSERT_ICON_INFO(STATUS_ICON_SMOKE_POISONING,			"statusIcon_smokehazard",				"ID_EM5_OVERLAY_COMPONENT_ICON_SMOKE_POISONING");
		INSERT_ICON_INFO(STATUS_ICON_STAKE,						"statusIcon_stake",						"ID_EM5_OVERLAY_COMPONENT_ICON_STAKE");
		INSERT_ICON_INFO(STATUS_ICON_TREE,						"statusIcon_tree",						"ID_EM5_OVERLAY_COMPONENT_ICON_TREE");
		INSERT_ICON_INFO(STATUS_ICON_WARBLERTWEET,				"statusIcon_warbler",					"");
		INSERT_ICON_INFO(STATUS_ICON_WELL,						"statusIcon_well",						"ID_EM5_OVERLAY_COMPONENT_ICON_WELL");
		INSERT_ICON_INFO(STATUS_ICON_WITCH,						"statusIcon_witch",						"ID_EM5_OVERLAY_COMPONENT_ICON_WITCH");
		INSERT_ICON_INFO(STATUS_ICON_WRENCH,					"statusIcon_wrench",					"");

		#undef INSERT_ICON_INFO
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void StatusOverlayComponent::centerStatusIcons(Rocket::Core::Element& element)
	{
		// TODO(sw) This is only an workaround, because with librocket centering 2 child elements vertically inside in a parent element doesn't work as in html/css
		// In CSS following would work
		// Style on the parent element: margin-left: auto; margin-right:auto; width: x px;
		// Style on the child elements: width:x px;
		const int newOffset = (80 - 36 * element.GetNumChildren()) / 2;
		if (newOffset > 0)
		{
			element.SetProperty("margin-left", (std::to_string(newOffset) + "px").c_str());
		}
		else
		{
			element.RemoveProperty("margin-left");
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
