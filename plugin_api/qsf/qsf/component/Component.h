// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/WeakPtr.h"
#include "qsf/math/BitFlagSet.h"
#include "qsf/reflection/object/InheritedObject.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class UserObject;
}
namespace qsf
{
	class Prototype;
	class Entity;
	class ComponentManager;
	class ComponentCollection;
	class Transform;
	class BinarySerializer;
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
	*    Abstract component class
	*
	*  @remarks
	*    Component lifecycle:
	*    - "onStartup()"
	*    - "onShutdown()"
	*
	*    In order to access properties of components, one can use directly the C++ headers. Beside this
	*    native way which requires access to these C++ headers, it's also possible to use the CAMP reflection
	*    system as shown in the following example:
	*    @code
	*    // Component* component = <"qsf::MetadataComponent" instance>
	*    if (nullptr != component)
	*    {
	*        // Set properties
	*        camp::UserObject campUserObject = component;
	*        campUserObject.set("Name", "Alice");
	*        campUserObject.set("Description", "This is the test entity Alice");
	*
	*        // Get property
	*        std::string name = campUserObject.get("Name").to<std::string>();
	*        // ...
	*    }
	*    @endcode
	*    Optionally, one can also write
	*    @code
	*    // Component* component = <"qsf::MetadataComponent" instance>
	*    if (nullptr != component)
	*    {
	*        // Set properties
	*        component->setPropertyValuesByString("Name=\"Alice\" Description=\"This is the test entity Alice\"");
	*
	*        // Get property
	*        std::string name = camp::UserObject(component).get("Name").to<std::string>();
	*        // ...
	*    }
	*    @endcode
	*    or also
	*    @code
	*    // Entity* entity = ...
	*    entity->createComponentByIdWithValues("qsf::MetadataComponent", "Name=\"Alice\" Description=\"This is the test entity Alice\"");
	*    @endcode
	*    in order to set CAMP reflection system properties directly when creating an component instance. In general, by using
	*    the CAMP reflection system approach, there's of course a runtime overhead involved. So, don't use this in highly
	*    performance critical situations.
	*
	*    Each component class connected to the CAMP reflection system can have the following tags:
	*    - "CoreComponent": "true" if this component is considered to be a core component, see "qsf::Map::getCoreEntity()" documentation for details, default is "false"
	*    - "Serializable": Use this to give the system a hint whether or not this component should be serialized (load/save), the default setting is "true"
	*    - "Hidden": Do not show this component within e.g. the editor
	*    - "RootClass": Name of the CAMP class which is the root of the class hierarchy used to register/unregister the component, the default setting is "qsf::Component". Do only set another CAMP class if you know what you're doing.
	*    - See "qsf::ClassSystem"-documentation for more tags
	*
	*  @note
	*    - Keep the constructor/destructor implementation lightweight and do the heavy work within "onStartup()"/"onShutdown()"
	*    - Only components which are part of entities (concrete prototype instances) get started
	*/
	class QSF_API_EXPORT Component : public InheritedObject, public WeakPtrTarget
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class Prototype;				// Calls "qsf::Component::onComponentPropertyChange()"
		friend class Entity;				// Calls "qsf::Component::onComponentPropertyChange()"
		friend class ComponentSystem;		// Component system sets the component ID
		friend class ComponentCollection;	// Component collection sets the collection registration index
		friend class Map;					// Calls "qsf::Component::onSetSimulating()"


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::Component" unique component ID

