// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline GroundTypePolygonComponent::GroundTypePolygonComponent(Prototype* prototype) :
		PolygonComponent(prototype),
		mGroundType(0)
	{
		// Nothing to do in here
	}

	inline GroundTypePolygonComponent::~GroundTypePolygonComponent()
	{
		// Nothing to do in here
	}

	inline uint32 GroundTypePolygonComponent::getGroundType() const
	{
		return mGroundType;
	}

	inline void GroundTypePolygonComponent::setGroundType(uint32 groundType)
	{
		assignAndPromoteChange(mGroundType, groundType, GROUND_TYPE);
	}

	inline const Complex2DPolygon& GroundTypePolygonComponent::getCachedComplex2DPolygon() const
	{
		return mCachedPolygon;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
