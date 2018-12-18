// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline const std::string& ModSystem::getModsUserDirectory() const
	{
		return mModsUserDirectory;
	}

	inline const boost::container::flat_map<uint32, Mod*>& ModSystem::getMods() const
	{
		return mMods.getElements();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* ModSystem::getName() const
	{
		return "Mod system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
