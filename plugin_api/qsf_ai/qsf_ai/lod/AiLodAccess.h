// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/lod/AiLodComponent.h"

namespace qsf
{

	namespace ai
	{
		/** AiLodAccess is a helper class to simplify access to an entity's AiLodComponent.
		* It automates the tedious and slowish task of checking for an AiLodComponent, managing
		* a WeakPtr (unless you prefer lookup-loops), updating that after component-replacement, etc.
		* Usage: just add an instance of this class to the (component-)class that needs to monitor
		* and / or modify the AiLod of its entity.
		*/
		class QSF_AI_API_EXPORT AiLodAccess
		{
		private:
			// No copying allowed.
			//@{
			AiLodAccess(const AiLodAccess& entity);
			AiLodAccess& operator=(const AiLodAccess& src);
			//@}

		public:
			AiLodAccess(const Entity& entity);
			AiLodAccess(AiLodComponent& component);

			// Clears the underlying weak-pointer and turns on automatic pointer-update.
			// This is the same state the class is in after construction through an entity-object.
			void clear();

			// Forces an update on the underlying weak-pointer.
			void update();

			// Check whether there is a wrapped AiLodComponent available.
			// If the class just "lost" a formerly valid pointer then it automatically tries to recover.
			// The methods are still marked const although the internal class state may change.
			// However the public behaviour is const enough to enforce constness through mutables in that case.
			// Return true if the underlying pointer is valid and dereferencing is safe.
			//@{
			bool valid() const;
			operator bool() const;
			//@}

			// Dereference / call methods on the wrapped AiLodComponent.
			// Throws an excpetion if the pointer is invalid.
			//@{
			AiLodComponent& operator*() const;
			AiLodComponent* operator->() const;
			//@}

		public:
			/* The methods in this public section mirror the AiLodComponent's public interface.
			* If there is a AiLodComponent then the calls are forwarded.
			* Otherwise they are either ignored or return meaningful default values.
			*/

			// Get / set the LOD value. The setter clamps the value to MIN_LOD<=lod<=MAX_LOD.
			//@{
			uint32 getLOD() const;
			void setLOD(uint32 lod);
			//@}

			// Returns true if mLod is MAX_LOD.
			bool isMaxLOD() const;

			// Returns true if mLod is MIN_LOD.
			bool isMinLOD() const;

			// Sets the LOD value to MAX_LOD.
			void setMaxLOD();

			// Sets the LOD value to MIN_LOD.
			void setMinLOD();

		private:
			mutable WeakPtr<AiLodComponent> mAiLodComponent;
			const Entity& mEntity;
			mutable bool mCheckIfBecomesNull;
		};

	}
}

#include "qsf_ai/lod/AiLodAccess-inl.h"
