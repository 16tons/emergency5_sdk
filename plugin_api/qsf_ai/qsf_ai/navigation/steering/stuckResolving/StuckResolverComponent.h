// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/navigation/steering/stuckResolving/StuckResolveStage.h"
#include "qsf_ai/navigation/NavigationComponent.h"
#include "qsf_ai/reflection/ExportedProperty.h"

#include "qsf/renderer/map/JobProxyMovableObjectRenderingListener.h"
#include <qsf/reflection/CampDefines.h>
#include <qsf/component/Component.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/base/WeakPtr.h>


namespace qsf
{
	namespace ai
	{
		/**
		* Component to help getting stuck entities moving again.
		*/
		class QSF_AI_API_EXPORT StuckResolverComponent : public Component, public JobProxyMovableObjectRenderingListener
		{
		public:
			QSF_DECLARE_CAMP_COMPONENT_ID;
			static const ExportedProperty<Time> STAGE_SWITCH_DELAY;
			// if set to true, the stuck resolving system will regard waiting before a traffic light (or somebody who does) as being stuck, in order to escape the situation
			static const ExportedProperty<bool> REGARD_TRAFFIC_IMPEDIMENTS_AS_STUCK;
			static const ExportedProperty<bool> OBJECT_CAN_BE_DELETED;

			StuckResolverComponent(Prototype* prototype);

			Time getStageSwitchDelay() const;
			void setStageSwitchDelay(Time delay);

			bool shouldRegardTrafficImpedimentsAsStuck() const;
			void setRegardTrafficImpedimentsAsStuck(bool enable);

			NavigationComponent* tryGetAttachedNavigation() const;
			MovableComponent* tryGetAttachedMovable() const;

			void clearState();

			stuckResolve::Stage getCurrentStage() const;
			void startStage(stuckResolve::Stage stage, Time now);

			stuckResolve::Analysis getCurrentAnalysis() const;
			void setCurrentAnalysis(stuckResolve::Analysis analysis);

			bool hasRunningStage() const;
			bool shouldWaitBeforeTryingNextStage(Time now) const;

			bool getObjectCanBeDeleted() const;
			void setObjectCanBeDeleted(bool objectCanBeDeleted);

			bool getVisibleByCamera() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			virtual bool onStartup() override;
			virtual void onShutdown() override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
		//[-------------------------------------------------------]
		protected:
			virtual Component* getMovableObjectRenderingListenerComponent() override;	// The component we use to check if we are visible
			virtual void onObjectRenderingStartup() override;
			virtual void onObjectRenderingShutdown() override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			QSF_CAMP_RTTI();

			Time mStageSwitchDelay;

			WeakPtr<NavigationComponent> mNavigationComponent;
			WeakPtr<MovableComponent> mMovableComponent;

			stuckResolve::Stage mCurrentStage; // the current measures to apply
			stuckResolve::Analysis mCurrentAnalysis; // the last result, mostly for debugging purposes
			boost::optional<Time> mCurrentStageStartedAt;
			bool mRegardTrafficImpedimentsAsStuck;

			bool mObjectCanBeDeleted;
			bool mVisibleByCamera;
		};
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::StuckResolverComponent);
