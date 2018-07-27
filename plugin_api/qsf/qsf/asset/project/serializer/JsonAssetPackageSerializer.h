// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/asset/project/serializer/AssetPackageSerializer.h"


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
	*    JSON asset package serializer
	*/
	class QSF_API_EXPORT JsonAssetPackageSerializer : public AssetPackageSerializer
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const std::string  FILE_EXTENSION;
		static const char		  FORMAT_TYPE[];
		static const uint32		  FORMAT_VERSION;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline JsonAssetPackageSerializer();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~JsonAssetPackageSerializer();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::AssetPackageSerializer methods    ]
	//[-------------------------------------------------------]
	public:
		virtual void deserialize(AssetPackage& assetPackage, std::istream& istream) const override;
		virtual void serialize(const AssetPackage& assetPackage, std::ostream& ostream) const override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Load a JSON asset package
		*
		*  @param[in] assetPackage
		*    Asset package instance to fill
		*  @param[in] rootPTree
		*    Boost root ptree
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		void loadJson(AssetPackage& assetPackage, boost::property_tree::ptree& rootPTree) const;

		/**
		*  @brief
		*    Load an asset package
		*
		*  @param[in] assetPackage
		*    Asset package instance to fill
		*  @param[in] rootPTree
		*    Boost root ptree
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		void loadAssetPackage(AssetPackage& assetPackage, boost::property_tree::ptree& rootPTree) const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/asset/project/serializer/JsonAssetPackageSerializer-inl.h"
