// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/shaderblueprint/ShaderSourceCodeBlueprint.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringHash ShaderPiecesId;	///< The shader pieces ID is a result of "qsf::StringHash(<shader pieces name>)"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Static shader pieces
	*
	*  @see
	*    - "qsf::ShaderPiecesManager"
	*/
	class ShaderPieces : public ShaderSourceCodeBlueprint
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ShaderPiecesManager;	///< The shader pieces manager constructs shader pieces and hence needs access to some internals we better don't make public


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] shaderPiecesId
		*    The shader pieces ID
		*/
		inline explicit ShaderPieces(ShaderPiecesId shaderPiecesId);	// TODO(co) Should be private

		/**
		*  @brief
		*    Destructor
		*/
		inline ~ShaderPieces();	// TODO(co) Should be private

		/**
		*  @brief
		*    Return the shader pieces ID
		*
		*  @return
		*    The shader pieces ID
		*/
		inline ShaderPiecesId getShaderPiecesId() const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/shaderblueprint/ShaderPieces-inl.h"
