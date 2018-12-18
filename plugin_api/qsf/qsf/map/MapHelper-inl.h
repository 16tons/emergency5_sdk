// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MapHelper::MapHelper(Map& map) :
		mMap(map)
	{
		// Nothing to do in here
	}

	inline MapHelper::~MapHelper()
	{
		// Nothing to do in here
	}

	inline Map& MapHelper::getMap() const
	{
		return mMap;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
