// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline EntitySelectionManager::EntitySelectionManager()
	{
		// Nothing here
	}

	inline EntitySelectionManager::EntitySelectionManager(SelectionSystem* selectionSystem) :
		SelectionManagerTemplate<uint64>(selectionSystem)
	{
		// Nothing here
	}

	inline EntitySelectionManager::~EntitySelectionManager()
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