		enum TransformChangeFlag
		{
			TRANSFORM_CHANGE_POSITION		= 0x01,	///< Position has changed
			TRANSFORM_CHANGE_ROTATION		= 0x02,	///< Rotation has changed
			TRANSFORM_CHANGE_SCALE			= 0x04,	///< Scale has changed
			TRANSFORM_CHANGE_TRANSFORM		= 0x07,	///< Whole transformation has changed (position, rotation and scale)
			TRANSFORM_CHANGE_FOR_SIMULATION = 0x08,	///< The simulation transformation was changed
			TRANSFORM_CHANGE_FOR_ANIMATION  = 0x10	///< The animation transformation was changed
		};
		typedef BitFlagSet<uint8, TransformChangeFlag> TransformChangeFlags;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Do never destroy a component manually, use "qsf::Prototype::destroyComponentById()" instead
		*/
		inline virtual ~Component();

		/**
		*  @brief
		*    Return the prototype this component is in
		*
		*  @return
		*    The prototype this component is in, do no destroy the returned instance
		*/
		inline Prototype& getPrototype() const;

		/**
		*  @brief
		*    Return a reference to the entity this component is in
		*
		*  @return
		*    The entity this component is in, do no destroy the returned instance
		*
		*  @note
		*    - An entity is a concrete prototype instance
		*    - Do only use this method if you know exactly that this component is
		*      started and therefore is part of an entity, there's no internal security check!
		*/
		Entity& getEntity() const;

		/**
		*  @brief
		*    Return the unique component identifier
		*
		*  @return
		*    The unique component identifier
		*
		*  @note
		*    - The unique component identifier is generated by "qsf::StringHash" by using the class name
		*      (e.g. qsf::StringHash("qsf::MetadataComponent") generating the hash value 483975945)
		*    - Connected to the CAMP reflection system
		*/
		inline uint32 getId() const;

		/**
		*  @brief
		*    Return the component manager that's responsible for this component instance
		*
		*  @return
		*    The component manager; do not destroy the returned instance
		*/
		ComponentManager& getComponentManager() const;

		/**
		*  @brief
		*    Return the component collection that's responsible for this component instance
		*
		*  @return
		*    The component collection; do not destroy the returned instance
		*
		*  @remarks
		*    Do not call this for prototype components, for only entity components can be registered within a component collection
		*/
		ComponentCollection& getComponentCollection() const;

		/**
		*  @brief
		*    Return whether or not the component is in simulating mode
		*
		*  @return
		*    "true" if the component is in simulating mode, else "false"
		*
		*  @note
		*    - This is an ease-of-use method that returns the associated map's simulating mode state
		*    - In case there is no map, i.e. for prototype components, this will always return "false"
		*/
		inline bool isSimulating() const;

		/**
		*  @brief
		*    Return whether or not the component is considered to be active
		*
		*  @return
		*    "true" if the component is considered to be active, else "false"
		*
		*  @note
		*    - By default, components are considered to be active
		*    - Not connected to the CAMP reflection system by intent, this only makes sense for concrete components
		*/
		inline bool isActive() const;

		/**
		*  @brief
		*    Set whether or not the component is considered to be active
		*
		*  @param[in] active
		*    "true" if the component is considered to be active, else "false"
		*
		*  @remarks
		*    The semantic of "active" depends of the component. An implementation is free to ignore the request or do nothing
		*    at all because an active state makes no sense for the component. A render component might for example interpret
		*    "active" as "invisible" while an input component might interpret "active" as "deactivate input".
		*
		*  @note
		*    - Not connected to the CAMP reflection system by intent, this only makes sense for concrete components
		*/
		void setActive(bool active);

		/**
		*  @brief
		*    Return whether or not the component is considered to be in debug mode
		*
		*  @return
		*    "true" if the component is considered to be in debug mode, else "false"
		*
		*  @note
		*    - Not connected to the CAMP reflection system by intent, this should be internal only
		*/
		inline bool isDebug() const;

		/**
		*  @brief
		*    Return whether or not the component is hidden
		*
		*  @return
		*    "true" if the component is hidden, else "false"
		*
		*  @remarks
		*    "Hidden" is a passive special component feature. While the content of hidden components
		*    is identical to the content of not hidden components, the content of hidden components
		*    will not be visible inside the render viewport. This special property will
		*    not be serialized and e.g. inside the QSF editor will have no influence on the
		*    component hidden state of other QSF editor users.
		*
		*  @note
		*    - Not connected to the CAMP reflection system by intent, this must be internal only
		*/
		inline bool isHidden() const;

