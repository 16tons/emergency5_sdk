// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LiquidPolygonComponent::LiquidPolygonComponent(Prototype* prototype) :
		PolygonComponent(prototype),
		mMaterial("qsf/material/water/water_surface"),
		mOgreSceneNode(nullptr),
		mOgreEntity(nullptr)
	{
		// Nothing to do in here
	}

	inline LiquidPolygonComponent::~LiquidPolygonComponent()
	{
		// Nothing to do in here
	}

	inline const AssetProxy& LiquidPolygonComponent::getMaterial() const
	{
		return mMaterial;
	}

	inline void LiquidPolygonComponent::setMaterial(const AssetProxy& material)
	{
		if (material != mMaterial)
		{
			mMaterial = material;

			// Update material if we are running (isRunning() check is performed in there)
			if (isRunning() && nullptr != mOgreEntity)
			{
				updateMaterial(*mOgreEntity);
			}

			// Promote the property change
			promotePropertyChange(MATERIAL);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
