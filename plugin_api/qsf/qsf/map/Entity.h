// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/prototype/Prototype.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class Transform;
	class TransformComponent;
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
	*    Entity class
	*
	*  @note
	*    - An entity is a concrete prototype instance
	*    - Everything within a map is an entity
	*    - "Objects" are formed by linking entities together
	*/
	class QSF_API_EXPORT Entity : public Prototype
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Map;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Do never destroy an entity manually, use "qsf::Map::destroyEntityById()" instead
		*/
		inline virtual ~Entity();

		/**
		*  @brief
		*    Return the map this entity is in
		*
		*  @return
		*    The map this entity is in, do no destroy the returned instance
		*/
		Map& getMap() const;

		//[-------------------------------------------------------]
		//[ Serialization                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return whether or not the entity is serializable
		*
		*  @return
		*    "true" if the entity is serializable, else "false"
		*
		*  @note
		*    - Not connected to the CAMP reflection system by intent, this must be internal only
		*/
		inline bool isSerializable() const;

		/**
		*  @brief
		*    Set whether or not the entity is serializable
		*
		*  @param[in] serializable
		*   "true" if the entity is serializable, else "false"
		*
		*  @see
		*    - "qsf::Entity::isSerializable()"
		*/
		inline void setSerializable(bool serializable);

		//[-------------------------------------------------------]
		//[ Component state (ease-of-use)                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set whether or not the entity is considered to be active
		*
		*  @param[in] active
		*    "true" if the entity is considered to be active, else "false"
		*
		*  @note
		*    - Calls "qsf::Component::setActive()" of each component
		*    - A "qsf::Entity::isActive()" is not possible because each component can have another active state
		*/
		void setActive(bool active) const;

		/**
		*  @brief
		*    Set whether or not the entity is considered to be in debug mode
		*
		*  @param[in] debug
		*    "true" if the entity is considered to be in debug mode, else "false"
		*
		*  @note
		*    - Calls "qsf::Component::setDebug()" of each component
		*    - A "qsf::Entity::isDebug()" is not possible because each component can have another debug mode state
		*/
		void setDebug(bool debug) const;

		/**
		*  @brief
		*    Set whether or not the entity is considered to be hidden
		*
		*  @param[in] hidden
		*    "true" if the entity is considered to be hidden, else "false"
		*
		*  @note
		*    - Calls "qsf::Component::setHidden()" of each component
		*    - A "qsf::Entity::isHidden()" is not possible because each component can have another hidden state
		*/
		void setHidden(bool hidden) const;

		/**
		*  @brief
		*    Set whether or not the entity is considered to be globally hidden
		*
		*  @param[in] globallyHidden
		*    "true" if the entity is considered to be globally hidden, else "false"
		*
		*  @note
		*    - Calls "qsf::Component::setGloballyHidden()" of each component
		*    - A "qsf::Entity::isGloballyHidden()" is not possible because each component can have another hidden state
		*/
		void setGloballyHidden(bool globallyHidden) const;

		//[-------------------------------------------------------]
		//[ Component lifecycle (ease-of-use)                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Loop through all components of the entity and startup them
		*
		*  @param[in] ownerIsRunning
		*   "true" if the owner is running, else "false"
		*
		*  @note
		*    - Do only use this method when you know what you're doing
		*    - Component startup failures are silently ignored
		*/
		void startup(bool ownerIsRunning);

		/**
		*  @brief
		*    Loop through all components of the entity and shutdown them
		*
		*  @note
		*    - Do only use this method when you know what you're doing
		*/
		void shutdown();

		//[-------------------------------------------------------]
		//[ Component transform change listeners                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Check if there are any simulation transform listeners
		*
		*  @return
		*    "true" if there's at least one listener for simulation transformation changes
		*/
		inline bool hasSimulationTransformListeners() const;

		/**
		*  @brief
		*    Register a component as simulation transform listener
		*
		*  @param[in] component
		*    The component to register
		*
		*  @remarks
		*    Registered listeners receive a call to "qsf::Component::onComponentTransformChange()" whenever the
		*    entity's simulation transformation changes.
		*    However, registering is only important in simulating mode (see "qsf::Component::onSetSimulating()"),
		*    for otherwise all property change listeners will receive "onComponentTransformChange" callbacks as well.
		*/
		void registerComponentForSimulationTransformChanges(Component& component);

		/**
		*  @brief
		*    Unregister a component as simulation transform listener
		*
		*  @param[in] component
		*    The component to unregister
		*/
		void unregisterComponentForSimulationTransformChanges(Component& component);

		/**
		*  @brief
		*    Check if there are any animation transform listeners
		*
		*  @return
		*    "true" if there's at least one listener for animation transformation changes
		*/
		inline bool hasAnimationTransformListeners() const;

		/**
		*  @brief
		*    Register a component as animation transform listener
		*
		*  @param[in] component
		*    The component to register
		*
		*  @remarks
		*    Registered listeners receive a call to "qsf::Component::onComponentTransformChange()" whenever the
		*    entity's animation transformation changes.
		*    However, registering is only important in simulating mode (see "qsf::Component::onSetSimulating()"),
		*    for otherwise all property change listeners will receive "onComponentTransformChange" callbacks as well.
		*/
		void registerComponentForAnimationTransformChanges(Component& component);

		/**
		*  @brief
		*    Unregister a component as animation transform listener
		*
		*  @param[in] component
		*    The component to unregister
		*/
		void unregisterComponentForAnimationTransformChanges(Component& component);

		/**
		*  @brief
		*    React to component transform changes
		*
		*  @param[in] transformComponent
		*    The transform component holding the changes transformation
		*  @param[in] transform
		*    The changed transform
		*  @param[in] flags
		*    Flags describing the change, see "qsf::Component::TransformChangeFlag"
		*/
		void onComponentTransformChange(TransformComponent& transformComponent, const Transform& transform, uint8 flags);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] id
		*    The unique entity identifier
		*  @param[in] map
		*    The map this entity is in
		*
		*  @note
		*    - Use "qsf::Map::createEntity()" to create an entity instance
		*/
		Entity(uint64 id, Map& map);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ComponentList mSimulationTransformListeners;	///< List of components that can be listeners to transform component property changes
		ComponentList mAnimationTransformListeners;		///< List of components that can be listeners to transform component property changes


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/Entity-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::Entity)
