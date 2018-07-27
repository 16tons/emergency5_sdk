// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/GetUninitialized.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	template <typename IdType>
	SelectionManagerTemplate<IdType>::SelectionManagerTemplate() :
		mCurrentId(getUninitialized<IdType>())
	{
		// Nothing here
	}

	template <typename IdType>
	SelectionManagerTemplate<IdType>::~SelectionManagerTemplate()
	{
		// Nothing here
	}

	template <typename IdType>
	IdType SelectionManagerTemplate<IdType>::getSelectedId() const
	{
		return mSelectedIds.empty() ? getUninitialized<IdType>() : *mSelectedIds.begin();
	}

	template <typename IdType>
	const typename SelectionManagerTemplate<IdType>::IdSet& SelectionManagerTemplate<IdType>::getSelectedIdSet() const
	{
		return mSelectedIds;
	}

	template <typename IdType>
	size_t SelectionManagerTemplate<IdType>::getSelectionSize() const
	{
		return mSelectedIds.size();
	}

	template <typename IdType>
	bool SelectionManagerTemplate<IdType>::isIdSelected(IdType id) const
	{
		typename IdSet::const_iterator iterator = mSelectedIds.find(id);
		return (iterator != mSelectedIds.end());
	}

	template <typename IdType>
	void SelectionManagerTemplate<IdType>::clearSelection()
	{
		for (IdType id : mSelectedIds)
		{
			onDeselect(id);
		}
		mSelectedIds.clear();
		setCurrentId(getUninitialized<IdType>());
	}

	template <typename IdType>
	void SelectionManagerTemplate<IdType>::setSelectionById(IdType id)
	{
		// Deselect the no longer selected IDs
		for (IdType currentId : mSelectedIds)
		{
			// Is the ID inside the current selection but not inside the new selection?
			if (currentId != id)
			{
				onDeselect(currentId);
			}
		}

		// Select the new ID
		if (isSelectionValid(id))
		{
			// Set the new selection
			const bool previouslySelected = isIdSelected(id);
			mSelectedIds.clear();
			mSelectedIds.insert(id);
			setCurrentId(id);
			if (!previouslySelected)
			{
				onSelect(id);
			}
		}
		else
		{
			mSelectedIds.clear();
			setCurrentId(getUninitialized<IdType>());
		}
	}

	template <typename IdType>
	void SelectionManagerTemplate<IdType>::setSelectionByIdSet(const IdSet& idSet)
	{
		// Deselect the no longer selected IDs
		for (IdType id : mSelectedIds)
		{
			// Is the ID inside the current selection but not inside the new selection?
			typename IdSet::const_iterator iterator = idSet.find(id);
			if (iterator == idSet.end())
			{
				onDeselect(id);
			}
		}

		// Select the new IDs
		IdSet newIdSet;
		IdSet newSelectIdSet;
		for (IdType id : idSet)
		{
			if (isSelectionValid(id))
			{
				if (!isIdSelected(id))
				{
					newSelectIdSet.insert(id);
				}
				newIdSet.insert(id);
			}
		}

		// Set the new selection
		mSelectedIds = newIdSet;
		setCurrentId(mSelectedIds.empty() ? getUninitialized<IdType>() : *(--mSelectedIds.cend()));
		for (IdType id : newSelectIdSet)
		{
			onSelect(id);
		}
	}

	template <typename IdType>
	void SelectionManagerTemplate<IdType>::addIdToSelection(IdType id)
	{
		if (isSelectionValid(id))
		{
			const bool wasInserted = mSelectedIds.insert(id).second;
			if (wasInserted)
			{
				setCurrentId(id);
				onSelect(id);
			}
		}
	}

	template <typename IdType>
	void SelectionManagerTemplate<IdType>::addIdSetToSelection(const IdSet& idSet)
	{
		if (!idSet.empty())
		{
			IdType newCurrentId = getUninitialized<IdType>();
			for (IdType id : idSet)
			{
				if (isSelectionValid(id))
				{
					const bool wasInserted = mSelectedIds.insert(id).second;
					if (wasInserted)
					{
						newCurrentId = id;
						onSelect(id);
					}
				}
			}

			if (qsf::isInitialized(newCurrentId))
			{
				setCurrentId(newCurrentId);
			}
		}
	}

	template <typename IdType>
	void SelectionManagerTemplate<IdType>::removeIdFromSelection(IdType id)
	{
		// Removing uninitialized IDs is not valid
		if (isInitialized(id))
		{
			// Get the ID iterator
			typename IdSet::iterator iterator = mSelectedIds.find(id);
			if (iterator != mSelectedIds.end())
			{
				// Erase and deselect
				mSelectedIds.erase(iterator);
				onDeselect(id);

				// Did we just lost our current ID?
				if (mCurrentId == id)
				{
					setCurrentId(mSelectedIds.empty() ? getUninitialized<IdType>() : *(--mSelectedIds.cend()));
				}
			}
		}
	}

	template <typename IdType>
	void SelectionManagerTemplate<IdType>::removeIdSetFromSelection(const IdSet& idSet)
	{
		if (!idSet.empty())
		{
			bool currentIdRemoved = false;
			for (IdType id : idSet)
			{
				// Removing uninitialized IDs is not valid
				if (isInitialized(id))
				{
					// Get the ID iterator
					typename IdSet::iterator iterator = mSelectedIds.find(id);
					if (iterator != mSelectedIds.end())
					{
						// Erase and deselect
						mSelectedIds.erase(iterator);
						onDeselect(id);

						// Did we just lost our current ID?
						if (mCurrentId == id)
						{
							currentIdRemoved = true;
						}
					}
				}
			}

			if (currentIdRemoved)
			{
				setCurrentId(mSelectedIds.empty() ? getUninitialized<IdType>() : *(--mSelectedIds.cend()));
			}
		}
	}

	template <typename IdType>
	IdType SelectionManagerTemplate<IdType>::getCurrentId() const
	{
		return mCurrentId;
	}

	template <typename IdType>
	void SelectionManagerTemplate<IdType>::setCurrentId(IdType id)
	{
		// State change?
		if (mCurrentId != id)
		{
			if (isSelectionValid(id))
			{
				typename IdSet::iterator iterator = mSelectedIds.find(id);
				if (iterator != mSelectedIds.end())
				{
					mCurrentId = id;
					onCurrentChange(mCurrentId);
				}
			}
			else
			{
				setUninitialized(mCurrentId);
				onCurrentChange(mCurrentId);
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	template <typename IdType>
	SelectionManagerTemplate<IdType>::SelectionManagerTemplate(SelectionSystem* selectionSystem) :
		SelectionManager(selectionSystem),
		mCurrentId(getUninitialized<IdType>())
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::SelectionManagerTemplate methods ]
	//[-------------------------------------------------------]
	template <typename IdType>
	bool SelectionManagerTemplate<IdType>::isSelectionValid(IdType id) const
	{
		// Uninitialized IDs are not valid
		return isInitialized(id);
	}

	template <typename IdType>
	void SelectionManagerTemplate<IdType>::onSelect(IdType id)
	{
		// Trigger Boost signal
		Selected(id);
	}

	template <typename IdType>
	void SelectionManagerTemplate<IdType>::onDeselect(IdType id)
	{
		// Trigger Boost signal
		Deselected(id);
	}

	template <typename IdType>
	void SelectionManagerTemplate<IdType>::onCurrentChange(IdType id)
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
