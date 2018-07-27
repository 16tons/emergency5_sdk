// Copyright (C) 2012-2017 Promotion Software GmbH


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
		inline AspectTags::AspectTags()
		{
			setTags(DEFAULT_TAGS);
		}

		inline AspectTags::AspectTags(const std::string& defaultTags)
		{
			setTags(defaultTags);
		}


		inline const std::string& AspectTags::getTags() const
		{
			return mTags;
		}

		inline size_t AspectTags::getNumTags() const
		{
			return mTagHashs.size();
		}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
