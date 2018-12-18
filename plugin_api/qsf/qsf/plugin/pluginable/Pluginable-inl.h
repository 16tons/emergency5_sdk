// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	template<typename T>
	Pluginable<T>::~Pluginable()
	{
		// Nothing to do in here
	}

	template<typename T>
	T& Pluginable<T>::getManager() const
	{
		return getPluginableManager<T>();
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	template<typename T>
	Pluginable<T>::Pluginable(T* pluginableManager) :
		PluginableBase(static_cast<PluginableManager*>(pluginableManager))
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
