// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/component/nodes/Node.h"
#include "qsf/debug/DebugDrawProxy.h"

#include <glm/glm.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Ray;
	class Color4;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract node collection component class
	*
	*  @note
	*    - All nodes are stored in local space coordinates. If working with them, you will probably have to convert them to world space (e.g. using the EntityHelper).
	*    - Nodes are identified using their internal index, beginning with 0.
	*
	*  @todo
	*    - TODO(co) Sadly, the interface is error prone and a little bit complicated at some places, make a refactoring of this class ensure it's well documented
	*/
	class QSF_API_EXPORT NodesComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::NodesComponent" unique component ID
		static const uint32 NODES;			///< "Nodes" unique class property ID inside the class

		/**
		*  @brief
		*    Debug flags
		*/
		enum DebugFlags
		{
			DEBUG_NODES    = 1<<0,	///< Debug nodes
			DEBUG_NODES_ID = 1<<1,	///< Debug node IDs
			DEBUG_CENTER   = 1<<2,	///< Debug nodes center
			DEBUG_EDGES    = 1<<3,	///< Debug edges
			DEBUG_GATEWAYS = 1<<4,	///< Debug gateways	// TODO(co) Move this into street component
			DEBUG_POLYGON  = 1<<5	///< Debug polygon	// TODO(co) Move this into polygon component
		};
		typedef BitFlagSet<uint8, DebugFlags> DebugFlagSet;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		explicit NodesComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~NodesComponent();

		/**
		*  @brief
		*    Return the debug flag set
		*
		*  @return
		*    The debug flag set, do not destroy the instance
		*/
		inline const DebugFlagSet& getDebugFlagSet() const;

		/**
		*  @brief
		*    Set the debug flag set
		*
		*  @param[in] debugFlagSet
		*    The debug flag set to set
		*/
		inline void setDebugFlagSet(const DebugFlagSet& debugFlagSet);

		/**
		*  @brief
		*    Calculate the center position of the nodes
		*
		*  @return
		*    The local space center position of the nodes
		*/
		glm::vec3 calculateCenterPosition() const;

		/**
		*  @brief
		*    Checks if the given ray is hitting a node
		*
		*  @param[in] ray
		*    Local space ray
		*  @param[out] nodeIndex
		*   The index of the closest node, hitten by the ray
		*
		*  @return
		*    True if an node is hidden, otherwise false
		*/
		bool hitNodeWithRay(const Ray& ray, int& nodeIndex) const;

		/**
		*  @brief
		*    Return the closed segment of the nodes from the given point
		*
		*  @param[in] point
		*    The point from what the calculation checks the closest segment
		*  @param[out] closestPoint
		*    In case the function finds a segment, it returns the closest point on this segment, this can be a null pointer
		*
		*  @return
		*    The index of a node
		*/
		int getClosestSegmentIndexToPoint(const glm::vec3& point, glm::vec3* closestPoint = nullptr) const;

		/**
		*  @brief
		*    Return the number of nodes
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - The only reason this tiny redundant method exists is to have a read only number of nodes CAMP property
		*/
		inline uint32 getNumberOfNodes() const;

		/**
		*  @brief
		*    Creates a visual box (looking like a bounding box) around the given node
		*
		*  @param[in] nodeIndex
		*    Node index
		*  @param[in] color
		*    Color to use
		*  @param[in] boxRadius
		*    Local space box radius
		*  @param[in] debugDrawProxy
		*    Debug draw proxy to write the visualization to, can be a null pointer to use the internal one
		*  @param[in] doInitialization
		*    If "true", the debug draw request will be (re-)registered and its transform set accordingly
		*/
		void createNodeGeometryOnNode(uint32 nodeIndex, const Color4& color, float boxRadius = 0.5f, DebugDrawProxy* debugDrawProxy = nullptr, bool doInitialization = false);


	//[-------------------------------------------------------]
	//[ Interface as required by camp_ext::ArrayMapper        ]
	//[-------------------------------------------------------]
	public:
		// TODO(co) Review & document
		virtual void setNode(std::size_t index, const Node& value);
		virtual void insertNode(std::size_t before, const Node& value);
		virtual void removeNode(std::size_t index);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::NodesComponent methods            ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the minimum number of nodes the concrete nodes component instance needs to be valid
		*
		*  @return
		*    The minimum number of nodes the concrete nodes component instance needs to be valid
		*/
		inline virtual uint32 getMiniumNumberOfNodes() const;

		// TODO(oi) Consolidate this method with setNodeToTransform(), which is more powerful
		virtual void setNodeToPosition(uint32 nodeIndex, const glm::vec3& newPosition);

		/**
		*  @brief
		*    Changes the transform of a node and propagates the property change
		*
		*  @param[in] nodeIndex
		*    Node which transform should be changed
		*  @param[in] newTransform
		*    Local transformation which should be applied to the node
		*/
		virtual void setNodeToTransform(uint32 nodeIndex, const Transform& newTransform);

		/**
		*  @brief
		*    Add a position to all nodes
		*
		*  @param[in] position
		*    The local space position to add
		*/
		virtual void addPosition(const glm::vec3& position);

		inline virtual const std::vector<Node>& getNodes() const;
		virtual void setNodes(const std::vector<Node>& nodes);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::NodesComponent methods         ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Return whether or not closed continuous lines are used
		*
		*  @note
		*    - This information is relevant for e.g. "qsf::NodesComponent::getClosestSegmentIndexToPoint()"
		*    - While a simple path usually does not form a circle (segment from end to start node), a polygon is usually closed
		*/
		virtual bool closedContinuousLines() const = 0;

		/**
		*  @brief
		*    Called if there's a debug flag set change
		*
		*  @note
		*    - This method is only called in case there's a real state change
		*
		*  @see
		*    - "qsf::NodesComponent::setDebugFlags()"
		*/
		virtual void onSetDebugFlagSet();

		/**
		*  @brief
		*    Start debug visualization
		*/
		virtual void startDebugDrawing();

		virtual void onUpdateNodes();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool implementsOnComponentPropertyChange() const override  { return true; }
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;
		virtual void onSetDebug(bool debug) override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void cancelDebugDrawing();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Connected to the CAMP reflection system
		std::vector<Node>	mNodes;					///< The local space nodes
		// Internal only
		DebugFlagSet		mDebugFlagSet;			///< Debug flag set
		DebugDrawProxy		mDebugDrawProxy;		///< Debug draw proxy for visualization


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	QSF_CAMP_ARRAY_PROPERTY(Nodes, NodesComponent, Node)	// Adds public member variable "Nodes" which is the middle part of the nodes <-> CAMP interconnection


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/component/nodes/NodesComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::NodesComponent)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(qsf::NodesComponent::NodesInterface)
