// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/CleanXutilityForBoostContainer.h"	// Include this first to eliminate the compiler warning "warning C4996: 'std::_Copy_impl': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"
#include "qsf/selection/SelectionManager.h"

#include <boost/container/flat_set.hpp>
#include <boost/noncopyable.hpp>
#include <boost/signals2.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    QSF selection manager template class
	*/
	template <typename IdType>
	class SelectionManagerTemplate : public SelectionManager
	{


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void(IdType)> Selected;		///< This Boost signal is emitted once for each ID selected
		boost::signals2::signal<void(IdType)> Deselected;	///< This Boost signal is emitted once for each ID deselected


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef boost::container::flat_set<IdType> IdSet;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SelectionManagerTemplate();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SelectionManagerTemplate();

		/**
		*  @brief
		*    Return the first selected ID
		*
		*  @return
		*    The selected ID, in case of multiple selected IDs the first one will be returned
		*/
		IdType getSelectedId() const;

		/**
		*  @brief
		*    Return whole selection as set of IDs
		*
		*  @return
		*    Whole selection as set of IDs, do not destroy the instance
		*/
		const IdSet& getSelectedIdSet() const;

		/**
		*  @brief
		*    Return the number of currently selected IDs
		*
		*  @return
		*    The number of currently selected IDs
		*/
		size_t getSelectionSize() const;

		/**
		*  @brief
		*    Check whether or not the given ID is currently selected
		*
		*  @return
		*    "true" in case the given ID is currently selected, else "false"
		*/
		bool isIdSelected(IdType id) const;

		/**
		*  @brief
		*    Clear the current selection
		*/
		void clearSelection();

		/**
		*  @brief
		*    Set the current selection by using an ID
		*
		*  @param[in] id
		*    ID to select after clearing the previous selection, setting "qsf::getUninitialized<IdType>()" just clears the selection
		*/
		void setSelectionById(IdType id);

		/**
		*  @brief
		*    Set the current selection by using an ID set
		*
		*  @param[in] idSet
		*    IDs to select after clearing the previous selection, "qsf::getUninitialized<IdType>()" is silently ignored
		*/
		void setSelectionByIdSet(const IdSet& idSet);

		/**
		*  @brief
		*    Add an ID to the current selection
		*
		*  @param[in] id
		*    ID to add to the current selection, already selected IDs or "qsf::getUninitialized<IdType>()" are silently ignored
		*/
		void addIdToSelection(IdType id);

		/**
		*  @brief
		*    Add multiple IDs to the current selection
		*
		*  @param[in] idSet
		*    IDs to add to the current selection, already selected IDs or "qsf::getUninitialized<IdType>()" are silently ignored
		*/
		void addIdSetToSelection(const IdSet& idSet);

		/**
		*  @brief
		*    Remove an ID from the current selection
		*
		*  @param[in] id
		*    ID to remove from the current selection, not selected IDs or "qsf::getUninitialized<IdType>()" are silently ignored
		*/
		void removeIdFromSelection(IdType id);

		/**
		*  @brief
		*    Remove multiple IDs from the current selection
		*
		*  @param[in] idSet
		*    IDs to remove from the current selection, not selected IDs or "qsf::getUninitialized<IdType>()" are silently ignored
		*/
		void removeIdSetFromSelection(const IdSet& idSet);

		/**
		*  @brief
		*    Return the current ID (the focus of attention)
		*
		*  @return
		*    The current ID, "qsf::getUninitialized<IdType>()" is there's no current ID
		*
		*  @remarks
		*    Usually the current ID is equal to the last selected ID. It's possible to explicitly set this current
		*    ID, although it's recommended to not do so if it can be avoided. Do also not use the information about
		*    the current ID if you don't have to and use the general selected ID set whenever possible.
		*/
		IdType getCurrentId() const;

		/**
		*  @brief
		*    Set the current ID (the focus of attention)
		*
		*  @param[in] id
		*    ID to be used as current ID, "qsf::getUninitialized<IdType>()" is there's no current ID
		*
		*  @return
		*    Explicitly sets the current ID
		*
		*  @see
		*    - qsf::SelectionManagerTemplate::getCurrentId()
		*/
		void setCurrentId(IdType id);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] selectionSystem
		*    The selection system this selection manager instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
		*/
		explicit SelectionManagerTemplate(SelectionSystem* selectionSystem);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::SelectionManagerTemplate methods ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Check whether or not the given selection is valid
		*
		*  @param[in] id
		*    ID to validate
		*
		*  @return
		*    "true" if the given selection is valid, else "false"
		*
		*  @note
		*    - The default implementation just rejects uninitialized IDs, derived classes might e.g. also check for existing selection locks
		*/
		virtual bool isSelectionValid(IdType id) const;

		/**
		*  @brief
		*    This method is called whenever there's a newly selected ID
		*
		*  @param[in] id
		*    Newly selected ID
		*/
		virtual void onSelect(IdType id);

		/**
		*  @brief
		*    This method is called whenever a previously selected ID is selected no more
		*
		*  @param[in] id
		*    The new longer selected ID
		*/
		virtual void onDeselect(IdType id);

		/**
		*  @brief
		*    This method is called whenever the current ID has been changed
		*
		*  @param[in] id
		*    The new current ID
		*
		*  @note
		*    - The default implementation is empty
		*/
		virtual void onCurrentChange(IdType id);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IdSet  mSelectedIds;	///< List of currently selected IDs
		IdType mCurrentId;		///< The current ID, "qsf::getUninitialized<IdType>()" if there's no current ID


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/selection/SelectionManagerTemplate-inl.h"
