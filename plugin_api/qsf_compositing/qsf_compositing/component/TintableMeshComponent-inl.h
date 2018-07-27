// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace compositing
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline TintableMeshComponent::TintableMeshComponent(Prototype* prototype) :
			MeshComponent(prototype),
			mTintPaletteIndex(0),
			mInheritTintColor(false),
			mMetallicCarPaint(false),
			mMaterialDirty(false),
			mMaterialChanged(false)
		{
			// Nothing to do in here
		}

		inline TintableMeshComponent::~TintableMeshComponent()
		{
			// Nothing to do in here
		}

		inline const AssetProxy& TintableMeshComponent::getTintPalette() const
		{
			return mTintPalette;
		}

		inline int TintableMeshComponent::getTintPaletteIndex() const
		{
			return mTintPaletteIndex;
		}

		inline bool TintableMeshComponent::getInheritTintColor() const
		{
			return mInheritTintColor;
		}

		inline void TintableMeshComponent::setInheritTintColor(bool inheritTintColor)
		{
			mInheritTintColor = inheritTintColor;
		}

		inline bool TintableMeshComponent::getMetallicCarPaint() const
		{
			return mMetallicCarPaint;
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		inline void TintableMeshComponent::makeMaterialDirty()
		{
			mMaterialDirty = true;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // compositing
} // qsf
