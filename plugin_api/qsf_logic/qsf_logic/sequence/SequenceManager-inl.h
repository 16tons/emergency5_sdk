// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline Sequence* SequenceManager::getSequenceById(uint64 id) const
		{
			// Find the sequence by its ID
			const SequenceMap::const_iterator iterator = mSequenceMap.find(id);
			return (iterator != mSequenceMap.cend()) ? iterator->second : nullptr;
		}

		inline const SequenceManager::SequenceVector& SequenceManager::getSequences() const
		{
			return mSequenceList;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf
