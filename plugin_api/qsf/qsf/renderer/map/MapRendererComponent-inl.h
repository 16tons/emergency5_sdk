// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MapRendererComponent::MapRendererComponent(Prototype* prototype) :
		Component(prototype),
		mCellsPerEdge(16)
	{
		// Nothing to do in here
	}

	inline MapRendererComponent::~MapRendererComponent()
	{
		// Nothing to do in here
	}

	inline uint32 MapRendererComponent::getCellsPerEdge() const
	{
		return mCellsPerEdge;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
