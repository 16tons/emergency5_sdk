// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/component/Component.h>
#include <qsf/logic/profiling/ProfilingData.h>
#include <qsf/job/JobProxy.h>
#include <qsf/reflection/CampDefines.h>

namespace qsf
{
	class JobArguments;

	namespace ai
	{
		/**
		* @brief abstract super class for systems realized as singleton components.
		*
		* Map specific system functionality can be realized as a component attached to the core component.
		* These components are called system components.
		*
		* @note This was done as an extra layer additional to the ai::StandardSystem to allow moving this back to QSF.
		* If we don't do this it can be merged with the StandardSystem
		*/
		class SystemComponent : public Component
		{
		public:
			QSF_DECLARE_CAMP_COMPONENT_ID;

			// Pass the plain text identifier of the system.
			// This will get hashed to a unique ID for quicker access.
			SystemComponent(Prototype* prototype, const char* name);

			// Access to the plain text name of the SystemComponent
			//@{
			const char* getName() const;
			//@}

		protected:
			// Overridden protected Component interface
			//@{
			virtual bool onStartup() override;
			virtual void onShutdown() override;
			virtual void onSetActive(bool active) override;
			//@}
			virtual unsigned int getJobManagerId() = 0;
			virtual void updateSystem(const JobArguments& jobArguments) = 0;
			virtual void updateDebug(const JobArguments& jobArguments) = 0;

			#ifdef QSF_PROFILING
				// Not serialized by design because it makes more sense to log this per session imo.
				// Thus the component does not need a serialization call at all
				ProfilingData mProfilingData;
			#endif

		private:
			bool registerUpdateJob();
			bool registerDebugJob();

			// Main update function that is passed to the job manager
			void updateJob(const JobArguments& jobArguments);
			void debugJob(const JobArguments& jobArguments);	// Debug job

			const char* mName;
			const unsigned int mHashedId; // unique ID used as logging context

			JobProxy mUpdateJobProxy;
			JobProxy mDebugJobProxy;

			QSF_CAMP_RTTI();
		};
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::SystemComponent);

#include "qsf_ai/base/SystemComponent-inl.h"
