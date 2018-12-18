// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Map* GameLogicManager::getMap() const
	{
		return mMap;
	}

	template <typename TYPE>
	TYPE& GameLogicManager::insertLogic()
	{
		// Create the logic
		TYPE* logic = new TYPE();

		// Insert it
		insertNewLogic(*logic);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
