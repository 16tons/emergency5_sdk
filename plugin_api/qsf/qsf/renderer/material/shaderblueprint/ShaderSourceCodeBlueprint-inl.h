// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ShaderSourceCodeBlueprintId ShaderSourceCodeBlueprint::getShaderSourceCodeBlueprintId() const
	{
		return mShaderSourceCodeBlueprintId;
	}

	inline const std::string& ShaderSourceCodeBlueprint::getSourceCodeFilename() const
	{
		return mSourceCodeFilename;
	}

	inline const std::string& ShaderSourceCodeBlueprint::getSourceCode() const
	{
		return mSourceCode;
	}

	inline uint32 ShaderSourceCodeBlueprint::getSourceCodeHash() const
	{
		return mSourceCodeHash;
	}

	inline bool ShaderSourceCodeBlueprint::isShaderPropertyReferenced(ShaderPropertyId shaderPropertyId) const
	{
		return (mReferencedShaderProperties.find(shaderPropertyId) != mReferencedShaderProperties.cend());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
