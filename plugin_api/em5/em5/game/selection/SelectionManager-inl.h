// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/binary/BasicTypeSerialization.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline SelectionManager::SelectionManager()
	{
		// Nothing here
	}

	inline SelectionManager::~SelectionManager()
	{
		// Nothing here
	}

	inline void SelectionManager::serialize(qsf::BinarySerializer& serializer)
	{
		const IdSet& set = getSelectedIdSet();
		uint32 idSetSize = static_cast<uint32>(set.size());
		uint64 currentId = getCurrentId();
		serializer.serialize(idSetSize);
		serializer.serialize(currentId);

		if (serializer.isReading())
		{
			// Reading
			clearSelection();
			for (uint32 i = 0; i < idSetSize; ++i)
			{
				// Serialize
				uint64 id = qsf::getUninitialized<uint64>();
				serializer.serialize(id);
				addIdToSelection(id);
			}
			setCurrentId(currentId);
		}
		else
		{
			for (uint64 id : set)
			{
				serializer.serialize(id);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
