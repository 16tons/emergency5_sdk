// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/StandardSystem.h"
#include "qsf_ai/lod/AiLodComponent.h"

namespace qsf
{
	namespace ai
	{

		class AiLodDebugGroup;

		class QSF_AI_API_EXPORT AiLodSystem : public StandardSystem<AiLodComponent, AiLodSystem>
		{
		public:
			// Unique CAMP Ids for the component, the exported properties and default values for the properties
			//@{
			QSF_DECLARE_CAMP_COMPONENT_ID;
			//@}

			// Plain text identifier
			static const char* NAME;

			explicit AiLodSystem(Prototype* prototype);
			virtual ~AiLodSystem();

			// Overridden Component interface
			//@{
			virtual bool onStartup() override;
			//@}

			// Required template interface of the StandardSystem
			//@{
			effort::Indicator updateComponent(AiLodComponent& lodComponent, const JobArguments& arguments);
			void createDebugOutput(const AiLodComponent& lodComponent) const;
			void updateGlobals(const JobArguments&);
			//@}

		private:
			AiLodDebugGroup* mDebugSettings; // pointer to the debug settings, may be null meaning no debugging output should be generated.

			QSF_CAMP_RTTI();
		};

	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::AiLodSystem);

#include "qsf_ai/lod/AiLodSystem-inl.h"
