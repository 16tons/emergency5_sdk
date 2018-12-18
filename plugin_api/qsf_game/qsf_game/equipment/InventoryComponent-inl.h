// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline InventoryComponent::InventoryComponent(Prototype* prototype) :
			Component(prototype),
			mActiveEquipmentId(getUninitialized<uint64>()),
			InventoryArray(*this, mInventoryIds)
		{
			// Nothing to do in here
		}

		inline InventoryComponent::~InventoryComponent()
		{
			// Nothing to do in here
		}

		inline void InventoryComponent::setActiveEquipmentId(uint64 id)
		{
			mActiveEquipmentId = id;
		}

		inline uint64 InventoryComponent::getActiveEquipmentId() const
		{
			return mActiveEquipmentId;
		}

		inline bool InventoryComponent::hasActiveEquipment() const
		{
			return isInitialized(mActiveEquipmentId);
		}


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		inline std::size_t InventoryComponent::PrefabIdArrayInterface::size() const
		{
			return getVariable().size();
		}

		inline const std::string& InventoryComponent::PrefabIdArrayInterface::get(std::size_t index) const
		{
			return InventoryComponent::getEquipmentNameFromId(getVariable()[index]);
		}

		inline void InventoryComponent::PrefabIdArrayInterface::set(std::size_t index, const std::string& value)
		{
			getVariable()[index] = InventoryComponent::getEquipmentIdFromName(value);
		}

		inline void InventoryComponent::PrefabIdArrayInterface::insert(std::size_t before, const std::string& value)
		{
			getVariable().insert(getVariable().begin() + before, InventoryComponent::getEquipmentIdFromName(value));
		}

		inline void InventoryComponent::PrefabIdArrayInterface::remove(std::size_t index)
		{
			getVariable().erase(getVariable().begin() + index);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
