// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/nodes/NodesComponent.h"
#include "qsf/component/street/StreetGateway.h"


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
	*    Abstract superclass for all components that make up the generic QSF street network system.
	*
	*  @note
	*    - In the street component base class, the street gateways are no property by intent (e.g. street crossing and street section have different requirements)
	*/
	class QSF_API_EXPORT StreetComponent : public NodesComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;			///< "qsf::StreetComponent" unique component ID
		static const uint32 STREET_GATEWAYS;		///< "StreetGateways" unique class property ID inside the class
		static const uint32 AI_NAVIGATION_MAP_ID;	///< "AiNavigationMapId" unique class property ID inside the class


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
		inline explicit StreetComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~StreetComponent();

		//[-------------------------------------------------------]
		//[ Gateways                                              ]
		//[-------------------------------------------------------]
		// TODO(co) Document those
		inline const std::vector<StreetGateway>& getStreetGateways() const;

		/**
		*  @brief
		*    Get position of a street gateway with specified index in world space
		*
		*  @param[in] streetGatewayIndex
		*    Street gateway index
		*
		*  @return
		*    World space position of street gateway
		*/
		glm::vec3 getStreetGatewayPositionInWorldSpace(uint32 streetGatewayIndex) const;

		/**
		*  @brief
		*    Get direction of a street gateway with specified index in world space
		*
		*  @param[in] streetGatewayIndex
		*    Gateway index
		*
		*  @return
		*    World space direction of street gateway
		*/
		glm::vec3 getStreetGatewayDirectionInWorldSpace(uint32 streetGatewayIndex) const;

		/**
		*  @brief
		*    Get street street gateway with specified index
		*
		*  @param[in] streetGatewayIndex
		*    Index of street gateway
		*
		*  @return
		*    Street gateway, can be a null pointer, do not destroy the instance
		*/
		inline StreetGateway* getStreetGatewayByIndex(uint32 streetGatewayIndex);
		inline const StreetGateway* getStreetGatewayByIndex(size_t streetGatewayIndex) const;

		/**
		*  @brief
		*    Return the ID of the AI navigation map that should use this data as input
		*
		*  @return
		*    The ID of the AI navigation map that should use this data as input
		*/
		inline uint32 getAiNavigationMapId() const;

		/**
		*  @brief
		*    Set the ID of the AI navigation map that should use this data as input
		*/
		inline void setAiNavigationMapId(uint32 id);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::StreetComponent methods           ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get position of a street gateway with specified index
		*
		*  @param[in] streetGatewayIndex
		*    Street gateway index
		*
		*  @return
		*    Local space position of gateway
		*/
		virtual glm::vec3 getStreetGatewayPosition(uint32 streetGatewayIndex) const = 0;

		/**
		*  @brief
		*    Get direction of a street gateway with specified index
		*
		*  @param[in] streetGatewayIndex
		*    Street gateway index
		*
		*  @return
		*    Local space direction of street gateway
		*/
		virtual glm::vec3 getStreetGatewayDirection(uint32 streetGatewayIndex) const = 0;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::NodesComponent methods         ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool closedContinuousLines() const override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		std::vector<StreetGateway> mStreetGateways;		///< Contains all street gateways of the street component, this list must keep free of duplicates
		uint32					   mAiNavigationMapId;	///< Used as a selector which AI navigation map uses this street component as input


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	QSF_CAMP_ARRAY_PROPERTY(StreetGateways, StreetComponent, StreetGateway)	// Adds public member variable "StreetGateways" which is the middle part of the nodes <-> CAMP interconnection


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/component/street/StreetComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::StreetComponent)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(qsf::StreetComponent::StreetGatewaysInterface)
