// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/spawnpoint/civilist/CivilistSpawnPointComponent.h"

#include <qsf/base/StringParser.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CivilistSpawnPointComponent::COMPONENT_ID = qsf::StringHash("em5::CivilistSpawnPointComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	std::string CivilistSpawnPointComponent::getSpawnPoolsAsString() const
	{
		return qsf::StringParser::combineString(mList);
	}

	void CivilistSpawnPointComponent::setSpawnPoolsAsString(const std::string& spawnPools)
	{
		qsf::StringParser::splitString(spawnPools, mList);
	}

	void CivilistSpawnPointComponent::setSpawnGroupByName(const std::string& spawnGroupName)
	{
		mSpawnGroup = spawnGroupName;
	}

	void CivilistSpawnPointComponent::setTag(const std::string& tag)
	{
		mTag = tag;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
