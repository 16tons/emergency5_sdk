// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/link/LinkComponent.h"
#include "qsf/base/NamedIdentifier.h"

#include <unordered_map>


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
	*    Static slot helper class
	*/
	class QSF_API_EXPORT SlotHelper
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		 * Keywords to use when defining link types for slots.
		 * Use these values as parameters for QSF_CAMP_SLOT definitions during export to CAMP.
		 */
		//@{
		static const std::string HARD_LINK_TYPE;
		static const std::string SOFT_LINK_TYPE;
		static const std::string CUSTOM_SLOT_TYPE;
		//@}

		typedef std::unordered_map<uint64, uint64> UniqueIdMap;
		typedef boost::container::flat_set<uint64> UniqueIdSet;

		// Slot identifier
		struct QSF_API_EXPORT SlotIdentifier
		{
			Component&				mRootComponent;
			const camp::Property&	mRootProperty;
			const camp::UserObject	mUserObject;
			const camp::Property&	mProperty;

			SlotIdentifier(Component& component, const camp::Property& campProperty);
			SlotIdentifier(Component& component, const camp::Property& rootCampProperty, const camp::UserObject& campUserObject, const camp::Property& campProperty);
		};

		// Functor base class
		struct Functor
		{
			enum Result
			{
				NO_CHANGE,
				REPLACE,
				REMOVE
			};

			virtual Result operator()(uint64&) const = 0;
		};

		// Replace functor
		struct ReplaceFunctor : public Functor
		{
			explicit ReplaceFunctor(const UniqueIdMap& uniqueIdMap);
			virtual Result operator()(uint64&) const override;

		private:
			const UniqueIdMap& mUniqueIdMap;
		};

		// RemoveFunctor
		struct RemoveFunctor : public Functor
		{
			explicit RemoveFunctor(const UniqueIdSet& idsToRemove);
			virtual Result operator()(uint64&) const override;

		private:
			const UniqueIdSet& mIdsToRemove;
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Collect all slots of the given prototype
		*
		*  @param[in] prototype
		*    The prototype that will be searched for slot properties
		*  @param[in] onlyLinkSlots
		*    If "true", only slots that are properties of a component get listed; if "false", all ID references get listed, including link component's parent ID and slots inside of properties
		*  @param[out] outListing
		*    The resulting list of slots, each one identified by a component ID and the slot property's name
		*/
		static void collectSlotsForPrototype(const Prototype& prototype, bool onlyLinkSlots, std::vector<SlotIdentifier>& outListing);

		/**
		*  @brief
		*    Collect all direct children linked into any slot of the given prototype
		*
		*  @param[in] prototype
		*    The prototype that will be searched for slot properties
		*  @param[out] outListing
		*    The resulting list of slots, each one identified by a component ID and the slot property's name
		*/
		static void collectChildrenInSlotsForPrototype(const Prototype& prototype, std::unordered_map<uint64, SlotIdentifier>& outListing);

		/**
		*  @brief
		*    Get link type of a given property
		*
		*  @param[in] campProperty
		*    The CAMP property to check
		*
		*  @return
		*    The property's link type, can also be LinkComponent::INVALID_LINK_TYPE
		*/
		static LinkComponent::LinkType getLinkTypeForProperty(const camp::Property& campProperty);

		/**
		*  @brief
		*    Collect all slots of the given prototype
		*
		*  @param[in] prototype
		*    The prototype that will be searched for slot properties
		*  @param[in] uniqueIdMapping
		*    Maps prototype IDs that should be replaced to new prototype IDs that should be used instead
		*
		*  @remarks
		*    IDs in slots and links that don't appear as keys in the unique ID mapping will be removed
		*/
		static void replaceLinkAndSlotContentsInPrototype(const Prototype& prototype, const UniqueIdMap& uniqueIdMapping);

		/**
		*  @brief
		*    Removes certain IDs from all slots of the given prototype
		*
		*  @param[in] prototype
		*    The prototype that will be searched for slot properties
		*  @param[in] idsToRemove
		*    Set of prototype IDs to remove from all slots
		*/
		static void removeIdsFromSlotContentsInPrototype(const Prototype& prototype, const UniqueIdSet& idsToRemove);

		/**
		*  @brief
		*    Process all slots in the given prototype with a functor
		*
		*  @param[in] prototype
		*    The prototype that will be searched for slot properties
		*  @param[in] functor
		*    Functor to apply to all slots; it tells the method whether to remove or replace slot contents
		*/
		static void processPrototypeWithFunctor(const Prototype& prototype, const Functor& functor);


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Collect slots inside a given CAMP user object
		*
		*  @param[in] component
		*    Component instance we originally came from
		*  @param[in] campPropertyRoot
		*    CAMP property instance we originally came from, or a null pointer if it is not defined yet
		*  @param[in] campUserObject
		*    CAMP user object to check
		*  @param[in] onlyLinkSlots
		*    If "true", only slots that are properties of a component get listed; if "false", all ID references get listed, including link component's parent ID and slots inside of properties
		*  @param[out] outListing
		*    The resulting list of slots, each one identified by a component ID and the slot property's name
		*/
		static void collectSlotsForCampUserObject(Component& component, const camp::Property* campPropertyRoot, const camp::UserObject& campUserObject, bool onlyLinkSlots, std::vector<SlotIdentifier>& outListing);

		/**
		*  @brief
		*    Process the given slot with a functor
		*
		*  @param[in] slotIdentifier
		*    Slot to apply the functor to
		*  @param[in] functor
		*    Functor to apply; it tells the method whether to remove or replace slot contents
		*/
		static void processSlotWithFunctor(const SlotIdentifier& slotIdentifier, const Functor& functor);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
