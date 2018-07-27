// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/operation/OperationHelper.h"

#include <boost/property_tree/ptree_fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Component;
	class Entity;
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
		*    Static component operation helper class
		*/
		class QSF_EDITOR_API_EXPORT ComponentOperationHelper : public OperationHelper
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Build an operation that sets an component array property
			*
			*  @param[in] component
			*    Reference to the component the changed property is in
			*  @param[in] propertyId
			*    Unique (inside the entity component) class property ID the CAMP property to manipulate
			*  @param[in] newValue
			*    New value to set
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			template <typename T>
			static base::Operation* buildSetArrayPropertyOperation(const Component& component, uint32 propertyId, const T& newValue);

			/**
			*  @brief
			*    Build an operation that sets an component array property
			*
			*  @param[in] component
			*    Reference to the component the changed property is in
			*  @param[in] propertyId
			*    Unique (inside the entity component) class property ID the CAMP property to manipulate
			*  @param[in] newValue
			*    New value to set
			*  @param[in] previousValue
			*    Previews value
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			template <typename T>
			static base::Operation* buildSetArrayPropertyOperation(const Component& component, uint32 propertyId, const T& newValue, const T& previousValue);

			/**
			*  @brief
			*    Build an operation that sets an component array property
			*
			*  @param[in] entityId
			*    The unique entity identifier
			*  @param[in] componentId
			*    The unique (inside the entity) component identifier
			*  @param[in] propertyId
			*    Unique (inside the entity component) class property ID the CAMP property to manipulate
			*  @param[in] newValue
			*    New value to set
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			template <typename T>
			static base::Operation* buildSetArrayPropertyOperation(uint64 entityId, uint32 componentId, uint32 propertyId, const T& newValue);

			/**
			*  @brief
			*    Build an operation that sets an component array property
			*
			*  @param[in] entityId
			*    The unique entity identifier
			*  @param[in] componentId
			*    The unique (inside the entity) component identifier
			*  @param[in] propertyId
			*    Unique (inside the entity component) class property ID the CAMP property to manipulate
			*  @param[in] newValue
			*    New value to set
			*  @param[in] previousValue
			*    Previews value
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			template <typename T>
			static base::Operation* buildSetArrayPropertyOperation(uint64 entityId, uint32 componentId, uint32 propertyId, const T& newValue, const T& previousValue);

			/**
			*  @brief
			*    Build an operation that creates a component if necessary and sets component properties by using a given Boost ptree
			*
			*  @param[in] entityId
			*    The unique entity identifier
			*  @param[in] componentId
			*    The unique (inside the entity) component identifier
			*  @param[in] pTree
			*    The component data to transform into an operation
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildComponentByBoostPTreeOperation(const Entity& entity, uint32 componentId, const boost::property_tree::ptree& pTree);

			/**
			*  @brief
			*    Build an operation that morphs a component from one class type to another
			*
			*  @param[in] entityId
			*    The unique entity identifier
			*  @param[in] componentId
			*    The unique (inside the entity) component identifier of the component to morph (e.g. "qsf::MeshComponent::COMPONENT_ID")
			*  @param[in] morphToComponentId
			*    The ID of the component to morph to (e.g. "qsf::compositing::TintableMeshComponent::COMPONENT_ID")
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildMorphComponentOperation(uint64 entityId, uint32 componentId, uint32 morphToComponentId);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/operation/component/ComponentOperationHelper-inl.h"
