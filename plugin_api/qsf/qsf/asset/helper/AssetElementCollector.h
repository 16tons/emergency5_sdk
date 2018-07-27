// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/asset/AssetProxy.h"
#include "qsf/base/GetUninitialized.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace boost
{
	namespace property_tree
	{
		template <typename U, typename V, typename W>
		class basic_ptree;
		typedef basic_ptree<std::string, std::string, std::less<std::string> > ptree;
	}
}
namespace qsf
{
	class StringHash;
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
	*    Asset element collector class
	*/
	class QSF_API_EXPORT AssetElementCollector
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct AssetContent
		{
			std::string mSourceFilename;
			AssetProxy  mSourceAsset;
			boost::property_tree::ptree* mPTree;

			AssetContent(const std::string& source, const AssetProxy& assetProxy, boost::property_tree::ptree& pTree) :
				mSourceFilename(source),
				mSourceAsset(assetProxy),
				mPTree(&pTree)
			{}
		};
		typedef std::vector<AssetContent> AssetContentArray;

		struct Element
		{
			std::string mName;
			std::string mSourceFilename;
			AssetProxy  mSourceAsset;
			boost::property_tree::ptree* mPTree;
		};
		typedef std::vector<Element> ElementArray;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		AssetElementCollector(const std::string& formatType, uint32 formatVersion);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AssetElementCollector();

		/**
		*  @brief
		*    Clear loaded contents
		*/
		void clear();

		/**
		*  @brief
		*    Load contents of all assets of the given type
		*
		*  @param[in] assetType
		*    The type of asset to load all known instances of
		*  @param[in] showMessageBoxOnParserError
		*    If "true", show a message box to the user if a parser error occurs reading the file
		*
		*  @return
		*    "true" if everything went fine, if one or more failed to load "false"
		*/
		bool loadAssetsOfType(const StringHash& assetType, bool showMessageBoxOnParserError);

		/**
		*  @brief
		*    Return the internal asset content array
		*
		*  @return
		*    The internal asset content array
		*/
		const AssetContentArray& getAssetContentArray() const;

		/**
		*  @brief
		*    Collect elements from the formerly loaded assets
		*
		*  @note
		*    - Elements get invalid when this AssetElementCollector instance is destroyed
		*/
		void collectOneElementPerAsset(ElementArray& outElements, const std::string& path = "");

		/**
		*  @brief
		*    Collect elements from the formerly loaded assets
		*
		*  @note
		*    - Elements get invalid when this AssetElementCollector instance is destroyed
		*/
		void collectTopLevelElements(ElementArray& outElements);

		/**
		*  @brief
		*    Collect elements from the formerly loaded assets
		*
		*  @note
		*    - Elements get invalid when this AssetElementCollector instance is destroyed
		*/
		void collectElementsAtPath(const std::string& path, ElementArray& outElements);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool addAssetFromJsonFile(const std::string& filename, const AssetProxy& assetProxy, bool showMessageBoxOnParserError);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AssetContentArray mAssetContents;
		std::string		  mFormatType;
		uint32			  mFormatVersion;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
