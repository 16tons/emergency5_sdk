// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline SelectionManager::~SelectionManager()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline SelectionManager::SelectionManager() :
		// TODO(co) Usually a pluginable should not be used without a pluginable manager, on the other hand, a game might only want to use an entity selection manager directly without a selection system
		Pluginable(nullptr)
	{
		// Nothing here
	}

	inline SelectionManager::SelectionManager(SelectionSystem* selectionSystem) :
		Pluginable(selectionSystem)
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
