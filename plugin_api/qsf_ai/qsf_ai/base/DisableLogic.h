// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"


// File contains simple helpers to disable the whole AI running with an entity en bloc
namespace qsf
{
	class Entity;

	namespace ai
	{
		// Disables all AI logic that is attached to this entity
		void QSF_AI_API_EXPORT disableLogic(Entity& entity);
		// Enable all AI logic that is attached to this entity.
		// Since entities normally start with everything enabled, this is usually done after disabling everything afterwards.
		void QSF_AI_API_EXPORT enableLogic(Entity& entity);
	}
}
