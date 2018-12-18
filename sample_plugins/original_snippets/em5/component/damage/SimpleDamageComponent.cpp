// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/damage/SimpleDamageComponent.h"
#include "em5/component/damage/SimpleDamageData.h"

#include <qsf/map/Entity.h>
#include <qsf/renderer/mesh/MeshComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 SimpleDamageComponent::COMPONENT_ID = qsf::StringHash("em5::SimpleDamageComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void SimpleDamageComponent::hideParticleChildrenAtMapSave()
	{
		mDamageData.hideParticleChildrenAtMapSave(getPrototype().getPrototypeManager());
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void SimpleDamageComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base class implementation
		DamageComponent::serialize(serializer);

		mDamageData.serialize(serializer);
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	void SimpleDamageComponent::applyCurrentMesh()
	{
		// Update the mesh
		const qsf::AssetProxy* meshAssetProxy = getCurrentMesh();
		if (nullptr != meshAssetProxy)
		{
			qsf::MeshComponent* meshComponent = getPrototype().getComponent<qsf::MeshComponent>();
			if (nullptr != meshComponent)
			{
				meshComponent->setMesh(*meshAssetProxy);
			}
		}
	}

	const qsf::AssetProxy* SimpleDamageComponent::getCurrentMesh() const
	{
		const qsf::AssetProxy* assetProxy = nullptr;
		switch (getDamageState())
		{
			case DAMAGESTATE_INTACT:
			case DAMAGESTATE_DAMAGE_LEVEL1:
				assetProxy = &mDamageData.getIntactMesh();
				break;

			case DAMAGESTATE_DAMAGE_LEVEL2:
				assetProxy = &mDamageData.getDamagedMesh();
				break;

			case DAMAGESTATE_DESTROYED:
			default:
				assetProxy = &mDamageData.getDestroyedMesh();
				break;
		}

		// There must always be a valid current mesh, in case there's no damage mesh edited, use the default mesh as fallback
		return (nullptr != assetProxy && assetProxy->isValid()) ? assetProxy : nullptr;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
