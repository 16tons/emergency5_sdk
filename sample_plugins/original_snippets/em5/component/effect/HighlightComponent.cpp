// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/effect/HighlightComponent.h"
#include "em5/component/effect/GroundSpotComponent.h"

#include <qsf/renderer/light/LightComponent.h>
#include <qsf/job/JobArguments.h>
#include <qsf/plugin/QsfJobs.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 HighlightComponent::COMPONENT_ID = qsf::StringHash("em5::HighlightComponent");


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	HighlightComponent::HighlightComponent(qsf::Prototype* prototype) :
		qsf::compositing::HighlightComponent(prototype),
		mCurrentHighlightType(FLAG_NONE),
		mGroundHeight(3.0f),
		mGroundSpotLightRadius(0.0f),
		mSpotLightNearClipDistance(0.0f)
	{
		// Nothing to do here
	}

	HighlightComponent::~HighlightComponent()
	{
		// Nothing to do in here
	}

	void HighlightComponent::setHighlightFlags(HighlightComponent::HighlightFlags flag, bool value)
	{
		const bool oldValue = getHighlightFlags(flag);
		mHighlightFlags.set(flag, value);

		if (flag == FLAG_COMMANDTARGET && getGroundSpotLightRadius() > 0.0f)
		{
			setGroundSpotLightRadius(0.0f);
		}

		if (oldValue != value)
		{
			updateHighlight();
		}
	}

	void HighlightComponent::setGroundSpotLightRadius(float radius)
	{
		if (isRunning())
		{
			if (radius > 0.0f)
			{
				mGroundSpotLightRadius = radius;
				createOrUpdateGroundSpotComponent();
			}
			else if (mGroundSpotLightRadius > 0.0f)	// Did we (probably) set it?
			{
				// TODO(sw) Bad idea, because this method is also called when Prototype::destroyAllComponents is called (via HighlightComponent::resetHighlightValues()). Causes that the assert triggers: QSF_ASSERT(mComponentList.size() == componentListSize, "Component shutdown changed size of prototype's component list, this must not happen", QSF_REACT_NONE);
				getEntity().destroyComponent<GroundSpotComponent>();
			}
		}

		mGroundSpotLightRadius = radius;
	}

	void HighlightComponent::setGroundHeight(float groundHeight)
	{
		if (isRunning())
		{
			GroundSpotComponent* groundSpotComponent = getEntity().getComponent<GroundSpotComponent>();
			if (nullptr != groundSpotComponent)
			{
				groundSpotComponent->setGroundSpotLightHeight(groundHeight);
			}
		}

		mGroundHeight = groundHeight;
	}

	void HighlightComponent::setSpotLightNearClipDistance(float distance)
	{
		if (isRunning())
		{
			const GroundSpotComponent* groundSpotComponent = getEntity().getComponent<GroundSpotComponent>();
			if (nullptr != groundSpotComponent)
			{
				const qsf::Entity* lightEntity = groundSpotComponent->getGroundSpotLightEntity();
				if (nullptr != lightEntity)
				{
					qsf::LightComponent* lightComponent = lightEntity->getComponent<qsf::LightComponent>();
					if (nullptr != lightComponent)
					{
						lightComponent->setSpotlightNearClipDistance(distance);
					}
				}
			}
		}

		mSpotLightNearClipDistance = distance;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	bool HighlightComponent::onStartup()
	{
		createOrUpdateGroundSpotComponent();
		return true;
	}

	void HighlightComponent::onShutdown()
	{
		resetHighlightValues();

		// Call the base implementation
		qsf::compositing::HighlightComponent::onShutdown();
	}

	void HighlightComponent::onSetActive(bool active)
	{
		// Tell the childs about the active change. Use only real ground spot components to avoid strange behavior with derived classes.
		GroundSpotComponent* groundSpotComponent = getEntity().getComponent<GroundSpotComponent>();
		if (nullptr != groundSpotComponent && groundSpotComponent->getId() == GroundSpotComponent::COMPONENT_ID)
		{
			groundSpotComponent->setActive(active);
		}
	}


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	void HighlightComponent::createOrUpdateGroundSpotComponent()
	{
		if (mGroundSpotLightRadius > 0.0f)
		{
			// Disable the ground spot component if it was already active for the extra reboot effect (read: for the changes to get 'active')
			GroundSpotComponent& groundSpotComponent = getEntity().getOrCreateComponentSafe<GroundSpotComponent>(false);

			// Should never be running at this point in time, but be safe
			if (groundSpotComponent.isRunning())
			{
				groundSpotComponent.setActive(false);
				groundSpotComponent.setRadius(mGroundSpotLightRadius);
				groundSpotComponent.setGroundSpotLightHeight(mGroundHeight);
				groundSpotComponent.setActive(true);
			}
			else
			{
				groundSpotComponent.setRadius(mGroundSpotLightRadius);
				groundSpotComponent.setGroundSpotLightHeight(mGroundHeight);
				groundSpotComponent.setSpotLightTexture("em5/texture/default/groundmarker_radius_flashbang");

				// Now that the ground spot component is configured, start it
				groundSpotComponent.startup(getEntity().getMap().isRunning());
			}

			const qsf::Entity* lightEntity = groundSpotComponent.getGroundSpotLightEntity();
			if (nullptr != lightEntity)
			{
				qsf::LightComponent* lightComponent = lightEntity->getComponent<qsf::LightComponent>();
				if (nullptr != lightComponent)
				{
					lightComponent->setSpotlightNearClipDistance(mSpotLightNearClipDistance);
				}
			}
		}
	}

	void HighlightComponent::updateHighlight()
	{
		HighlightFlags newHighlightType = FLAG_NONE;

		if (getHighlightFlags(FLAG_COMMANDTARGET))
		{
			if (mCurrentHighlightType != FLAG_COMMANDTARGET)
			{
				newHighlightType = FLAG_COMMANDTARGET;

				// Set pulsing rim-light
				setRimLightColor(qsf::Color3::WHITE * 0.15f);	// *0.15 = super-atomic-glow, please
				setRimLightEnabled(true);
				setManualPulseEnabled(false);
				setPulseEnabled(true);
			}
		}
		else if (getHighlightFlags(FLAG_SELECTED))
		{
			if (mCurrentHighlightType != FLAG_SELECTED)
			{
				newHighlightType = FLAG_SELECTED;

				// Only setup color, when not changing from command target to this state
				if (mCurrentHighlightType != FLAG_COMMANDTARGET)
				{
					setRimLightColor(qsf::Color3::GREEN);
					setRimLightEnabled(true);
					setManualPulseEnabled(false);
					setPulseEnabled(false);

					// The selected color should only be shown for a short time
					// Setup a job proxy for the fade out effect when entity got selected
					mJobProxy.registerAt(qsf::QsfJobs::REALTIME_GENERAL, boost::bind(&HighlightComponent::updateJob, this, _1));
				}
				else
				{
					// We are coming from command target state -> reset all values, because the selection highlight is only a one-shot effect
					resetHighlightValues();
				}
			}
		}
		else if (getHighlightFlags(FLAG_SELECTABLE))
		{
			if (mCurrentHighlightType != FLAG_SELECTABLE)
			{
				newHighlightType = FLAG_SELECTABLE;

				// Set pulsing rim-light
				setRimLightColor(qsf::Color3::CYAN);
				setRimLightEnabled(true);
				setManualPulseEnabled(false);
				setPulseEnabled(true);
			}
		}
		else if (getHighlightFlags(FLAG_CUSTOM))
		{
			newHighlightType = FLAG_CUSTOM;

			// Values get changes from extern
		}

		if (newHighlightType != mCurrentHighlightType)
		{
			mCurrentHighlightType = newHighlightType;
			if (mCurrentHighlightType == FLAG_NONE)
			{
				resetHighlightValues();
			}
			else
			{
				if (mCurrentHighlightType != FLAG_SELECTED)
				{
					// Unregister update job for the fade out effect when entity got selected
					mJobProxy.unregister();
				}

				// Start a fresh pulse animation so the user always sees at once the full color
				if (getPulseEnabled())
				{
					setPulseValue(0.0f);
				}
			}

			// Only update material when not custom management is set
			if (mCurrentHighlightType != FLAG_CUSTOM)
			{
				updateMaterial();
			}
		}
	}

	void HighlightComponent::resetHighlightValues()
	{
		setGroundSpotLightRadius(0.0f);
		setPulseEnabled(false);
		setRimLightEnabled(false);
		setManualPulseEnabled(false);
		setActive(false);
	}

	void HighlightComponent::updateJob(const qsf::JobArguments& jobArguments)
	{
		// Only do the fade out effect, when in selected state
		if (mCurrentHighlightType == FLAG_SELECTED)
		{
			// Update rim light color
			qsf::Color3 rimLightColor = getRimLightColor();
			rimLightColor -= 0.5f * jobArguments.getSecondsPassed();
			if (rimLightColor.r <= 0.0f && rimLightColor.g <= 0.0f && rimLightColor.b <= 0.0f)
			{
				// Disable rim lighting completely, won't have any visible effect now anyway (and this way we avoid issues with e.g. the shine-through)
				setRimLightEnabled(false);
			}
			rimLightColor.saturate();

			if (getRimLightColor() != rimLightColor)
			{
				setRimLightColor(rimLightColor);
				updateMaterial();
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
