// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/BoostAssertHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline const std::vector<PluginableBase*>& PluginableManager::getPluginableList() const
	{
		return mPluginables;
	}

	template <typename T> const std::vector<T*>& PluginableManager::getList() const
	{
		return reinterpret_cast<const std::vector<T*>&>(mPluginables);
	}

	inline PluginableBase* PluginableManager::getPluginableById(const StringHash& id) const
	{
		const boost::container::flat_map<uint32, PluginableBase*>::const_iterator iterator = mPluginableMap.find(id.getHash());
		return (iterator != mPluginableMap.cend()) ? iterator->second : nullptr;
	}

	template <typename T> T* PluginableManager::get() const
	{
		return static_cast<T*>(getPluginableById(T::PLUGINABLE_ID));
	}

	template <typename T> T* PluginableManager::getById(const StringHash& id) const
	{
		return static_cast<T*>(getPluginableById(id));
	}

	template <typename T> T& PluginableManager::getSafe() const
	{
		T* instance = get<T>();
		QSF_CHECK(nullptr != instance, "QSF did not find the requested pluginable instance", QSF_REACT_THROW);
		return *instance;
	}

	template <typename T> T& PluginableManager::getByIdSafe(const StringHash& id) const
	{
		T* instance = getById<T>(id);
		QSF_CHECK(nullptr != instance, "QSF did not find the requested pluginable instance", QSF_REACT_THROW);
		return *instance;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
