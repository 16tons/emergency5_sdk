// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline uint64 EntitySelectionManager::getHighlightedEntityId() const
		{
			return mHighlightedEntity;
		}

		inline const EntitySelectionManager::LockedEntitiesSet& EntitySelectionManager::getLockedEntitiesSet() const
		{
			return mLockedEntitiesSet;
		}

		inline uint64 EntitySelectionManager::getSelectedId() const
		{
			// Directly call the private base implementation
			return qsf::EntitySelectionManager::getSelectedId();
		}

		inline const EntitySelectionManager::IdSet& EntitySelectionManager::getSelectedIdSet() const
		{
			// Directly call the private base implementation
			return qsf::EntitySelectionManager::getSelectedIdSet();
		}

		inline size_t EntitySelectionManager::getSelectionSize() const
		{
			// Directly call the private base implementation
			return qsf::EntitySelectionManager::getSelectionSize();
		}

		inline bool EntitySelectionManager::isIdSelected(uint64 id) const
		{
			// Directly call the private base implementation
			return qsf::EntitySelectionManager::isIdSelected(id);
		}

		inline uint64 EntitySelectionManager::getCurrentId() const
		{
			// Directly call the private base implementation
			return qsf::EntitySelectionManager::getCurrentId();
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::SelectionManagerTemplate methods ]
		//[-------------------------------------------------------]
		inline bool EntitySelectionManager::isSelectionValid(uint64 id) const
		{
			// Uninitialized IDs or locked IDs are not valid
			return (isInitialized(id) && mLockedEntitiesSet.find(id) == mLockedEntitiesSet.end());
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
