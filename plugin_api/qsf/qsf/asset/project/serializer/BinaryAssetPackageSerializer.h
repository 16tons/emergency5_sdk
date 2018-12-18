// Copyright (C) 2012-2018 Promotion Software GmbH


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
namespace qsf
{
	class BinarySerializer;
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
	*    Binary asset package serializer
	*/
	class QSF_API_EXPORT BinaryAssetPackageSerializer : public AssetPackageSerializer
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const std::string FILE_EXTENSION;
		static const std::string FORMAT_TYPE;
		static const uint32		 FORMAT_VERSION;
		static const uint32		 FORMAT_VERSION_READABLE[];


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline BinaryAssetPackageSerializer();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~BinaryAssetPackageSerializer();

		/**
		*  @brief
		*    Serialization of the asset package
		*/
		void serialize(AssetPackage& assetPackage, BinarySerializer& serializer) const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::AssetPackageSerializer methods    ]
	//[-------------------------------------------------------]
	public:
		virtual void deserialize(AssetPackage& assetPackage, std::istream& istream) const override;
		virtual void serialize(const AssetPackage& assetPackage, std::ostream& ostream) const override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Checks if the stream version is readable by this serializer
		*/
		template<int N>
		bool isVersionReadable(uint32 formatVersion, const uint32 (&supportedVersions)[N]) const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/asset/project/serializer/BinaryAssetPackageSerializer-inl.h"
