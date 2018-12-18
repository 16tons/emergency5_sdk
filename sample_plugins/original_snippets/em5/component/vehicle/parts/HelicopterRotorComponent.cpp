// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vehicle/parts/HelicopterRotorComponent.h"

#include <qsf/component/link/LinkComponent.h>
#include <qsf/renderer/component/RendererComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/math/Math.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/time/Time.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 HelicopterRotorComponent::COMPONENT_ID = qsf::StringHash("em5::HelicopterRotorComponent");

	const float HelicopterRotorComponent::SPEED_TRESHHOLD = glm::pi<float>() * 4.0f;	// 2 revolution per second 150 U/min
	const float HelicopterRotorComponent::SPEED_LIMIT = glm::pi<float>() *10.0f;		// 5 revolution per second 300 U/min
	const float HelicopterRotorComponent::SPEED_MAIN_TO_TAIL_FACTOR = 5.0f;				// Tail rotor 5 times faster
	const float HelicopterRotorComponent::SPEED_ACCELERATION_FACTOR = 1.8f;
	const float HelicopterRotorComponent::SPEED_DECELERATION_FACTOR = 2.5f;				// Slightly faster deceleration


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	HelicopterRotorComponent::HelicopterRotorComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mRotorPosition(ROTOR_MAIN),
		mRotorType(ROTOR_REAL),
		mRotorRealSpeed(0.0f)
	{
		// Nothing to do in here
	}

	HelicopterRotorComponent::~HelicopterRotorComponent()
	{
		// Nothing to do in here
	}

	void HelicopterRotorComponent::updateRotor(const qsf::Time& timePassed)
	{
		float speed = mRotorRealSpeed;
		bool visible = false;
		glm::vec3 axis = qsf::CoordinateSystem::getUp();

		if (mRotorPosition == ROTOR_TAIL)
		{
			speed *= SPEED_MAIN_TO_TAIL_FACTOR;
			axis = qsf::CoordinateSystem::getRight();
		}
		if (mRotorType == ROTOR_REAL)
		{
			visible = (speed < SPEED_TRESHHOLD);
		}
		else if (mRotorType == ROTOR_SLOW_BLURRED)
		{
			visible = (speed >= SPEED_TRESHHOLD);
			speed *= -1;
		}
		else if (mRotorType == ROTOR_FAST_BLURRED)
		{
			visible = (speed >= SPEED_TRESHHOLD);
		}

		qsf::LinkComponent* linkComponent = getEntity().getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			const glm::quat newRotation = linkComponent->getLocalRotation() * qsf::Math::getRotationAroundAxis(axis, fmod(speed * timePassed.getSeconds(), 2.0f * glm::pi<float>()));
			linkComponent->setLocalRotationForAnimation(newRotation);
		}
		qsf::RendererComponent* renderComponent = getEntity().getComponent<qsf::RendererComponent>();
		if (nullptr != renderComponent)
		{
			renderComponent->setHidden(!visible);
		}
	}

	HelicopterRotorComponent::RotorType HelicopterRotorComponent::getRotorType() const
	{
		return mRotorType;
	}

	void HelicopterRotorComponent::setRotorType(RotorType rotorType)
	{
		mRotorType = rotorType;
	}

	HelicopterRotorComponent::RotorPosition HelicopterRotorComponent::getRotorPosition() const
	{
		return mRotorPosition;
	}

	void HelicopterRotorComponent::setRotorPosition(RotorPosition rotorPosition)
	{
		mRotorPosition = rotorPosition;
	}

	float HelicopterRotorComponent::getRotorRealSpeed() const
	{
		return mRotorRealSpeed;
	}

	void HelicopterRotorComponent::setRotorRealSpeed(float rotorRealSpeed)
	{
		mRotorRealSpeed = std::min(std::abs(rotorRealSpeed), SPEED_LIMIT);
	}

	bool HelicopterRotorComponent::hasMaximalSpeed() const
	{
		return (mRotorRealSpeed >= SPEED_LIMIT - glm::epsilon<float>());
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void HelicopterRotorComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint8>(mRotorType);
		serializer.serializeAs<uint8>(mRotorPosition);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
