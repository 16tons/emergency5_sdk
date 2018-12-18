// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ShaderPieces::ShaderPieces(ShaderPiecesId shaderPiecesId) :
		ShaderSourceCodeBlueprint(shaderPiecesId)
	{
		// Nothing to do in here
	}

	inline ShaderPieces::~ShaderPieces()
	{
		// Nothing to do in here
	}

	inline ShaderPiecesId ShaderPieces::getShaderPiecesId() const
	{
		return getShaderSourceCodeBlueprintId();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
