// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/reflection/ExportedProperty.h"
#include <qsf/reflection/CampDefines.h>
#include <qsf/debug/group/DebugGroup.h>
#include <qsf/base/NamedIdentifier.h>
#include <qsf/base/UniqueInstance.h>

namespace qsf
{
	namespace ai
	{
		/**
		* Encapsulates the configurations for all available debugging options inside the AI LevelOfDetail system.
		* Noncopyable by inheritance.
		*/
		class QSF_AI_API_EXPORT AiLodDebugGroup : public DebugGroup, public UniqueInstance<AiLodDebugGroup>
		{
		public:
			// Unique CAMP Ids and initial values
			//@{
			QSF_DECLARE_CAMP_PLUGINABLE_ID;
			//@}

			// Constructor required by DebugGroup interface
			AiLodDebugGroup(DebugGroupManager* dgm);

			QSF_CAMP_RTTI();
		};
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::AiLodDebugGroup);

// inline implementations
#include "qsf_ai/lod/AiLodDebugGroup-inl.h"
