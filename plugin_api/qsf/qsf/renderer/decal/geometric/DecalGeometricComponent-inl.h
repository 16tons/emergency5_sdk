// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline DecalGeometricComponent::DecalGeometricComponent(Prototype* prototype) :
		DecalComponent(prototype),
		mOgreEntity(nullptr),
		mDirtyMesh(false),	// By default, there are no nodes so no procedural decal mesh is required
		mUseAsLocal(false)
	{
		// Nothing to do in here
	}

	inline DecalGeometricComponent::~DecalGeometricComponent()
	{
		removeDecal();
	}

	inline bool DecalGeometricComponent::getUseAsLocal() const
	{
		return mUseAsLocal;
	}

	inline void DecalGeometricComponent::setUseAsLocal(bool local)
	{
		assignAndPromoteChange(mUseAsLocal, local, USE_AS_LOCAL);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
