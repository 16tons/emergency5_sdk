// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/BoostAssertHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline AmbientAudioManagementComponent::AmbientAudioManagementComponent(Prototype* prototype) :
		Component(prototype)
	{
		// Nothing to do in here
	}

	inline AmbientAudioManagementComponent::~AmbientAudioManagementComponent()
	{
		QSF_ASSERT(mAmbientSoundLookup.empty(), "Ambient sound list should be empty by now", QSF_REACT_NONE);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
