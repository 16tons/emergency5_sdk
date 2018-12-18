// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/selection/SelectionRectangle.h>

#include "qsf_editor/selection/node/NodeSelectionManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Pixel space selection rectangle for selecting nodes
		*/
		class NodeSelectionRectangle : public SelectionRectangle
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit NodeSelectionRectangle(RenderWindow& renderWindow);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~NodeSelectionRectangle();

			/**
			*  @brief
			*    Calculate and return the selected nodes inside the rectangle
			*
			*  @param[in] nodesComponent
			*    Nodes component instance to select nodes from
			*  @param[out] nodeIdSet
			*    Node ID set to be filled; will not be automatically cleared
			*/
			void getNodeSelection(NodesComponent& nodesComponent, NodeSelectionManager::IdSet& nodeIdSet) const;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
