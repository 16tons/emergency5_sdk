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
	class MaterialBlueprint;
	class ShaderBlueprintManager;
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
	*    Abstract material blueprint serializer interface
	*
	*  @note
	*    - Must work with exceptions in order to be able to interact with standard C++ streams
	*/
	class QSF_API_EXPORT MaterialBlueprintSerializer : public AssetDataSerializer
	{


	//[-------------------------------------------------------]
	//[ Public virtual qsf::MaterialBlueprintSerializer methods ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Deserialize a material blueprint by using a given input stream
		*
		*  @param[in] materialBlueprint
		*    Material blueprint to use deserialize
		*  @param[in] shaderBlueprintManager
		*    Shader blueprint manager to use
		*  @param[in] istream
		*    Input stream to deserialize the material blueprint from
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		virtual void deserialize(MaterialBlueprint& materialBlueprint, const ShaderBlueprintManager& shaderBlueprintManager, std::istream& istream) const = 0;

		/**
		*  @brief
		*    Serialize the material blueprint into a given output stream
		*
		*  @param[in] materialBlueprint
		*    Material blueprint to serialize
		*  @param[in] shaderBlueprintManager
		*    Shader blueprint manager to use
		*  @param[in] ostream
		*    Output stream to serialize the material blueprint into
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		virtual void serialize(const MaterialBlueprint& materialBlueprint, const ShaderBlueprintManager& shaderBlueprintManager, std::ostream& ostream) const = 0;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
