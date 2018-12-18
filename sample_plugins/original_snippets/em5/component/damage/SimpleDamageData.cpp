// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/damage/SimpleDamageData.h"

#include <qsf/prototype/helper/PrototypeManagerHelper.h>
#include <qsf/serialization/binary/StlTypeSerialization.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void SimpleDamageData::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base class implementation
		serializer.serialize(mIntactMesh);
		serializer.serialize(mDamagedMesh);
		serializer.serialize(mDestroyedMesh);

		serializer.serialize(mSmutTexture);

		serializer.serialize(mDamagedLevel1ParticleIds);
		serializer.serialize(mDamagedLevel2ParticleIds);
		serializer.serialize(mDamagedLevel3ParticleIds);
		serializer.serialize(mDestroyedParticleIds);
	}

	void SimpleDamageData::hideParticleChildrenAtMapSave(qsf::BasePrototypeManager& prototypeManager)
	{
		const qsf::PrototypeManagerHelper helper(prototypeManager);
		helper.hideMultipleParticlesById(mDamagedLevel1ParticleIds);
		helper.hideMultipleParticlesById(mDamagedLevel2ParticleIds);
		helper.hideMultipleParticlesById(mDamagedLevel3ParticleIds);
		helper.hideMultipleParticlesById(mDestroyedParticleIds);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
