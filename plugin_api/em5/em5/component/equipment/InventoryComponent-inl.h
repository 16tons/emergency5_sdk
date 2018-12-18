// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline InventoryComponent::InventoryComponent(qsf::Prototype* prototype) :
		qsf::game::InventoryComponent(prototype)
	{
		// Nothing to do in here
	}

	inline InventoryComponent::~InventoryComponent()
	{
		// Nothing to do in here
	}

	inline const std::string& InventoryComponent::getActiveEquipmentName() const
	{
		return mEquipmentName;
	}

	inline void InventoryComponent::setActiveEquipmentName(const std::string& name)
	{
		mEquipmentName = name;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
