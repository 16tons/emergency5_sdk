// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LinkComponent::~LinkComponent()
	{
		// Nothing to do in here
	}

	inline bool LinkComponent::isSelectable() const
	{
		return (nullptr == mParentLink || !(mParentLinkAspectFlags & SELECT_PARENT));
	}

	inline bool LinkComponent::isListable() const
	{
		return (nullptr == mParentLink || !(mParentLinkAspectFlags & LIST_PARENT));
	}

	inline LinkComponent::LinkType LinkComponent::getParentLinkType() const
	{
		return mParentLinkType;
	}

	inline uint8 LinkComponent::getParentLinkAspectFlags() const
	{
		return mParentLinkAspectFlags;
	}

	inline bool LinkComponent::hasParent() const
	{
		return (nullptr != mParentLink);
	}

	inline uint64 LinkComponent::getParentId() const
	{
		return mParentId;
	}

	inline LinkComponent* LinkComponent::getParentLinkComponent() const
	{
		return mParentLink;
	}

	inline const boost::container::flat_set<LinkComponent*>& LinkComponent::getChildLinks() const
	{
		return mChildLinks;
	}

	inline const glm::vec3& LinkComponent::getLocalPosition() const
	{
		return mLocalTransform.getPosition();
	}

	inline const glm::quat& LinkComponent::getLocalRotation() const
	{
		return mLocalTransform.getRotation();
	}

	inline const glm::vec3& LinkComponent::getLocalScale() const
	{
		return mLocalTransform.getScale();
	}

	inline const Transform& LinkComponent::getLocalTransform() const
	{
		return mLocalTransform;
	}

	template<typename T>
	T* LinkComponent::getComponentFromEntityOrLinkedParent() const
	{
		return static_cast<T*>(getComponentFromEntityOrLinkedParentById(T::COMPONENT_ID));
	}

	template<typename T>
	T* LinkComponent::getComponentFromEntityOrLinkedChild() const
	{
		return static_cast<T*>(getComponentFromEntityOrLinkedChildById(T::COMPONENT_ID));
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	inline bool LinkComponent::implementsOnComponentPropertyChange() const
	{
		// React on component property change
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
