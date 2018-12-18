// Copyright (C) 2012-2018 Promotion Software GmbH


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
		inline SkinnableMeshComponent::SkinnableMeshComponent(Prototype* prototype) :
			TintableMeshComponent(prototype),
			mMaterialsDirty(false),
			mMaterialChanged(false),
			Materials(*this)
		{
			// Nothing to do in here
		}

		inline SkinnableMeshComponent::~SkinnableMeshComponent()
		{
			// Nothing to do in here
		}

		inline const MaterialVariationManager::Materials& SkinnableMeshComponent::getMaterials() const
		{
			return mMaterials;
		}

		inline void SkinnableMeshComponent::setMaterials(const MaterialVariationManager::Materials& materials)
		{
			mMaterials = materials;
			mMaterialsDirty = true;
		}


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		inline std::size_t SkinnableMeshComponent::MaterialsInterface::size() const
		{
			return getOwner().mMaterials.size();
		}

		inline const GlobalAssetId& SkinnableMeshComponent::MaterialsInterface::get(std::size_t index) const
		{
			return getOwner().mMaterials.at(index);
		}

		inline void SkinnableMeshComponent::MaterialsInterface::set(std::size_t index, const GlobalAssetId& value)
		{
			getOwner().mMaterials.at(index) = value;
			getOwner().mMaterialsDirty = true;
		}

		inline void SkinnableMeshComponent::MaterialsInterface::insert(std::size_t before, const GlobalAssetId& value)
		{
			getOwner().mMaterials.insert(getOwner().mMaterials.begin() + before, value);
			getOwner().mMaterialsDirty = true;
		}

		inline void SkinnableMeshComponent::MaterialsInterface::remove(std::size_t index)
		{
			getOwner().mMaterials.erase(getOwner().mMaterials.begin() + index);
			getOwner().mMaterialsDirty = true;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // compositing
} // qsf
