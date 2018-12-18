// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/SystemComponent.h"

#include <qsf/component/ComponentCollection.h>
#include <qsf/logic/EffortIndicator.h>


namespace qsf
{
	class JobArguments;
	class Map;

	namespace ai
	{
		/** Templated implementation of an typical AI system.
		* An AI system provides update logic for some kind of components.
		* The component type as well as an instance of the inheriting system itself are used as template parameters.
		* This trick is done for technical reasons to avoid virtual function calls inside the main update.
		* All AI systems register themselves at the job manager.
		* Not every AI system needs to inherit from this base class if the internal structure is different.
		* But it is worthwhile to try to avoid exceptions here because otherwise each change would need to be synchronized with the nonstandard systems.
		* Besides the component specific update and debug functions there is a function for updating the core system globally.
		* AI StandardSystems are used like singletons with the one instance being attached to the core entity.
		*/
		template <typename ComponentType, typename AISystem>
		class StandardSystem : public SystemComponent
		{
		public:
			StandardSystem(Prototype* prototype, const char* name);

			// Pseudo singleton access for the instance attached to the core entity.
			static AISystem* tryGetInstance(Map& map);
			static AISystem& getInstance(Map& map);
			static AISystem* tryGetInstance(const Component& component); // accesses the map for that entity
			static AISystem& getInstance(const Component& component); // accesses the map for that entity
			static AISystem* tryGetInstance(); // accesses QSF_MAINMAP
			static AISystem& getInstance(); // accesses QSF_MAINMAP

		protected:
			// overridden SystemComponent interface
			//@{
			virtual unsigned int getJobManagerId() override;
			virtual void updateSystem(const JobArguments& jobArguments) override;
			virtual void updateDebug(const JobArguments& jobArguments) override;
			//@}

		protected:
			// This is the type for the registration container for the components
			typedef ComponentCollection::ComponentList<ComponentType> Registration;
			const Registration& getRegisteredEntities() const;
		};
	}
}

#include "qsf_ai/base/StandardSystem-inl.h"
