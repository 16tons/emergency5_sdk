// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		inline AiLodComponent::AiLodComponent(Prototype* prototype) :
			Component(prototype),
			mLod(MAX_LOD)
		{
		}

		inline AiLodComponent::~AiLodComponent()
		{
		}

		inline uint32 AiLodComponent::getLOD() const
		{
			return mLod;
		}

		inline void AiLodComponent::setLOD(uint32 lod)
		{
			if (lod <= MIN_LOD) lod = MIN_LOD;
			else if (lod > MAX_LOD) lod = MAX_LOD;
			assignAndPromoteChange(mLod, lod, LOD_PROPERTY_ID);
		}

		inline bool AiLodComponent::isMaxLOD() const
		{
			return mLod == MAX_LOD;
		}

		inline bool AiLodComponent::isMinLOD() const
		{
			return mLod == MIN_LOD;
		}

		inline void AiLodComponent::setMaxLOD()
		{
			setLOD(MAX_LOD);
		}

		inline void AiLodComponent::setMinLOD()
		{
			setLOD(MIN_LOD);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