		/**
		*  @brief
		*    Set whether or not the component is hidden
		*
		*  @param[in] hidden
		*   "true" if the component is hidden, else "false"
		*
		*  @see
		*    - "qsf::Component::isHidden()"
		*/
		void setHidden(bool hidden);

		/**
		*  @brief
		*    Return whether or not the component is globally hidden
		*
		*  @return
		*    "true" if the component is globally hidden, else "false"
		*
		*  @remarks
		*    While a component is not hidden itself, it might be hidden as an end result because
		*    the layer the owner entity of the component is in is hidden.
		*
		*  @note
		*    - Not connected to the CAMP reflection system by intent, this must be internal only
		*/
		inline bool isGloballyHidden() const;

		/**
		*  @brief
		*    Set whether or not the component is globally hidden
		*
		*  @param[in] globallyHidden
		*   "true" if the component is globally hidden, else "false"
		*
		*  @see
		*    - "qsf::Component::isGloballyHidden()"
		*
		*  @note
		*    - This method is usually only used by layers
		*/
		void setGloballyHidden(bool globallyHidden);

		/**
		*  @brief
		*    Return whether or not the component is really hidden
		*
		*  @return
		*    "true" if the component is really hidden, else "false"
		*
		*  @remarks
		*    While a component is not hidden itself, it might be hidden as an end result because
		*    the layer the owner entity of the component is in is hidden. This method returns the
		*    final hidden state after taking everything into account.
		*
		*  @note
		*    - Not connected to the CAMP reflection system by intent, this must be internal only
		*/
		inline bool isReallyHidden() const;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the component
		*
		*  @param[in] ownerIsRunning
		*   "true" if the owner is running, else "false"
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - Only components which are part of entities (concrete prototype instances) get started
		*/
		bool startup(bool ownerIsRunning);

		/**
		*  @brief
		*    Return whether or not the component is up-and-running
		*
		*  @return
		*    "true" if the component is up-and-running ("onStartup()" was called successfully), else "false"
		*/
		inline bool isRunning() const;

		/**
		*  @brief
		*    Shutdown the component
		*/
		void shutdown();

		//[-------------------------------------------------------]
		//[ Ease of use methods                                   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the ID of the prototype this component is in
		*
		*  @return
		*    The ID of the prototype this component is in
		*
		*  @note
		*    - A shortcut for "getPrototype().getId()" that avoids including the prototype header just for using its ID
		*/
		uint64 getPrototypeId() const;

		/**
		*  @brief
		*    Return the ID of the entity this component is in
		*
		*  @return
		*    The ID of the entity this component is in
		*
		*  @note
		*    - An entity is a concrete prototype instance
		*    - Do only use this method if you know exactly that this component is
		*      started and therefore is part of an entity, there's no internal security check!
		*    - A shortcut for "getEntity().getId()" that avoids including the entity header just for using its ID
		*/
		uint64 getEntityId() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Invalidate the component
		*
		*  @note
		*    - Called after the component was removed from its prototype, but is not yet destroyed
		*/
		inline virtual void onInvalidation();

		/**
		*  @brief
		*    Set whether or not the component is considered to be in debug mode
		*
		*  @param[in] debugMode
		*    "true" if the component is considered to be in debug mode, else "false"
		*
		*  @remarks
		*    The semantic of "debug mode" depends of the component. An implementation is free to ignore the request or do nothing
		*    at all because a debug mode state makes no sense for the component. A polygon component might for example interpret
		*    "debug mode" as "render the polygons" while an input component might interpret "debug mode" as "draw some texts".
		*
		*  @note
		*    - Not connected to the CAMP reflection system by intent, this should be internal only
		*/
		virtual void setDebug(bool debugMode);

