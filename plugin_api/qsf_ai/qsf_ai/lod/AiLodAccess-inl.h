// Copyright (C) 2012-2018 Promotion Software GmbH
namespace qsf
{

	namespace ai
	{

		inline AiLodAccess::AiLodAccess(const Entity& entity) :
			mAiLodComponent(nullptr),
			mEntity(entity),
			mCheckIfBecomesNull(true)
		{
		}

		inline AiLodAccess::AiLodAccess(AiLodComponent& component) :
			mAiLodComponent(component),
			mEntity(component.getEntity()),
			mCheckIfBecomesNull(true)
		{
		}

		inline void AiLodAccess::clear()
		{
			mAiLodComponent.clear();
			mCheckIfBecomesNull = true;
		}

		inline AiLodAccess::operator bool() const
		{
			return valid();
		}

		inline AiLodComponent& AiLodAccess::operator*() const
		{
			return mAiLodComponent.getSafe();
		}

		inline AiLodComponent* AiLodAccess::operator->() const
		{
			return &mAiLodComponent.getSafe();
		}

		uint32 AiLodAccess::getLOD() const
		{
			return valid() ? mAiLodComponent->getLOD() : AiLodComponent::MAX_LOD;
		}

		void AiLodAccess::setLOD(uint32 lod)
		{
			if (valid())
				mAiLodComponent->setLOD(lod);
		}

		bool AiLodAccess::isMaxLOD() const
		{
			return valid() ? mAiLodComponent->isMaxLOD() : true;
		}

		bool AiLodAccess::isMinLOD() const
		{
			return valid() ? mAiLodComponent->isMinLOD() : false;
		}

		void AiLodAccess::setMaxLOD()
		{
			if (valid())
				mAiLodComponent->setMaxLOD();
		}

		void AiLodAccess::setMinLOD()
		{
			if (valid())
				mAiLodComponent->setMinLOD();
		}

	}
}
