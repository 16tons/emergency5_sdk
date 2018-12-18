// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline const AspectTags& AspectComponent::getAspectTags() const
		{
			return mAspectTags;
		}

		inline const std::string& AspectComponent::getTags() const
		{
			return mAspectTags.getTags();
		}

		inline void AspectComponent::setTags(const std::string& tags)
		{
			return mAspectTags.setTags(tags);
		}

		inline AspectComponent::AspectComponent(Prototype* prototype) :
			Component(prototype)
		{
			// Nothing to do
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