		//[-------------------------------------------------------]
		//[ Serialization                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Serialize or deserialize the component using a binary serializer
		*
		*  @param[in] serializer
		*    The serializer, which can be either in read or write mode
		*/
		virtual void serialize(BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
		*
		*  @note
		*    - Use "qsf::Prototype::createComponentById()" to create a prototype component instance
		*/
		explicit Component(Prototype* prototype);

		/**
		*  @brief
		*    A component property value was changed
		*
		*  @param[in] propertyId
		*    Unique class property ID the component property which values was changed
		*
		*  @note
		*    - The unique class property identifier is generated by "qsf::StringHash" by using the class property name
		*      (e.g. qsf::StringHash("Position") generating the hash value 3091144222)
		*/
		void promotePropertyChange(uint32 propertyId);

		/**
		*  @brief
		*    Comfort function for updating an exported member variable and promoting any change that happened
		*
		*  @return
		*    "true" the value actually changed, "false" if the same value was already set
		*
		*  @remarks
		*    If variable is equal to newValue, nothing happens.
		*    Otherwise variable is assigned newValue and a property change for propertyId is promoted.
		*/
		template <typename T, typename S>
		bool assignAndPromoteChange(T& variable, const S& newValue, uint32 propertyId);

		/**
		*  @brief
		*    Register or unregister this component as simulation transform listener
		*
		*  @param[in] enable
		*    If "true", register, otherwise unregister
		*
		*  @note
		*    - As this registration is only necessary in simulation mode, best call it inside "onSetSimulating"
		*/
		void setRegisterForSimulationTransformChanges(bool enable);

		/**
		*  @brief
		*    Register or unregister this component as animation transform listener
		*
		*  @param[in] enable
		*    If "true", register, otherwise unregister
		*
		*  @note
		*    - As this registration is only necessary in simulation mode, best call it inside "onSetSimulating"
		*/
		void setRegisterForAnimationTransformChanges(bool enable);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::InheritedObject methods        ]
	//[-------------------------------------------------------]
	protected:
		virtual const Object* getPropertyDefaultsProvider() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Return whether or not the component will only startup if the owner is in running state
		*
		*  @return
		*    If "false", this component will only startup if the owner is in running state, else the component will always startup
		*/
		inline virtual bool ignoreOwnerRunningState() const;

		/**
		*  @brief
		*    Signal whether this component needs the "onComponentPropertyChange" callback at all
		*
		*  @return
		*    If "true", this component can receive property change callbacks, otherwise "onComponentPropertyChange" won't get called
		*/
		inline virtual bool implementsOnComponentPropertyChange() const;

		/**
		*  @brief
		*    Called in case a component property value was changed
		*
		*  @param[in] component
		*    Component instance which had a property value change
		*  @param[in] propertyId
		*    Unique class property ID the component property which values was changed
		*
		*  @note
		*    - The default implementation is empty
		*    - The unique class property identifier is generated by "qsf::StringHash" by using the class property name
		*      (e.g. qsf::StringHash("Position") generating the hash value 3091144222)
		*/
		inline virtual void onComponentPropertyChange(const Component& component, uint32 propertyId);

