// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/component/IndicatorComponent.h"

#include <qsf/component/base/TransformComponent.h>
#include <qsf/debug/request/PolygonDebugDrawRequest.h>
#include <qsf/job/JobArguments.h>
#include <qsf/map/Entity.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/math/Random.h>
#include <qsf/plugin/QsfJobs.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 IndicatorComponent::COMPONENT_ID = qsf::StringHash("user::IndicatorComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	IndicatorComponent::IndicatorComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mColor(Color::WHITE),
		mIsAnimated(false)
	{
		// Nothing to do in here
	}

	IndicatorComponent::~IndicatorComponent()
	{
		// Nothing to do in here
	}

	void IndicatorComponent::setColor(Color color)
	{
		mColor = color;
	}

	void IndicatorComponent::setRandomColor()
	{
		switch (qsf::Random::getRandomUint(0, 4))
		{
			default:
			case 0:
				setColor(Color::WHITE);
				break;
			case 1:
				setColor(Color::RED);
				break;
			case 2:
				setColor(Color::YELLOW);
				break;
			case 3:
				setColor(Color::GREEN);
				break;
			case 4:
				setColor(Color::BLUE);
				break;
		}
	}

	void IndicatorComponent::setIsAnimated(bool animated)
	{
		mIsAnimated = animated;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::Component methods              ]
	//[-------------------------------------------------------]
	bool IndicatorComponent::onStartup()
	{
		// Activate regular update call by registering via the job proxy
		//  -> We take the "ANIMATION_GENERAL" job here, which gets called once a frame and works in the editor as well -- in contrast to "SIMULATION" jobs
		mUpdateJobProxy.registerAt(qsf::QsfJobs::ANIMATION_GENERAL, boost::bind(&IndicatorComponent::updateJob, this, _1));

		// Done
		return true;
	}

	void IndicatorComponent::onShutdown()
	{
		// Deactivate update job
		mUpdateJobProxy.unregister();

		// Deactivate visualization
		mDebugDrawProxy.unregister();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void IndicatorComponent::updateJob(const qsf::JobArguments& jobArguments)
	{
		// Time progress since last update -- not used here, just an example
		const qsf::Time timeSinceLastUpdate = jobArguments.getTimePassed();

		// Animation time update
		if (mIsAnimated)
		{
			mAnimationTime += timeSinceLastUpdate;
		}

		// Update visual representation
		//  -> This is actually quite inperformant this way, it's better to update only when necessary (e.g. color change or entity moved)
		updateVisualization();
	}

	void IndicatorComponent::updateVisualization(bool onlyPosition)
	{
		if (!onlyPosition)
		{
			// Register debug draw proxy
			//  -> This will remove all visualizations (= "debug draw requests") associated with the proxy, so we can fill it with new requests
			mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);

			// Build 6 vertices using local coordinates relative to the entity
			//  -> Note that the coordinate system uses Y-direction as up vector, X as right vector and negative Z as front vector
			static const glm::vec3 front ( 0.0f,  0.0f, -0.5f);
			static const glm::vec3 back  ( 0.0f,  0.0f, +0.5f);
			static const glm::vec3 left  (-0.5f,  0.0f,  0.0f);
			static const glm::vec3 right (+0.5f,  0.0f,  0.0f);
			static const glm::vec3 bottom( 0.0f, -1.0f,  0.0f);
			static const glm::vec3 top   ( 0.0f, +1.0f,  0.0f);

			// Choose a color
			qsf::Color4 color;
			switch (mColor)
			{
				case WHITE:
					color = qsf::Color4::WHITE;
					break;
				case RED:
					color = qsf::Color4::RED;
					break;
				case YELLOW:
					color = qsf::Color4::YELLOW;
					break;
				case GREEN:
					color = qsf::Color4::GREEN;
					break;
				case BLUE:
					color = qsf::Color4::BLUE;
					break;
			}

			// Create a new visualization consisting of multiple polygons
			mDebugDrawProxy.addRequest(qsf::PolygonDebugDrawRequest( { left,  front, top },    true, color, 0.5f));
			mDebugDrawProxy.addRequest(qsf::PolygonDebugDrawRequest( { back,  left,  top },    true, color, 0.5f));
			mDebugDrawProxy.addRequest(qsf::PolygonDebugDrawRequest( { right, back,  top },    true, color, 0.5f));
			mDebugDrawProxy.addRequest(qsf::PolygonDebugDrawRequest( { front, right, top },    true, color, 0.5f));
			mDebugDrawProxy.addRequest(qsf::PolygonDebugDrawRequest( { front, left,  bottom }, true, color, 0.5f));
			mDebugDrawProxy.addRequest(qsf::PolygonDebugDrawRequest( { left,  back,  bottom }, true, color, 0.5f));
			mDebugDrawProxy.addRequest(qsf::PolygonDebugDrawRequest( { back,  right, bottom }, true, color, 0.5f));
			mDebugDrawProxy.addRequest(qsf::PolygonDebugDrawRequest( { right, front, bottom }, true, color, 0.5f));
		}

		// Move to the right position, namely to the entity containing this component
		{
			// Access the entity's transform component (create one if necessary)
			//  -> The transform component holds position, rotation and scale of an entity
			qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();

			// Access the animation transform, so we get a smoothly interpolated position in-game
			//  -> In contrast, "transformComponent.getPosition()" would give us the non-interpolated position used by game simulation
			const glm::vec3 entityPosition = transformComponent.getAnimationTransform().getPosition();

			// Set transformation by taking the entity position with an offset, animated rotation and default scale
			const float offsetSinInput = mAnimationTime.getSeconds() * glm::pi<float>() * 0.5f;
			const glm::vec3 offset(0.0f, 3.0f + std::sin(offsetSinInput) * 0.25f, 0.0f);	// Hovering 3.0 +- 0.25 meters above entity pivot
			const glm::vec3 position = entityPosition + offset;

			// Rotate with 90° per second
			const float angleRadians = mAnimationTime.getSeconds() * glm::pi<float>() * 0.5f;
			const glm::quat rotation = qsf::EulerAngles::eulerToQuaternion(angleRadians, 0.0f, 0.0f);

			mDebugDrawProxy.setTransform(qsf::Transform(position, rotation));
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user
