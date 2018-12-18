// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_compositing/Export.h"

#include <qsf/base/PropertyHelper.h>
#include <qsf/asset/AssetSystemTypes.h>
#include <qsf/component/Component.h>
#include <qsf/reflection/type/CampQsfColor3.h>

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	namespace v1
	{
		class Entity;
	}
}
namespace qsf
{
	class MeshComponent;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace compositing
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Highlight component to highlight a entity
		*/
		class QSF_COMPOSITING_API_EXPORT HighlightComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::compositing::HighlightComponent" unique component ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] prototype
			*    The prototype this component is in, no null pointer allowed
			*/
			explicit HighlightComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~HighlightComponent();

			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(RimLightEnabled, bool, mRimLightEnabled);
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(RimLightColor, const Color3&, mRimLightColor);

			// The power value indicates how fast the effect will fall off the edges. A value of 0.0 means that the whole mesh is covered. Higher values causes that the effect covers less of the whole mesh and only the edges
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(RimLightPower, float, mRimLightPower);
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(ManualPulseEnabled, bool, mManualPulseEnabled);
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PulseEnabled, bool, mPulseEnabled);
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PulseSpeed, float, mPulseSpeed);
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PulseValue, float, mPulseValue);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::HighlightComponent methods        ]
		//[-------------------------------------------------------]
		public:
			virtual void updateMaterial();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			virtual void onSetSimulating(bool simulating) override;
			virtual void onShutdown() override;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			void updateHighlightMaterial(Ogre::v1::Entity* ogreEntity);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void updateEntityAndLinkedEntities();
			void updateOgreEntityMaterial(Ogre::v1::Entity& ogreEntity);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			GlobalAssetId	mHighlightMaterialName;
			bool			mRimLightEnabled;
			glm::vec3		mRimLightStart;
			glm::vec3		mRimLightEnd;
			Color3			mRimLightColor;
			float			mRimLightPower;
			bool			mPulseEnabled;
			bool			mManualPulseEnabled;
			float			mPulseSpeed;
			float			mPulseValue;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // compositing
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::compositing::HighlightComponent)
