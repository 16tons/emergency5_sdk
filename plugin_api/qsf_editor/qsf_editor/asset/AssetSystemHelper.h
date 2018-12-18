// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <vector>
#include <string>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
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
		*    Asset system helper class
		*/
		class QSF_EDITOR_API_EXPORT AssetSystemHelper : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			AssetSystemHelper();

			/**
			*  @brief
			*    Destructor
			*
			*  @note
			*    - Not virtual by intent
			*/
			~AssetSystemHelper();

			/**
			*  @brief
			*    Return a list of asset categories
			*
			*  @param[out] assetCategories
			*    Receives the list of asset categories, list is not cleared before new entries are added
			*/
			void getAssetCategories(std::vector<std::string>& assetCategories) const;

			/**
			*  @brief
			*    Return a list of asset tags
			*
			*  @param[out] assetTags
			*    Receives the list of asset tags, list is not cleared before new entries are added
			*/
			void getAssetTags(std::vector<std::string>& assetTags) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			AssetSystem& mAssetSystem;	///< Asset system instance to use, always valid, do not destroy the instance


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
