// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vehicle/parts/WheelComponent.h"

#include <qsf/serialization/binary/GlmTypeSerialization.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/math/Math.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/renderer/RendererSystem.h>
#include <qsf/renderer/material/MaterialSystem.h>
#include <qsf/renderer/material/material/MaterialProperty.h>
#include <qsf/renderer/material/material/MaterialVariationManager.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 WheelComponent::COMPONENT_ID = qsf::StringHash("em5::WheelComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	bool WheelComponent::isChain() const
	{
		return (mWheelType == WHEELTYPE_CHAIN_RIGHT || mWheelType == WHEELTYPE_CHAIN_LEFT);
	}

	void WheelComponent::updateWheel(float movedDistance, float turningAngle)
	{
		if (isRunning())
		{
			if (isChain())
			{
				float move = movedDistance;

				if (mWheelType == WHEELTYPE_CHAIN_RIGHT)
				{
					move += mDistanceFromTurnPoint * turningAngle;
				}
				if (mWheelType == WHEELTYPE_CHAIN_LEFT)
				{
					move -= mDistanceFromTurnPoint * turningAngle;
				}

				mChainOffsetU = std::fmod(mChainOffsetU + move * mChainSpeedU, 1.0f);
				mChainOffsetV = std::fmod(mChainOffsetV + move * mChainSpeedV, 1.0f);

				// Set material properties
				static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();
				materialVariationManager.setEntityMaterialPropertyValue(getEntity(), "UvOffsetU", qsf::MaterialPropertyValue::fromFloat(mChainOffsetU));
				materialVariationManager.setEntityMaterialPropertyValue(getEntity(), "UvOffsetV", qsf::MaterialPropertyValue::fromFloat(mChainOffsetV));
			}
			else
			{
				if (mWheelType != WHEELTYPE_FRONT_LEFT && mWheelType != WHEELTYPE_FRONT_RIGHT)
				{
					// Turn only front wheels
					turningAngle = 0;
				}

				// Get wheel link component
				qsf::LinkComponent* linkComponent = getEntity().getComponent<qsf::LinkComponent>();
				if (nullptr != linkComponent)
				{
					glm::quat localRotation = mOriginalLocalRotation;

					// Apply curve angle rotation (around up axis)
					if (turningAngle != 0.0f)
					{
						localRotation = localRotation * qsf::Math::getRotationAroundAxis(qsf::CoordinateSystem::getUp(), glm::radians(turningAngle));
					}

					// Rotate wheel around its right axis
					if (movedDistance != 0.0f)
					{
						// Rotate wheel a bit further
						mAngleInRadians -= movedDistance / mRadius;
						mAngleInRadians = std::fmod(mAngleInRadians, 2 * glm::pi<float>());
						localRotation = localRotation * qsf::Math::getRotationAroundAxis(qsf::CoordinateSystem::getRight(), mAngleInRadians);
					}

					// Apply the result
					linkComponent->setLocalRotation(localRotation);
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void WheelComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mOriginalLocalRotation);
		serializer.serialize(mRadius);
		serializer.serialize(mAngleInRadians);
		serializer.serializeAs<uint16>(mWheelType);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
