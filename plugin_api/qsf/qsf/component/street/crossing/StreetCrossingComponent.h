// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/street/StreetComponent.h"
#include "qsf/component/street/crossing/StreetEdge.h"


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
	*    Street crossing component class
	*/
	class QSF_API_EXPORT StreetCrossingComponent : public StreetComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::StreetCrossingComponent" unique component ID
		static const uint32 EDGES;			///< "Edges" unique class property ID inside the class


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
		explicit StreetCrossingComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~StreetCrossingComponent();

		//[-------------------------------------------------------]
		//[ Edges                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the number of edges
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - The only reason this tiny redundant method exists is to have a read only number of edges CAMP property
		*/
		inline uint32 getNumberOfStreetEdges() const;

		// TODO(co) Document those
		inline const std::vector<StreetEdge>& getEdges() const;
		void setEdges(const std::vector<StreetEdge>& edges);

		/**
		*  @brief
		*    Get the Id of the bridge of this Street Crossing, if the street crossing is bridge connection (uninitialized otherwise)
		*
		*  @return
		*    ID of this bridge
		*/
		inline uint32 getBridgeId() const;

		/**
		*  @brief
		*    Set the Id of the bridge of this Street Crossing, if the street crossing is bridge connection (uninitialized otherwise)
		*
		*  @param[in] bridgeId
		*    ID of the bridge
		*/
		inline void setBridgeId(uint32 bridgeId);


		//[-------------------------------------------------------]
		//[ Gateways                                              ]
		//[-------------------------------------------------------]
		// TODO(co) Document those
		inline void setStreetGateways(const std::vector<StreetGateway>& streetGateways);

		inline void repaintDebugDrawing();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::StreetComponent methods           ]
	//[-------------------------------------------------------]
	public:
		virtual glm::vec3 getStreetGatewayPosition(uint32 streetGatewayIndex) const override;
		virtual glm::vec3 getStreetGatewayDirection(uint32 streetGatewayIndex) const override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(BinarySerializer& serializer) override;


	//[----------------------------------------------------------------]
	//[ Interface as required by NodesComponent::camp_ext::ArrayMapper ]
	//[----------------------------------------------------------------]
	public:
		/**
		*  @brief
		*   TODO(mk) check if edges data is valid
		*/
		virtual void setNodes(const std::vector<Node>& nodes) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool implementsOnComponentPropertyChange() const override { return true; }
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::NodesComponent methods         ]
	//[-------------------------------------------------------]
	protected:
		virtual void startDebugDrawing() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		std::vector<StreetEdge> mStreetEdges;		///< Contains all edges of the street, this list must keep free of duplicates

		uint32 mBridgeId;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	QSF_CAMP_ARRAY_PROPERTY(Edges, StreetCrossingComponent, StreetEdge)	// Adds public member variable "Edges" which is the middle part of the nodes <-> CAMP interconnection


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/component/street/crossing/StreetCrossingComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::StreetCrossingComponent)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(qsf::StreetCrossingComponent::EdgesInterface)
