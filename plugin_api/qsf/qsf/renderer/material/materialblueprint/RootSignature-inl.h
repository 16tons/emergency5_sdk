// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline RootSignature::RootSignature()
	{
		initializeRendererRootSignature();
	}

	inline RootSignature::~RootSignature()
	{
		// Nothing here
	}

	inline const Renderer::RootSignature& RootSignature::getRendererRootSignature() const
	{
		return mRendererRootSignature;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
