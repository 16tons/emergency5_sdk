// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline uint64 Prototype::getId() const
	{
		return mId;
	}

	inline BasePrototypeManager& Prototype::getPrototypeManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mManager, "The prototype manager instance is invalid", QSF_REACT_THROW);
		return *mManager;
	}

	inline void Prototype::reserveComponents(size_t numberOfComponents)
	{
		mComponentMap.reserve(numberOfComponents);
		mComponentList.reserve(numberOfComponents);
	}

	inline Component* Prototype::createComponentById(const StringHash& id, bool startup)
	{
		return internalCreateComponent(id, nullptr, startup);
	}

	template <typename T> T* Prototype::createComponent(bool startup)
	{
		return static_cast<T*>(createComponentById(T::COMPONENT_ID, startup));
	}

	inline Component* Prototype::createComponentByIdWithValues(const StringHash& id, const std::string& values, bool startup)
	{
		return internalCreateComponent(id, &values, startup);
	}

	template <typename T> T* Prototype::createComponentWithValues(const std::string& values, bool startup)
	{
		return static_cast<T*>(createComponentByIdWithValues(T::COMPONENT_ID, values, startup));
	}

	template <typename T> bool Prototype::destroyComponent()
	{
		return destroyComponentById(T::COMPONENT_ID);
	}

	template <typename T> T* Prototype::getComponent() const
	{
		const ComponentMap::const_iterator iterator = mComponentMap.find(T::COMPONENT_ID);
		return (iterator != mComponentMap.cend()) ? static_cast<T*>(iterator->second) : nullptr;
	}

	template <typename T> T& Prototype::getComponentSafe() const
	{
		T* component = getComponent<T>();
		QSF_CHECK(nullptr != component, "There is no component with ID " << T::COMPONENT_ID << " in prototype " << getId(), QSF_REACT_THROW);
		return *component;
	}

	inline Component* Prototype::getComponentById(const StringHash& id) const
	{
		const ComponentMap::const_iterator iterator = mComponentMap.find(id);
		return (iterator != mComponentMap.cend()) ? iterator->second : nullptr;
	}

	template <typename T> T* Prototype::getOrCreateComponent(bool startup)
	{
		return static_cast<T*>(getOrCreateComponentById(T::COMPONENT_ID, startup));
	}

	template <typename T> T& Prototype::getOrCreateComponentSafe(bool startup)
	{
		Component* component = getOrCreateComponentById(T::COMPONENT_ID, startup);
		QSF_CHECK(nullptr != component, "Could not create an instance of component with ID " << T::COMPONENT_ID << " in prototype " << getId(), QSF_REACT_THROW);
		return static_cast<T&>(*component);
	}

	inline const std::vector<Component*>& Prototype::getComponents() const
	{
		return mComponentList;
	}

	inline bool Prototype::isEntity() const
	{
		return mFlags.isSet(ENTITY);
	}

	inline bool Prototype::getIgnorePropertyChanges() const
	{
		return mFlags.isSet(IGNORE_PROPERTY_CHANGES);
	}

	inline void Prototype::setIgnorePropertyChanges(bool ignorePropertyChanges)
	{
		return mFlags.set(IGNORE_PROPERTY_CHANGES, ignorePropertyChanges);
	}

	//[-------------------------------------------------------]
	//[ Misc                                                  ]
	//[-------------------------------------------------------]
	inline uint8 Prototype::getComponentChangesCount() const
	{
		return mComponentChangesCount;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline Prototype::Prototype(uint64 id, BasePrototypeManager& manager) :
		mId(id),
		mManager(&manager),
		mComponentChangesCount(0)
	{
		// By default, prototypes are serializable
		mFlags.set(SERIALIZABLE);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
