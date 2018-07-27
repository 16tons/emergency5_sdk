// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/operation/OperationHelper.h"

#include <qsf/component/nodes/Node.h>
#include <qsf/component/street/StreetGateway.h>
#include <qsf/component/street/crossing/StreetEdge.h>

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class StreetSectionComponent;
	namespace editor
	{
		namespace base
		{
			class Operation;
		}
	}
}


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
		*    Static section component operation helper class
		*/
		class QSF_EDITOR_API_EXPORT StreetOperationHelper : public OperationHelper
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Initial type
			*/
			enum InitialType
			{
				IT_EMPTY = 0,	///< Empty
				IT_SIMPLE,		///< An one sided way with three nodes
				IT_COMPLEX,		///< An two sided way, three nodes each
				IT_SIMPLE_T,	///< An way with a right corner, four nodes, two edges
				IT_COMPLEX_T,	///< An three sided way with two directions
				IT_SIMPLE_X,	///< An way with two in, two out
				IT_COMPLEX_X	///< An four sided way with lanes for every direction
			};

			/**
			*  @brief
			*    Gateway information
			*/
			struct Gateway
			{
				StreetGatewayLink streetGatewayLink;	///< Street gateway link
				glm::vec3		  position;				///< Position is cached for speedup
				glm::vec3		  direction;			///< Direction is cached for speedup
				bool			  crossingGateway;		///< Gateway from a street crossing
			};


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Build an operation that construct a street crossing component for the given entity
			*
			*  @param[in] entity
			*    Entity to create the street crossing component in
			*  @param[in] initialType
			*    Type of the street crossing content
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildStreetCrossingComponentOperation(Entity& entity, InitialType initialType);

			/**
			*  @brief
			*    Build an operation that creates an entity with a street section component instance
			*
			*  @param[in] name
			*    The general name of the entity instance, the operation might change the name in detail
			*  @param[in] nodes
			*    Local space nodes of the street section
			*  @param[in] centerPosition
			*    World space center position
			*  @param[in] beginStreetGatewayLink
			*    Begin street gateway link
			*  @param[in] endStreetGatewayLink
			*    End street gateway link
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildCreateStreetSectionOperation(const std::string& name, uint64& entityId, const std::vector<Node>& nodes, const glm::vec3& centerPosition, const StreetGatewayLink& beginStreetGatewayLink, const StreetGatewayLink& endStreetGatewayLink);

			/**
			*  @brief
			*    Build an operation that directly interconnects two street gateways with each other
			*
			*  @param[in] firstStreetGatewayLink
			*    First street gateway link
			*  @param[in] secondStreetGatewayLink
			*    Second street gateway link
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildDirectStreetInterconnectionOperation(const StreetGatewayLink& firstStreetGatewayLink, const StreetGatewayLink& secondStreetGatewayLink);

			/**
			*  @brief
			*    Build an operation that directly disjoins two street gateways from each other
			*
			*  @param[in] firstStreetGatewayLink
			*    First street gateway link
			*  @param[in] secondStreetGatewayLink
			*    Second street gateway link
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildDirectStreetDisjoinOperation(const StreetGatewayLink& firstStreetGatewayLink, const StreetGatewayLink& secondStreetGatewayLink);

			/**
			*  @brief
			*    Build an operation that interconnects all street gateways which are close enough together to be interconnected
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildInterconnectionAllStreetGatewaysOperation();

			//[-------------------------------------------------------]
			//[ Gateway helper                                        ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Gather all street gateways
			*
			*  @param[out] gateways
			*    Receives the found street gateways, list is not cleared before new entries are added
			*/
			static void gatherStreetGateways(std::vector<Gateway>& gateways);


		//[-------------------------------------------------------]
		//[ Private static methods                                ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Build an operation that directly interconnects two street gateways with each other
			*
			*  @param[in] sourceStreetGatewayLink
			*    Source street gateway link which is going to be connected to another street gateway
			*  @param[in] destinationStreetGatewayLink
			*    Destination street gateway link to connect
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* privateBuildDirectStreetInterconnectionOperation(const StreetGatewayLink& sourceStreetGatewayLink, const StreetGatewayLink& destinationStreetGatewayLink);

			/**
			*  @brief
			*    Create a new node instance with the given local space node position and the default node radius
			*
			*  @param[in] position
			*    Local space node position
			*/
			static Node createNode(const glm::vec3& position);

			// Street crossing content creation
			static void initWithSimpleCluster(std::vector<Node>& nodes, std::vector<StreetEdge>& streetEdges, std::vector<StreetGateway>& streetGateways);
			static void initWithComplexCluster(std::vector<Node>& nodes, std::vector<StreetEdge>& streetEdges, std::vector<StreetGateway>& streetGateways);
			static void initWithSimpleTCluster(std::vector<Node>& nodes, std::vector<StreetEdge>& streetEdges, std::vector<StreetGateway>& streetGateways);
			static void initWithComplexTCluster(std::vector<Node>& nodes, std::vector<StreetEdge>& streetEdges, std::vector<StreetGateway>& streetGateways);
			static void initWithSimpleXCluster(std::vector<Node>& nodes, std::vector<StreetEdge>& streetEdges, std::vector<StreetGateway>& streetGateways);
			static void initWithComplexXCluster(std::vector<Node>& nodes, std::vector<StreetEdge>& streetEdges, std::vector<StreetGateway>& streetGateways);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
