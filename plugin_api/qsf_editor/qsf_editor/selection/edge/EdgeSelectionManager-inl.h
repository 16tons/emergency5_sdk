// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline const std::vector<StreetEdge>& EdgeSelectionManager::getEdges() const
		{
			return *mEdges;
		}

		inline const std::vector<Node>& EdgeSelectionManager::getNodes() const
		{
			return *mNodes;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
