// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/StringHash.h"	// No forward declaration by intent
#include "qsf/component/ComponentCollection.h"
#include "qsf/map/query/MapQuery.h"

#include <glm/fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class Class;
}
namespace qsf
{
	class Component;
	class Transform;
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
	*    Component map query interface
	*
	*  @remarks
	*    Usage example:
	*    @code
	*    qsf::Component* qsfComponent = qsf::ComponentMapQuery(QSF_MAINMAP).getFirstInstanceByClass("qsf::CameraComponent");
	*    @endcode
	*/
	class QSF_API_EXPORT ComponentMapQuery : public MapQuery
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] map
		*    Map instance to perform the map query on, the instance must stay valid as long as this map query instance exists
		*/
		inline explicit ComponentMapQuery(const Map& map);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~ComponentMapQuery();

		//[-------------------------------------------------------]
		//[ First instance                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the first component instance of a certain type found in any entity
		*
		*  @return
		*    The requested component instance, null pointer if no instance exists in map, do not destroy the returned instance, do not keep a reference to it outside the C runtime stack (use the unique component ID instead)
		*/
		template <typename T>
		T* getFirstInstance() const;

		/**
		*  @brief
		*    Return the first component instance by using its unique identifier found in any entity
		*
		*  @param[in] id
		*    The unique component identifier, which is generated from the component name by "qsf::StringHash"
		*
		*  @return
		*    The requested component instance, null pointer if no instance exists in map, do not destroy the returned instance, do not keep a reference to it outside the C runtime stack (use the unique component ID instead)
		*/
		Component* getFirstInstanceById(const StringHash& id) const;

		/**
		*  @brief
		*    Return the first component instance by using a given CAMP reflection class found in any entity
		*
		*  @param[in] campClass
		*    CAMP reflection class of the component to instantiate
		*
		*  @return
		*    The requested component instance, null pointer if no instance exists in map, do not destroy the returned instance, do not keep a reference to it outside the C runtime stack (use the unique component ID instead)
		*
		*  @note
		*    - Less efficient as e.g. qsf::ComponentMapQuery::getFirstInstanceById()"
		*/
		Component* getFirstInstanceByCampClass(const camp::Class& campClass) const;

		//[-------------------------------------------------------]
		//[ All instances                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return all component instances of a certain type found in any entity
		*
		*  @return
		*    The list of component instances, do not destroy the returned instances, do not keep a reference to it outside the C runtime stack (use the unique component ID instead)
		*/
		template <typename T>
		const ComponentCollection::ComponentList<T>& getAllInstances() const;

		/**
		*  @brief
		*    Return all component instances by using its unique identifier found in any entity
		*
		*  @param[in] id
		*    The unique component identifier, which is generated from the component name by "qsf::StringHash"
		*
		*  @return
		*    The list of component instances, do not destroy the returned instances, do not keep a reference to it outside the C runtime stack (use the unique component ID instead)
		*/
		const ComponentCollection::ComponentList<Component>& getAllInstancesById(const StringHash& id) const;

		/**
		*  @brief
		*    Return all component instances by using a given CAMP reflection class found in any entity
		*
		*  @param[in]  campClass
		*    CAMP reflection class of the component to instantiate
		*
		*  @return
		*    The list of component instances, do not destroy the returned instances, do not keep a reference to it outside the C runtime stack (use the unique component ID instead)
		*
		*  @note
		*    - Less efficient as e.g. qsf::ComponentMapQuery::getFirstInstanceById()"
		*/
		const ComponentCollection::ComponentList<Component>& getAllInstancesByCampClass(const camp::Class& campClass) const;

		//[-------------------------------------------------------]
		//[ Instances in an area                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return all component instances of a certain type found inside the given circle in the xz-plane
		*
		*  @param[in] center
		*    World space center of the circle
		*  @param[in] radius
		*    World space radius of the circle
		*  @param[out] outComponents
		*    Receives the found component instances, do not destroy the instances, list is not cleared before new entries are added
		*
		*  @return
		*    "true" if at least one component was found, "false" if none was found
		*/
		template <typename T>
		bool getInstancesInCircle(const glm::vec3& center, float radius, std::vector<T*>& outComponents) const;

		/**
		*  @brief
		*    Return all component instances of a certain type found inside the given circle in the xz-plane
		*
		*  @param[in] id
		*    The unique component identifier, which is generated from the component name by "qsf::StringHash"
		*  @param[in] center
		*    World space center of the circle
		*  @param[in] radius
		*    World space radius of the circle
		*  @param[out] outComponents
		*    Receives the found component instances, do not destroy the instances, list is not cleared before new entries are added
		*
		*  @return
		*    "true" if at least one component was found, "false" if none was found
		*/
		bool getInstancesInCircleById(const StringHash& id, const glm::vec3& center, float radius, std::vector<Component*>& outComponents) const;

		/**
		*  @brief
		*    Return all component instances of a certain type found inside the given box
		*
		*  @param[in] boxTransform
		*    World space transformation of the box, e.g. a debug box
		*  @param[in] yAxisCentered
		*    If "true", the box transform's position is regarded as box center, otherwise this is the box's bottom's center
		*  @param[out] outComponents
		*    Receives the found component instances, do not destroy the instances, list is not cleared before new entries are added
		*
		*  @return
		*    "true" if at least one component was found, "false" if none was found
		*
		*  @note
		*    - The box is regarded as spanning from x,y,z = [-0.5, 0.5] in the transform's local coordinate system, at least if yAxisCentered is set
		*    - If yAxisCentered is "false", the local coordinate system y-range is [0.0, 1.0] instead
		*/
		template <typename T>
		bool getInstancesInBox(const Transform& boxTransform, bool yAxisCentered, std::vector<T*>& outComponents) const;

		/**
		*  @brief
		*    Return all component instances of a certain type found inside the given box
		*
		*  @param[in] id
		*    The unique component identifier, which is generated from the component name by "qsf::StringHash"
		*  @param[in] boxTransform
		*    World space transformation of the box, e.g. a debug box
		*  @param[in] yAxisCentered
		*    If "true", the box transform's position is regarded as box center, otherwise this is the box's bottom's center
		*  @param[out] outComponents
		*    Receives the found component instances, do not destroy the instances, list is not cleared before new entries are added
		*
		*  @return
		*    "true" if at least one component was found, "false" if none was found
		*
		*  @note
		*    - The box is regarded as spanning from x,y,z = [-0.5, 0.5] in the transform's local coordinate system, at least if yAxisCentered is set
		*    - If yAxisCentered is "false", the local coordinate system y-range is [0.0, 1.0] instead
		*/
		bool getInstancesInBoxById(const StringHash& id, const Transform& boxTransform, bool yAxisCentered, std::vector<Component*>& outComponents) const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/query/ComponentMapQuery-inl.h"
