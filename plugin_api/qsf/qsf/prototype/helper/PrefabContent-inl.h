// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PrototypeManager& PrefabContent::getPrototypeManager() const
	{
		return mPrototypeManager;
	}

	inline bool PrefabContent::empty() const
	{
		return mPrototypes.empty();
	}

	inline Prototype* PrefabContent::getMainPrototype() const
	{
		return empty() ? nullptr : mPrototypes[0];
	}

	inline const PrefabContent::PrototypeArray& PrefabContent::getPrototypes() const
	{
		return mPrototypes;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
