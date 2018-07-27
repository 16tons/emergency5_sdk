// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/plugin/Jobs.h"

#include <qsf/QsfHelper.h>
#include <qsf/base/error/ErrorHandling.h>
#include <qsf/time/HighResolutionStopwatch.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/component/ComponentSystem.h>
#include <qsf/component/factory/PagedComponentFactory.h>
#include <qsf/map/Entity.h>
#include <qsf/log/LogSystem.h>

#include <boost/bind.hpp>


namespace qsf
{
	namespace ai
	{
		template <typename ComponentType, typename AISystem>
		StandardSystem<ComponentType, AISystem>::StandardSystem(Prototype* prototype, const char* name) :
			SystemComponent(prototype, name)
		{}

		template <typename ComponentType, typename AISystem>
		AISystem* StandardSystem<ComponentType, AISystem>::tryGetInstance(Map& map)
		{
			return map.getCoreEntity().getComponent<AISystem>();
		}

		template <typename ComponentType, typename AISystem>
		AISystem& StandardSystem<ComponentType, AISystem>::getInstance(Map& map)
		{
			AISystem* instance = tryGetInstance(map);

			// Unable to use getName() in static function
			QSF_CHECK(instance, "Unable to get singleton instance of " << AISystem::NAME << " from core entity",
				QSF_REACT_THROW);

			return *instance;
		}

		template <typename ComponentType, typename AISystem>
		AISystem* StandardSystem<ComponentType, AISystem>::tryGetInstance()
		{
			return tryGetInstance(QSF_MAINMAP);
		}

		template <typename ComponentType, typename AISystem>
		AISystem& StandardSystem<ComponentType, AISystem>::getInstance()
		{
			return getInstance(QSF_MAINMAP);
		}

		template <typename ComponentType, typename AISystem>
		AISystem* StandardSystem<ComponentType, AISystem>::tryGetInstance(const Component& component)
		{
			return tryGetInstance(component.getEntity().getMap());
		}

		template <typename ComponentType, typename AISystem>
		AISystem& StandardSystem<ComponentType, AISystem>::getInstance(const Component& component)
		{
			return getInstance(component.getEntity().getMap());
		}

		template <typename ComponentType, typename AISystem>
		unsigned int StandardSystem<ComponentType, AISystem>::getJobManagerId()
		{
			return Jobs::SIMULATION_AI;
		}

		template <typename ComponentType, typename AISystem>
		void StandardSystem<ComponentType, AISystem>::updateSystem(const JobArguments& jobArguments)
		{
			AISystem& system = getInstance(*this);

			system.updateGlobals(jobArguments);

			#ifdef QSF_PROFILING
				for (ComponentType* c : getRegisteredEntities())
				{
					HighResolutionStopwatch componentWatch;
					try
					{
						const effort::Indicator effort = (c->isActive() && c->isRunning()) ? system.updateComponent(*c, jobArguments) : effort::TRIVIAL;
						mProfilingData.registerComponentUpdate(componentWatch.getElapsed(), effort == effort::REGULAR); // use the effort reported to decide how to log this time for profiling
						if (c->isDebug())
							system.createDebugOutput(*c);
					}
					catch (const std::exception& e)
					{
						QSF_LOG_PRINTS(ERROR, "Updating " << getName() << " failed because of " << e.what());

						// Exception firewall. The individual component updates should not interfere and so we continue with the next one when one individual update fails.
						mProfilingData.registerFailedComponentUpdate();
					}
				}
			#else
				for (ComponentType* c : getRegisteredEntities())
				{
					try
					{
						if (c->isActive() && c->isRunning())
						{
							system.updateComponent(*c, jobArguments);
							if (c->isDebug())
								system.createDebugOutput(*c);
						}
					}
					catch (const std::exception& e)
					{
						QSF_LOG_PRINTS(ERROR, "Updating " << getName() << " failed because of " << e.what());
					}
				}
			#endif
		}

		template <typename ComponentType, typename AISystem>
		void StandardSystem<ComponentType, AISystem>::updateDebug(const JobArguments& jobArguments)
		{
			// Nothing to do
		}

		template <typename ComponentType, typename AISystem>
		const typename StandardSystem<ComponentType, AISystem>::Registration& StandardSystem<ComponentType, AISystem>::getRegisteredEntities() const
		{
			return ComponentMapQuery(getEntity().getMap()).getAllInstances<ComponentType>();
		}

	}
}
