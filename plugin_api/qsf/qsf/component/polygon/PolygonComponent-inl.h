// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PolygonComponent::PolygonComponent(Prototype* prototype) :
		NodesComponent(prototype),
		mHeight(10.0f)
	{
		// Nothing here
	}

	inline PolygonComponent::~PolygonComponent()
	{
		// Nothing here
	}

	inline float PolygonComponent::getHeight() const
	{
		return mHeight;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::NodesComponent methods         ]
	//[-------------------------------------------------------]
	inline bool PolygonComponent::closedContinuousLines() const
	{
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
