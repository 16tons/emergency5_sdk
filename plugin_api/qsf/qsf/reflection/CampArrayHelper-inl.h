// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/reflection/CampArrayHelper.h"
#include "qsf/reflection/CampHelper.h"
#include "qsf/reflection/type/CampBoostPTree.h"

#include <camp/class.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	template <typename T>
	void CampArrayHelper<T>::toBoostPTree(const T& sourceArray, const camp::ArrayProperty& campArrayProperty, boost::property_tree::ptree& pTree)
	{
		// For array types, we do not support default values
		const size_t arraySize = sourceArray.size();
		if (arraySize > 0)
		{
			// Serialize the property in a compact way?
			// -> Use the first element to get the CAMP class of the element type, we now have all information we need to get the "compact"-information
			const bool compact = CampHelper::isCampValueCompact(sourceArray[0]);

			// Save all array elements
			for (size_t arrayElement = 0; arrayElement < arraySize; ++arrayElement)
			{
				const camp::Value campValue = sourceArray[arrayElement];
				boost::property_tree::ptree elementIdPTree;

				if (compact)
				{
					// Save as compact
					elementIdPTree.put("", CampHelper::campValueToString(campValue));
				}
				else
				{
					// Rollout
					CampHelper::campUserObjectToBoostPTree(campValue.to<camp::UserObject>(), elementIdPTree);
				}

				// When saving the Boost ptree as JSON, this will result in a simple JSON array which looks nice
				pTree.push_back(std::make_pair("", elementIdPTree));
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
