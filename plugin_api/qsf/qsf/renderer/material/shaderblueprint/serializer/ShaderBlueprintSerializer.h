// Copyright (C) 2012-2017 Promotion Software GmbH


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
	*    Abstract shader blueprint serializer interface
	*
	*  @note
	*    - Must work with exceptions in order to be able to interact with standard C++ streams
	*/
	class QSF_API_EXPORT ShaderBlueprintSerializer : public AssetDataSerializer
	{


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ShaderBlueprintSerializer methods ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Deserialize a shader blueprint by using a given input stream
		*
		*  @param[in] shaderBlueprintManager
		*    Shader blueprint manager to use
		*  @param[in] absoluteFilename
		*    Absolute shader blueprint filename
		*  @param[in] istream
		*    Input stream to deserialize the shader blueprint from
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		virtual void deserialize(ShaderBlueprintManager& shaderBlueprintManager, const std::string& absoluteFilename, std::istream& istream) const = 0;

		/**
		*  @brief
		*    Serialize the shader blueprint into a given output stream
		*
		*  @param[in] shaderBlueprintManager
		*    Shader blueprint manager to use
		*  @param[in] ostream
		*    Output stream to serialize the shader blueprint into
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		virtual void serialize(const ShaderBlueprintManager& shaderBlueprintManager, std::ostream& ostream) const = 0;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
