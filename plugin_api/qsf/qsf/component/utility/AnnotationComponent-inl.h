// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline AnnotationComponent::AnnotationComponent(Prototype* prototype) :
		Component(prototype),
		mAnnotationType(BUG_TYPE),
		mSeverityLevel(NORMAL_SEVERITY)
	{
		// Nothing to do in here
	}

	inline AnnotationComponent::~AnnotationComponent()
	{
		// Nothing to do in here
	}

	inline const std::string& AnnotationComponent::getAuthor() const
	{
		return mAuthor;
	}

	inline const std::string& AnnotationComponent::getName() const
	{
		return mName;
	}

	inline const std::string& AnnotationComponent::getDescription() const
	{
		return mDescription;
	}

	inline AnnotationComponent::AnnotationType AnnotationComponent::getAnnotationType() const
	{
		return mAnnotationType;
	}

	inline AnnotationComponent::SeverityLevel AnnotationComponent::getSeverityLevel() const
	{
		return mSeverityLevel;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
