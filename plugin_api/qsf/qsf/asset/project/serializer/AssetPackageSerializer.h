// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/serialization/Serializer.h"

#include <ostream>
#include <istream>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AssetPackage;
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
	*    Abstract asset package serializer interface
	*
	*  @note
	*    - Must work with exceptions in order to be able to interact with standard C++ streams
	*/
	class AssetPackageSerializer : public Serializer
	{


	//[-------------------------------------------------------]
	//[ Public virtual qsf::AssetPackageSerializer methods    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Deserialize an asset package by using a given input stream
		*
		*  @param[in] assetPackage
		*    Asset package to fill
		*  @param[in] istream
		*    Input stream to deserialize the asset package from
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		virtual void deserialize(AssetPackage& assetPackage, std::istream& istream) const = 0;

		/**
		*  @brief
		*    Serialize the asset package into a given output stream
		*
		*  @param[in] assetPackage
		*    Asset package instance to serialize
		*  @param[in] ostream
		*    Output stream to serialize the asset package into
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		virtual void serialize(const AssetPackage& assetPackage, std::ostream& ostream) const = 0;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