		/**
		*  @brief
		*    Called in case a component transformation value was changed
		*
		*  @param[in] transform
		*    The changed transformation
		*  @param[in] flags
		*    Flags describing the change
		*
		*  @note
		*    - The default implementation is empty
		*    - In order for this to get called at all in simulating mode, an entity has to register itself as listener for
		*      simulation and/or animation transform changes, see e.g. "qsf::Component::setRegisterForSimulationTransformChanges".
		*/
		inline virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags);

		/**
		*  @brief
		*    Set whether or not the component is in simulating mode
		*
		*  @param[in] active
		*    "true" if the component is in simulating mode, else "false"
		*
		*  @note
		*    - The default implementation is empty
		*    - This method is only called in case there's a real state change
		*    - In contrast to internal flags (e.g. Active, Debug), the simulating mode state can only be changed globally for a whole map
		*    - On component shutdown and simulating enabled, "onSetSimulating()" is automatically called with "false" before calling "onShutdown()"
		*/
		inline virtual void onSetSimulating(bool simulating);

		/**
		*  @brief
		*    Set whether or not the component is considered to be active
		*
		*  @param[in] active
		*    "true" if the component is considered to be active, else "false"
		*
		*  @note
		*    - The default implementation is empty
		*    - This method is only called in case there's a real state change
		*    - This method is also called in case the component is currently not running
		*
		*  @see
		*    - "qsf::Component::setActive()"
		*/
		inline virtual void onSetActive(bool active);

		/**
		*  @brief
		*    Set whether or not the component is considered to be in debug mode
		*
		*  @param[in] debug
		*    "true" if the component is considered to be in debug mode, else "false"
		*
		*  @note
		*    - The default implementation is empty
		*    - This method is only called in case there's a real state change
		*
		*  @see
		*    - "qsf::Component::setDebug()"
		*/
		inline virtual void onSetDebug(bool debug);

		/**
		*  @brief
		*    Set whether or not the component is considered to be hidden
		*
		*  @param[in] hidden
		*    "true" if the component is considered to be hidden, else "false"
		*
		*  @note
		*    - The default implementation is empty
		*    - This method is only called in case there's a real state change
		*
		*  @see
		*    - "qsf::Component::setHidden()"
		*/
		inline virtual void onSetHidden(bool hidden);

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the component
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - The default implementation is empty
		*    - Only components which are part of entities (concrete prototype instances) get started
		*    - When this method gets called, the properties might still be on default values but it's also
		*      possible that properties already have customized values. Take this into account when
		*      implementing your method.
		*/
		inline virtual bool onStartup();

		/**
		*  @brief
		*    Shutdown the component
		*
		*  @note
		*    - Only called in case "onStartup()" returned successfully
		*    - The default implementation is empty
		*/
		inline virtual void onShutdown();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Component flags
		*/
		enum Flags
		{
			ACTIVE			= 1<<0,	///< This flag is set by default and indicates that the component is active (the semantic of "active" depends of the component, see "qsf::Component::setActive()" for details)
			DEBUG			= 1<<1,	///< This flag indicates that the component is in debug mode (the semantic of "debug" depends of the component, see "qsf::Component::setDebug()" for details)
			HIDDEN			= 1<<2,	///< This flag indicates that the component is hidden, see "qsf::Component::setHidden()" for details
			GLOBALLY_HIDDEN = 1<<3,	///< This flag indicates that the component is globally hidden, see "qsf::Component::setGloballyHidden()" for details
			REALLY_HIDDEN   = 1<<4,	///< This flag indicates that the component is really hidden, see "qsf::Component::setHidden()" and "qsf::Component::setGloballyHidden()" for details
			RUNNING			= 1<<5, ///< This flag indicates that the component is up-and-running ("onStartup()" was called successfully, already set inside "onStartup()")
			SIMULATING		= 1<<6, ///< This flag indicates that the component is simulating
			REGISTERED_FOR_SIMULATION_TRANSFORM = 1<<7,
			REGISTERED_FOR_ANIMATION_TRANSFORM  = 1<<8
		};
		typedef BitFlagSet<uint16, Flags> FlagSet;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Update the really hidden flag
		*/
		void reallyHidden();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Prototype*	mPrototype;		///< The prototype this component is in, always valid
		uint32		mId;			///< The unique component identifier, set by the prototype
		FlagSet		mFlags;			///< Internal flags, see "qsf::Component::Flags"
		uint32		mCollectionRegistrationIndex;	///< Index of registration in component collection


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
#include "qsf/component/Component-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::Component)
