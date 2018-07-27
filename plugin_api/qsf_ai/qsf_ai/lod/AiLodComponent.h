// Copyright (C) 2012-2017 Promotion Software GmbH


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

namespace qsf
{
	namespace ai
	{
		/** AI Level Of Detail Component class.
		* Provides an entity with an AI LOD interface.
		* If you want to access it from another component attached to this entity
		* consider using the AiLodAccess helper class.
		* A LOD in this context is represented by an integer MIN_LOD <= lod <= MAX_LOD.
		*/
		class QSF_AI_API_EXPORT AiLodComponent : public Component
		{
		public:
			// Unique CAMP Ids for the component, the exported properties and default values for the properties
			//@{
			QSF_DECLARE_CAMP_COMPONENT_ID;
			static const unsigned int LOD_PROPERTY_ID;
			//@}

			// The LOD value is actually a more or less arbitrary unsigned integer, but for convenience at least
			// min and max values are enforced.
			//@{
			static const uint32 MIN_LOD;
			static const uint32 MAX_LOD;
			//@}

		public:
			explicit AiLodComponent(Prototype* prototype);
			virtual ~AiLodComponent();

			// Overridden Component interface
			//@{
			virtual void serialize(BinarySerializer& serializer) override;
			//virtual bool implementsOnComponentPropertyChange() const override;
			//virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;
			//@}

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
			uint32 mLod;

			QSF_CAMP_RTTI();
		};

	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::AiLodComponent);

#include "qsf_ai/lod/AiLodComponent-inl.h"
