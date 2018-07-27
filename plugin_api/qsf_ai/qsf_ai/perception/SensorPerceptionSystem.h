// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/base/StandardSystem.h"
#include "qsf_ai/perception/SensorComponent.h"

#include <qsf/reflection/CampDefines.h>
#include <qsf/component/Component.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{
		class PerceptionDebugGroup;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Standard-system that manages SensorComponents.
		*/
		class QSF_AI_API_EXPORT SensorPerceptionSystem : public StandardSystem<SensorComponent, SensorPerceptionSystem>
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			QSF_DECLARE_CAMP_COMPONENT_ID;

			// Plain text identifier
			static const char* NAME;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			SensorPerceptionSystem(Prototype* prototype);

			// public System implementation override
			//@{
			virtual bool onStartup() override;
			//@}

			// Required template interface of the StandardSystem
			//@{
			effort::Indicator updateComponent(SensorComponent& sc, const JobArguments& arguments);
			void createDebugOutput(const SensorComponent& sc) const;
			void updateGlobals(const JobArguments&) const;
			//@}


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			PerceptionDebugGroup* mDebugSettings; // pointer to the debug settings, may be null meaning no debugging output should be generated

		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI();

		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/perception/SensorPerceptionSystem-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::SensorPerceptionSystem);
