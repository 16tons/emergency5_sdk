// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <stdexcept>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline uint32 ComponentSystem::getComponentClassTopmostAncestorId(uint32 componentId) const
	{
		const ComponentClassInfoMap::const_iterator iterator = mComponentClassInfoMap.find(componentId);
		if (iterator != mComponentClassInfoMap.end())
		{
			return iterator->second->mTopmostAncestorId;
		}

		// std::exception(const char*) is a non standard extension -> replaced it with std::logic_error
		throw std::logic_error("Unknown component ID");
	}

	inline const std::vector<uint32>& ComponentSystem::getComponentClassAncestorIds(uint32 componentId) const
	{
		const ComponentClassInfoMap::const_iterator iterator = mComponentClassInfoMap.find(componentId);
		if (iterator != mComponentClassInfoMap.end())
		{
			return iterator->second->mAncestorIds;
		}

		// std::exception(const char*) is a non standard extension -> replaced it with std::logic_error
		throw std::logic_error("Unknown component ID");
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* ComponentSystem::getName() const
	{
		return "Component system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
