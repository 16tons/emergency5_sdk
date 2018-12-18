// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/AssetStorageManagerTypes.h"

#include <boost/noncopyable.hpp>

#include <set>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AssetSystem;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Asset priority provider class
		*/
		class AssetPriorityProvider : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			AssetPriorityProvider();

			/**
			*  @brief
			*    Destructor
			*/
			~AssetPriorityProvider();

			/**
			*  @brief
			*    Return whether or not the given asset has to be synchronized before the map loading will be started
			*
			*  @param[in] asset
			*    Asset to evaluate
			*
			*  @return
			*    "true" if the given asset has to be synchronized before the map loading will be started, else "false"
			*/
			bool synchronizeAssetBeforeMapLoad(const Asset& asset);

			/**
			*  @brief
			*    Called to let the method pick one of many assets
			*
			*  @param[in] editorCachedAssetDownloads
			*    Set of scheduled editor cached asset downloads to pick one from
			*
			*  @return
			*    An iterator pointing to the asset picked from the given set
			*/
			std::set<EditorCachedAssetDownload>::const_iterator pickOneMethod(const std::set<EditorCachedAssetDownload>& editorCachedAssetDownloads);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			const AssetSystem& mAssetSystem;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
