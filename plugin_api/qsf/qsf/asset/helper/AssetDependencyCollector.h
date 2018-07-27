// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/asset/AssetSystemTypes.h"

#include <boost/container/flat_set.hpp>

#include <vector>


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
	*    Asset dependency collector class
	*
	*  @note
	*    - Is looking up all (recursive) dependencies of an asset
	*/
	class QSF_API_EXPORT AssetDependencyCollector
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit AssetDependencyCollector(GlobalAssetId assetId);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AssetDependencyCollector();

		/**
		*	@brief
		*     Collects all recursive dependencies of the source asset itself
		*
		*   @param[out] outAssetList
		*     All found dependencies are stored in this list. The list is not cleared!
		*
		*   @note
		*     The source asset is treated as a dependency, too!
		*/
		void collectUniqueRecursiveAssetDependencies(std::vector<GlobalAssetId>& outAssetList);
		void collectUniqueRecursiveAssetDependenciesWithDeriverance(std::vector<GlobalAssetId>& outAssetList);

		void collectAllDerivedAssets(std::vector<GlobalAssetId>& outAssetList);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void collectDependencies(boost::container::flat_set<GlobalAssetId>& outAssetList, bool withDeriverance);
		void collectDerivedAssetsFromMultipleSources(const boost::container::flat_set<GlobalAssetId>& sourceAssetList, boost::container::flat_set<GlobalAssetId>& outDerivedAssetList) const;
		bool resolveDependenciesInline(boost::container::flat_set<GlobalAssetId>& inOutAssets) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		GlobalAssetId mSourceAsset;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
