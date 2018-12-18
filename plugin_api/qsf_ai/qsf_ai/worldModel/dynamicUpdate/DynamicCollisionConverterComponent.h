// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"

#include <qsf/component/Component.h>
#include <qsf/reflection/CampDefines.h>
#include <qsf/base/WeakPtr.h>
#include <qsf/math/Transform.h>


namespace qsf
{
	class BulletCollisionComponent;
	class MovableComponent;
	class TransformComponent;
	class ActionComponent;

	namespace ai
	{
		class NavigationComponent;

		/**
		* Technical class that translates collision information into AI navigation map changes.
		* Some entities are completely static and are only written into the navigation map data during map creation time inside the editor.
		* Other entities are always considered dynamic obstacles because they evade for example.
		* This component is for entities that may dynamically fall into one of the two categories and state changes need to be tracked to see whether the category has switched.
		* If this happens their collision information need to be integrated into the navigation map data or removed from it.
		*
		* I decided to make this decision by active polling as there were many differences events I would have to listen to for changes (changes to properties not yet exported to CAMP for example).
		* Of course this could lead to overhead. We will see whether it is noticeable.
		*/
		class QSF_AI_API_EXPORT DynamicCollisionConverterComponent : public Component
		{
		public:
			QSF_DECLARE_CAMP_COMPONENT_ID;

			DynamicCollisionConverterComponent(Prototype* prototype);

			// public component interface implementation
			//@{
			virtual bool onStartup() override;
			virtual void onShutdown() override;
			//@}

			// Calculate whether the entity should currently be treated as static or dynamic with regard to the AI navigation maps
			virtual bool shouldBeTreatedAsStatic() const;
			// Returns whether the entity is currently treated as static - usually the result of the should Currently be treated as static calculation of the last tick
			bool isCurrentlyTreatedAsStatic() const;
			// Set whether the entity is currently treated as static - usually with the result of a current should currently be treated as static calculation
			void setTreatAsStatic(bool enable);

			// Compare with current collision information to see whether there is newer data available that makes updating necessary
			bool needsTransformUpdate() const;
			// Transfer the current collision position to the last static transform.
			// Throws an exception if there is no collision object attached. Should therefore only be called after needsTransformUpdate returned true
			void transferLastStaticTransform();
			// Retrieve the last known transform position where the entity was registered as a static obstacle
			const Transform& getLastStaticTransform() const;

		protected:
			bool mCurrentlyTreatedAsStatic; // The result of the last evaluation of being dynamic or static. Not exported to CAMP because there is no reason to configure it manually
			Transform mLastStaticTransform; // Stores the last transform values with which the entity was registered as static. May be deprecated as long as the entity is currently moving.

			// Pointers to all component that influence the decision whether this entity should be treated as static or dynamic.
			// All are optional and may legally be nullptr
			WeakPtr<BulletCollisionComponent> mCollisionComponent;
			WeakPtr<TransformComponent> mTransformComponent;
			WeakPtr<MovableComponent> mMovableComponent;
			WeakPtr<NavigationComponent> mNavigationComponent;
			WeakPtr<ActionComponent> mActionComponent;

			QSF_CAMP_RTTI();
		};
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::DynamicCollisionConverterComponent);
