// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline SensorPerceptionSystem::SensorPerceptionSystem(Prototype* prototype) :
			StandardSystem<SensorComponent,SensorPerceptionSystem>(prototype, NAME),
			mDebugSettings(nullptr)
		{
		}

		inline void SensorPerceptionSystem::updateGlobals(const JobArguments&) const
		{}

		inline void SensorPerceptionSystem::createDebugOutput(const SensorComponent& sc) const
		{
			// Note:
			// the standard-system calls this method only if sc is in debugging-mode.
			// But our debug-group supports debug-output for ALL entities regardless of their respective isDebug flag!
			// Therefore the initial debugOutput-trigger-code is moved into updateComponent for now.
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
