// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/asset/AssetDataSerializer.h"

#include <ostream>
#include <istream>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Material;
	class MaterialManager;
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
	*    Abstract material serializer interface
	*
	*  @note
	*    - Must work with exceptions in order to be able to interact with standard C++ streams
	*/
	class QSF_API_EXPORT MaterialSerializer : public AssetDataSerializer
	{


	//[-------------------------------------------------------]
	//[ Public virtual qsf::MaterialSerializer methods        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Deserialize a material by using a given input stream
		*
		*  @param[in] materialManager
		*    Material manager to use
		*  @param[in] materialName
		*    Material name to use
		*  @param[in] istream
		*    Input stream to deserialize the material from
		*
		*  @return
		*    The loaded material
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		virtual Material* deserialize(MaterialManager& materialManager, const std::string& materialName, std::istream& istream) const = 0;

		/**
		*  @brief
		*    Serialize the material into a given output stream
		*
		*  @param[in] materialManager
		*    Material manager to use
		*  @param[in] materialName
		*    Material name to use
		*  @param[in] ostream
		*    Output stream to serialize the material into
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		virtual void serialize(const MaterialManager& materialManager, const std::string& materialName, std::ostream& ostream) const = 0;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
