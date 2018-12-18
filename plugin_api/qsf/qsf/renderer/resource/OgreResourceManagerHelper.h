// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"
#include "qsf/asset/AssetSystemTypes.h"

#include <boost/signals2/connection.hpp>
#include <boost/container/flat_set.hpp>

#include <unordered_map>
#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class ResourceManager;
}
namespace qsf
{
	class AssetSystem;
	namespace detail
	{
		class DependencyCallback;
	}
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
	*    This class automatically connects to the asset system and reloads OGRE resources whenever their associated (cached) assets get (re)mounted
	*/
	class QSF_API_EXPORT OgreResourceManagerHelper : public Manager
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const char MESH_PLACEHOLDER_ASSET[];
		static const char TEXTURE_PLACEHOLDER_ASSET[];


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		OgreResourceManagerHelper();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OgreResourceManagerHelper();

		int registerCallBackWhenAvailable(const boost::container::flat_set<GlobalAssetId>& assetSet, boost::function<void()> callback);

		void deregisterCallBack(int callbackId);

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the OGRE resource manager helper
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - Do not call in server mode
		*/
		bool onStartup();

		/**
		*  @brief
		*    Shutdown the OGRE resource manager helper
		*
		*  @remarks
		*    Do not call in server mode
		*
		*  @note
		*    - This method is only called case "onStartup()" returned successfully
		*/
		void onShutdown();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onAssetsMounted(const Assets& assets);
		void onAssetsUnmounted(const Assets& assets);
		bool notifyAllResourceManagersThatResourceGotUpdated(const Asset& asset, bool mounted);
		bool notifyResourceManagerThatResourceGotUpdated(Ogre::ResourceManager& resourceManager, const Asset& asset, bool mounted);
		void onMissingDependenciesResolved(Ogre::ResourceManager* resourceManager, GlobalAssetId globalAssetId);

		/**
		*  @brief
		*    Reinitialise OGRE entities by using a given global mesh asset ID
		*
		*  @param[in] globalAssetId
		*    Global mesh asset ID
		*
		*  @remarks
		*    "Ogre::v1::SubEntity" is using raw OGRE sub-mesh pointers. So, reloading an OGRE mesh resource will invalidate those pointers - OGRE does not handle
		*    this situation automatically and will just crash.
		*
		*    See http://www.ogre3d.org/forums/viewtopic.php?t=34862 and http://www.ogre3d.org/mantis/view.php?id=182 for background information.
		*
		*    To keep the solution simple and local, we're reinitializing all OGRE entities of all OGRE scene managers using the reloaded mesh asset. This is
		*    certainly not effective, but at the point of fixing a certain crash related to this odd OGRE behaviour this solution was totally sufficient. In
		*    case this becomes a real performance issue, a more complex and less local solution has to be implemented.
		*/
		void reinitialiseOgreEntitiesByGlobalMeshAssetId(GlobalAssetId globalAssetId) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AssetSystem&																							mAssetSystem;	///< Asset system instance, do not destroy the instance
		std::unordered_map<GlobalAssetId, std::unordered_map<int, std::shared_ptr<detail::DependencyCallback>>> mDependencyMap;
		int																										mLastDependencyCallbackId;
		boost::signals2::connection																				slot_onAssetsMounted;
		boost::signals2::connection																				slot_onAssetsUnmounted;
		std::vector<int>																						mResourceCallbackIds;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
