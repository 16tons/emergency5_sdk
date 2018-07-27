// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ShaderBlueprintId ShaderBlueprint::getShaderBlueprintId() const
	{
		return mShaderBlueprintId;
	}

	inline ShaderBlueprint::Type ShaderBlueprint::getType() const
	{
		return mType;
	}

	inline const std::string& ShaderBlueprint::getHlslProfile() const
	{
		return mHlslProfile;
	}

	inline ShaderSourceCodeBlueprint* ShaderBlueprint::getShaderSourceCodeBlueprint() const
	{
		return mShaderSourceCodeBlueprint;
	}

	inline const ShaderBlueprint::ShaderPiecesIds& ShaderBlueprint::getReferencedShaderPiecesIds() const
	{
		return mReferencedShaderPiecesIds;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
