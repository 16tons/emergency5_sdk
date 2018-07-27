// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/math/Transform.h"
#include "qsf/math/Color4.h"
#include "qsf/reflection/type/CampUint64.h"
#include "qsf/debug/DebugDrawProxy.h"

#include <boost/signals2.hpp>
#include <boost/container/flat_set.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class TransformComponent;
	class MovableComponent;
	class CameraComponent;
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
	*    Link component class
	*
	*  @note
	*    - Currently linked children get scaled with their parent. This may be changed (or optional) in the future.
	*       This would require changes or creating alternative versions of Transform class methods createLocalTransform() and fromLocalTransform().
	*    - "Objects" are formed by linking entities together
	*    - "Prefabs" are formed by linking prototypes together
	*/
	class QSF_API_EXPORT LinkComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "qsf::LinkComponent" unique component ID
		static const uint32 PARENT_LINK_TYPE;	///< "ParentLinkType" unique class property ID inside the class
		static const uint32 PARENT_ID;			///< "ParentId" unique class property ID inside the class
		static const uint32 LOCAL_POSITION;		///< "LocalPosition" unique class property ID inside the class
		static const uint32 LOCAL_ROTATION;		///< "LocalRotation" unique class property ID inside the class
		static const uint32 LOCAL_SCALE;		///< "LocalScale" unique class property ID inside the class

		/**
		*  @brief
		*    Link type
		*/
		enum LinkType
		{
			INVALID_LINK_TYPE = -1,	///< Invalid value; can e.g. stand for no link at all
			HARD_LINK  = 0,			///< An entity will destroy other entities linked via hard link; green visualisation
			SOFT_LINK  = 1,			///< An entity will not destroy other entities linked via soft links (aka symlink or symbolic link, similar concept to the "shortcut files" of MS Windows); yellow visualisation
			GROUP_LINK = 2			///< Linked to an entity group; blue visualisation
		};

		/**
		*  @brief
		*    Link aspect flags
		*
		*  @remarks
		*    The link aspect flags are automatically derived from the link type
		*/
		enum LinkAspectFlags
		{
			TRANSFORM					 = 1<<0,	///< Move the child together with its parent
			PARENT_DEPENDING_EXISTENCE	 = 1<<1,	///< The existence of the child depends on its parent, destroy the parent and the child is destroyed automatically as well
			PARENT_INDEPENDENT_EXISTENCE = 1<<2,	///< The existence of the child does not depend on its parent, destroy the parent and the child will still exist
			SELECT_PARENT				 = 1<<3,	///< Indirect the selection to the parent
			LIST_PARENT					 = 1<<4		///< Do not list the entity, list its parent instead
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get topmost ancestor (i.e. parent of the parent of the...) of a prototype
		*
		*  @param[in] prototype
		*    Prototype to check
		*  @param[in] linkAspectFlags
		*    Link aspect flags, e.g. "SELECT_PARENT"
		*
		*  @return
		*    Reference to the topmost parent's link component, may be *this in case link has no parent
		*/
		static Prototype& getTopmostAncestorPrototype(Prototype& prototype, uint8 linkAspectFlags);

		/**
		*  @brief
		*    Get listing of all direct child prototypes of a given prototype
		*
		*  @param[in] prototype
		*    Prototype to check
		*  @param[in] linkAspectFlags
		*    Link aspect flags, e.g. "SELECT_PARENT"
		*  @param[out] outChildPrototypes
		*    List of child prototypes
		*
		*  @return
		*    "true" in case any children were found, "false" if there are none
		*/
		static bool getDirectChildren(Entity& prototype, uint8 linkAspectFlags, std::vector<Entity*>& outChildren);
		static bool getDirectChildren(Prototype& prototype, uint8 linkAspectFlags, std::vector<Prototype*>& outChildren);

		/**
		*  @brief
		*    Get group of all link components directly or indirectly connected to an entity
		*
		*  @param[in] entity
		*    Entity to check
		*  @param[in] linkAspectFlags
		*    Link aspect flags, e.g. "SELECT_PARENT"
		*  @param[in] downwardsOnly
		*    If true, only children (and their children, and so on) are added, as if there was no parent
		*  @param[out] outLinkGroup
		*    List of pointers to all connected entities, including the given entity; ordering is depth-first with parents before their children; list is not cleared before adding new entities
		*/
		static void getConnectedLinkGroup(Entity& entity, uint8 linkAspectFlags, bool downwardsOnly, std::vector<Entity*>& outLinkGroup, bool sortedById = false);
		static void getConnectedLinkGroup(const Entity& entity, uint8 linkAspectFlags, bool downwardsOnly, std::vector<const Entity*>& outLinkGroup, bool sortedById = false);

		/**
		*  @brief
		*    Get group of all link components directly or indirectly connected to a prototype
		*
		*  @param[in] prototype
		*    Prototype to check
		*  @param[in] linkAspectFlags
		*    Link aspect flags, e.g. "SELECT_PARENT"
		*  @param[in] downwardsOnly
		*    If true, only children (and their children, and so on) are added, as if there was no parent
		*  @param[out] outLinkGroup
		*    List of pointers to all connected entities, including the given prototype; ordering is depth-first with parents before their children; list is not cleared before adding new entities
		*/
		static void getConnectedLinkGroup(Prototype& prototype, uint8 linkAspectFlags, bool downwardsOnly, std::vector<Prototype*>& outLinkGroup, bool sortedById = false);
		static void getConnectedLinkGroup(const Prototype& prototype, uint8 linkAspectFlags, bool downwardsOnly, std::vector<const Prototype*>& outLinkGroup, bool sortedById = false);

		/**
		*  @brief
		*    Get the color used for link visualisation of a certain link type
		*
		*  @param[in] linkType
		*    Link type to return the visualisation color for
		*
		*  @return
		*    The visualisation color for the given link type
		*/
		static Color4 getLinkTypeColor(LinkType linkType);

		/**
		*  @brief
		*    Check whether or not the given entity is considered to be an object
		*
		*  @return
		*    "true" if the given entity is considered to be an object, else "false"
		*
		*  @note
		*    - "Objects" are formed by linking entities together, prefab instances generate objects (linked entities)
		*/
		static bool isEntityObject(const Entity& entity);


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
		explicit LinkComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~LinkComponent();

		//[-------------------------------------------------------]
		//[ Abstract link methods                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return whether or not it's possible to select the owner of this component
		*
		*  @return
		*    "true" if it's possible to select the owner of this component, else "false"
		*
		*  @remarks
		*    Depending on the concrete link settings the prototype the link component is part of the prototype
		*    should act as individual prototype or be hidden as part of a more complex object or group. Use this
		*    abstract method in case you're only interested in whether or not the prototype should be selectable
		*    as individual prototype inside e.g. a render view without caring about the details.
		*
		*    Use e.g.
		*    @code
		*    const uint64 prototypeId = qsf::LinkComponent::getTopmostAncestorPrototype(prototype, LinkComponent::HARD_LINK).getId();
		*    @endcode
		*    in order to get the topmost selectable prototype inside an prototype hierarchy.
		*/
		inline bool isSelectable() const;

		/**
		*  @brief
		*    Return whether or not to list the owner of this component
		*
		*  @return
		*    "true" to list the owner of this component, else "false"
		*
		*  @remarks
		*    Depending on the concrete link settings the prototype the link component is part of the prototype
		*    should act as individual prototype or be hidden as part of a more complex object or group. Use this
		*    abstract method in case you're only interested in whether or not the prototype should be listed
		*    as individual prototype inside e.g. a GUI without caring about the details.
		*/
		inline bool isListable() const;

		//[-------------------------------------------------------]
		//[ Concrete link methods                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the link type
		*
		*  @return
		*    The link type
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline LinkType getParentLinkType() const;

		/**
		*  @brief
		*    Set the link type
		*
		*  @param[in] linkType
		*    The link type
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setParentLinkType(LinkType linkType);

		/**
		*  @brief
		*    Return the parent link aspect flags derived from the link type
		*
		*  @return
		*    The parent link aspect flags derived from the link type, see "qsf::LinkComponent::LinkAspectFlags"
		*/
		inline uint8 getParentLinkAspectFlags() const;

		/**
		*  @brief
		*    Return true if a link parent is set
		*/
		inline bool hasParent() const;

		/**
		*  @brief
		*    Return the link parent prototype's ID
		*
		*  @return
		*    The unique prototype/entity identifier of the link target, "qsf::getUninitialized<uint64>()" if invalid
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline uint64 getParentId() const;

		/**
		*  @brief
		*    Return the parent link component
		*
		*  @return
		*    The parent link component, can be a null pointer, do not destroy the instance
		*/
		inline LinkComponent* getParentLinkComponent() const;

		/**
		*  @brief
		*    Return the link parent
		*
		*  @return
		*    Pointer to the link parent, or null pointer if there is none set
		*/
		Prototype* getParentPrototype() const;

		/**
		*  @brief
		*    Set the link parent
		*
		*  @param[in] parentId
		*    The unique prototype/entity identifier of the link target, "qsf::getUninitialized<uint64>()" if invalid
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setParentId(uint64 parentId);

		/**
		*  @brief
		*    Set the link parent
		*
		*  @param[in] parentPrototype
		*    The prototype/entity of the parent to link to
		*  @param[in] linkType
		*    The link type to set
		*/
		void linkToParent(const Prototype& parentPrototype, LinkType linkType = HARD_LINK);

		/**
		*  @brief
		*    Unlink from linked parent
		*/
		void unlinkFromParent();

		/**
		*  @brief
		*    Return the list of link component instances of the link children
		*
		*  @return
		*    Reference to the internal list of child link component instances, do not manipulate the list or destroy the link components
		*/
		inline const boost::container::flat_set<LinkComponent*>& getChildLinks() const;

		/**
		*  @brief
		*    Get topmost ancestor (i.e. parent of the parent of the...)
		*
		*  @param[in] linkAspectFlags
		*    Link aspect flags, e.g. "SELECT_PARENT"
		*
		*  @return
		*    Reference to the topmost parent's link component, may be *this in case link has no parent
		*/
		LinkComponent& getTopmostAncestorLink(uint8 linkAspectFlags) const;

		/**
		*  @brief
		*    Get group of all link components directly or indirectly connected to this one
		*
		*  @param[in] linkAspectFlags
		*    Link aspect flags, e.g. "SELECT_PARENT"
		*  @param[in] downwardsOnly
		*    If true, only children (and their children, and so on) are added, as if there was no parent
		*  @param[out] outLinkGroup
		*    List of pointers to all connected link components, including this; list is not cleared before adding new entities
		*/
		void getConnectedLinkGroup(uint8 linkAspectFlags, bool downwardsOnly, std::vector<LinkComponent*>& outLinkGroup, bool sortedById = false) const;

		//[-------------------------------------------------------]
		//[ Local transform                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the local position
		*
		*  @return
		*    The position, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec3& getLocalPosition() const;

		/**
		*  @brief
		*    Set the local position
		*
		*  @param[in] position
		*    The position
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setLocalPosition(const glm::vec3& position);

		/**
		*  @brief
		*    Set the local position and update the world transform from the parent's animation transform
		*
		*  @param[in] position
		*    The position
		*
		*  @note
		*    - This method should only be preferred to "setLocalPosition" in case the local transformation is updated just for animation effects
		*/
		void setLocalPositionForAnimation(const glm::vec3& position);

		/**
		*  @brief
		*    Return the local rotation
		*
		*  @return
		*    The rotation, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::quat& getLocalRotation() const;

		/**
		*  @brief
		*    Set the local rotation
		*
		*  @param[in] rotation
		*    The rotation quaternion
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setLocalRotation(const glm::quat& rotation);

		/**
		*  @brief
		*    Set the local rotation and update the world transform from the parent's animation transform
		*
		*  @param[in] rotation
		*    The rotation quaternion
		*
		*  @note
		*    - This method should only be preferred to "setLocalRotation" in case the local transformation is updated just for animation effects
		*/
		void setLocalRotationForAnimation(const glm::quat& rotation);

		/**
		*  @brief
		*    Return the local scale
		*
		*  @return
		*    The scale, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec3& getLocalScale() const;

		/**
		*  @brief
		*    Set the local scale
		*
		*  @param[in] scale
		*    The scale
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setLocalScale(const glm::vec3& scale);

		/**
		*  @brief
		*    Return the local transform
		*
		*  @return
		*    The transform, do no destroy the returned instance
		*/
		inline const Transform& getLocalTransform() const;

		/**
		*  @brief
		*    Set the local transform
		*
		*  @param[in] transform
		*    The transform
		*/
		void setLocalTransform(const Transform& transform);

		//[-------------------------------------------------------]
		//[ Helper methods                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the given component of the entity; if not found it ask the linked parent entity
		*
		*  @return
		*    Pointer to the searched component or a null pointer
		*/
		template<typename T>
		T* getComponentFromEntityOrLinkedParent() const;

		/**
		*  @brief
		*    Returns the given component of the entity; if not found it ask the linked child entities
		*
		*  @return
		*    Pointer to the searched component or a null pointer
		*/
		template<typename T>
		T* getComponentFromEntityOrLinkedChild() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void onInvalidation() override;
		virtual void serialize(BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool implementsOnComponentPropertyChange() const override;
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;
		virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags) override;
		virtual void onSetSimulating(bool simulating) override;
		virtual void onSetDebug(bool debug) override;
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		uint8 getLinkAspectFlagsFromType() const;
		void registerAtParent(Prototype& parentPrototype, bool isInStartup = false);
		void registerAtParent(LinkComponent& parentLink, bool isInStartup = false);
		void unregisterAtParent();
		void addChildLink(LinkComponent& childLink);
		void removeChildLink(LinkComponent& childLink);

		void updateLocalTransformation();
		void updateWorldTransformationFromLocal(bool forAnimation = false);
		void updateWorldTransformationFromLocal(const Transform& parentTransform, TransformChangeFlags flags);
		void updateDebug();
		void onPreCompositorWorkspaceUpdate(const CameraComponent& cameraComponent);

		Component* getComponentFromEntityOrLinkedParentById(const StringHash& id) const;
		Component* getComponentFromEntityOrLinkedChildById(const StringHash& id) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		LinkType  mParentLinkType;				///< Link type
		uint64	  mParentId;					///< Prototype ID of link parent
		Transform mLocalTransform;				///< Local transformation relative to parent; not used if there is no parent

		// Direct component references for efficient access
		WeakPtr<TransformComponent> mTransformComponent;
		WeakPtr<TransformComponent> mParentTransformComponent;

		// Internal only
		LinkComponent* mParentLink;				///< Parent link component instance, or null pointer if there is not parent set
		uint8		   mParentLinkAspectFlags;	///< Link aspect flags derived from the link type
		bool		   mListen;
		DebugDrawProxy mDebugDrawProxy;			///< Debug draw proxy
		boost::signals2::connection mPreCompositorWorkspaceUpdateConnection;

		typedef boost::container::flat_set<LinkComponent*> LinkSet;
		LinkSet mChildLinks;					///< List of link component instances linked to the owner prototype of this link component, we're not responsible for destroying the instances


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
#include "qsf/component/link/LinkComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::LinkComponent)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::LinkComponent::LinkType)
