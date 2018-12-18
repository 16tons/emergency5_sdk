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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{
		class EActor;
		class EGenericObject;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		class QSF_AI_API_EXPORT RouterComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public static definitions                             ]
		//[-------------------------------------------------------]
		public:
			QSF_DECLARE_CAMP_COMPONENT_ID;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			RouterComponent(Prototype* prototype);
			virtual ~RouterComponent();

			EActor* getActor();
			bool isGenericObject() const;
			EGenericObject* getEGenericObject();
			EGenericObject* tryGetEGenericObject();

			void createCollisionBox();
			void removeCollisionBox();
			void recreateCollisionBox();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			inline virtual bool implementsOnComponentPropertyChange() const override  { return true; }
			virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags) override;
			virtual void onSetSimulating(bool simulating) override;
			virtual void onSetActive(bool active) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			EActor* mActor;		///< Actor instance; managed by this component, so we're responsible for destroying it

			QSF_CAMP_RTTI();


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::RouterComponent)
