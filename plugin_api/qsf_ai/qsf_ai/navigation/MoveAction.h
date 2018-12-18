// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/navigation/Path.h"
#include "qsf_ai/navigation/goal/NavigationGoal.h"

#include <qsf/logic/action/Action.h>
#include <qsf/base/NamedIdentifier.h>

#include <glm/glm.hpp>

namespace qsf
{
	namespace ai
	{
		class NavigationComponent;

		/** Hardcoded QSF standard action type that makes an entity move along a path.
		* It has all the parameters to initiate a movement and may optionally already hold a preplanned path.
		* Otherwise a path is planned dynamically based on the current goal and move action parameters.
		*/
		class QSF_AI_API_EXPORT MoveAction : public Action
		{
		public:
			// Unique action ID
			static const NamedIdentifier ACTION_ID;

			// Default constructor (for using CAMP as a factory and deserialize the action afterwards)
			MoveAction();

			// Initialization needs to be called before the action is started instead of a regular constructor because of unified interface for CAMP export.
			// The move action takes ownership of the goal and / or path
			// Either goal or path should be set, otherwise the action can't do anything.
			void init(NavigationGoal* goal, unsigned int movementModeId = getUninitialized<unsigned int>(), Path* path = nullptr);

			// Configure how a move action should reaction when it notices that it can't reach the desired goal.
			// The hardcoded default it to return a FAIL_PLAN but it might be better to let the following actions decide whether they can still be executed in their preconditions.
			// You should then configure to return an action::RESULT_CONTINUE here
			void setReactionToFailedGoal(action::Result reaction);

			// Action public interface implementation and overrides
			//@{
			virtual action::Result updateAction(const Clock& clock) override;
			virtual bool onStartup() override;
			virtual void onShutdown() override;
			virtual void onPause() override;
			virtual void onResume() override;
			virtual void serialize(BinarySerializer& serializer) override;
			//@}

			inline unsigned int getMovementModeId() const;

		protected:
			// Constructor for sub-classes
			explicit MoveAction(uint32 actionId);
			inline const WeakPtr<NavigationComponent>& getNavigationComponent() const;

			inline std::auto_ptr<NavigationGoal>& getGoal();

		private:
			// What to return if the goal was found to be not achievable, see static setter above.
			action::Result mReactionOnFailedGoal;

			// The goal to reach, may be a nullptr
			std::auto_ptr<NavigationGoal> mGoal;

			// Optional indicator into the global array of movement modes stored at the AI pathfinding navigation system. Uninitialized if not used.
			unsigned int mMovementMode;

			// Will only be set in special circumstances when the path is defined statically for example.
			std::auto_ptr<Path> mPath;

			// Permanent reference to the navigation component; in case this gets null pointer, the action can't continue
			WeakPtr<NavigationComponent> mNavigationComponent;
		};
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::MoveAction)

#include "qsf_ai/navigation/MoveAction-inl.h"
