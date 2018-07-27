// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once

//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/asset/AssetSystemTypes.h"

#include <boost/signals2.hpp>


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
	*    Asset background loader, loads assets of specific, background-loadable, types in the background.
	*
	*  @remarks
	*    To load an asset in the background, create a new AssetBackgroundLoader instance and pass the global asset
	*    id of the asset in question to the constructor. Then check if it is already loaded (@see isLoaded). If it is
	*    not already loaded, connect yourself to the @see AssetLoaded signal and then kick-off background loading
	*    by calling @see beginLoading(). Once the asset is loaded, it will invoke the signal (in the main thread).
	*
	*    If you destruct the background loader while the load is already in progress, it will be canceled.
	*
	*  @remarks
	*    Assets should be loaded prior to any use. For example:
	*    You want to show a mesh in the scene, but it is rather big and you only want to load it for that
	*    specific point in time. Then you can't just fire up a AssetBackgroundLoader and immediately create
	*    an MeshComponent and assign the mesh to it. You'd have to wait until the mesh actually was loaded
	*    and then assign it to your MeshComponent. If you assign it earlier, loading will be elevated to
	*    the main thread and lock it up. So always wait on your assets before usage! Or try to use
	*    proxying to shadow the fact that there actually is no data yet.
	*
	*  @note
	*    In order to correctly load OGRE resource-style assets, the resource has to be known by OGREs resource
	*    system beforehand. This restriction is imposed to allow quick queries to Ogre::Resource::isLoaded() or to
	*    query the exact resource group of the resource, which is not possible if the resource is not already declared.
	*
	*/
	class QSF_API_EXPORT AssetBackgroundLoader : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Protected definitions                                 ]
	//[-------------------------------------------------------]
	protected:
		struct AssetBackgroundLoaderData;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] globalAssetId
		*    The global asset identifier
		*/
		AssetBackgroundLoader(GlobalAssetId globalAssetId);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    Destructing the loader while loading is in-progress will cancel the loading
		*/
		virtual ~AssetBackgroundLoader();

		//[-------------------------------------------------------]
		//[ Properties                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the global asset identifier of the asset this loader is loading
		*
		*  @return
		*    The global asset identifier of the asset which is loaded by this background loader
		*/
		GlobalAssetId getGlobalAssetId() const;

		/**
		*  @brief
		*    Starts background loading if the resource is not loaded yet and no loading is in-progress
		*/
		void beginLoading();

		/**
		*  @brief
		*    Cancels loading of the resource (but only if it still is in the background queue, otherwise, this has no effect)
		*/
		void cancel();

		/**
		*  @brief
		*    Checks if the resource is already loaded.
		*
		*  @note
		*    Will always return true for assets which cannot be loaded in the background!
		*/
		bool isLoaded() const;

		/**
		*  @brief
		*    Checks if the background loader is currently loading the asset in the background
		*
		*/
		bool isLoading() const;

		/**
		*  @brief
		*    Returns true if the asset is actually loadable in the background
		*/
		bool isBackgroundLoadable() const;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*   Figures out the OGRE resource type of the encapsulated asset
		*
		*  @return
		*   Will return true if the resource type has been filled out or false otherwise (e.g. when the asset is no OGRE resource)
		*/
		bool getOgreResourceType(std::string& outResourceType) const;


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void (GlobalAssetId)> AssetLoaded;		///< This Boost signal is emitted when the asset finished loading


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		AssetBackgroundLoaderData*	mData;	///< "PIMPL" for internal data to avoid header pollution


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
