// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/base/PropertyHelper.h>
#include <qsf/component/Component.h>
#include <qsf/math/Color4.h>
#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class SceneNode;
	namespace v1
	{
		class BillboardSet;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Ground component base class
		*/
		class QSF_GAME_API_EXPORT GroundMarkerComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::game::GroundMarkerComponent" unique component ID
			static const std::string MATERIAL_BOX;
			static const std::string MATERIAL_CIRCLE;

			enum GroundMarkerType
			{
				GROUND_MARKER_TYPE_BOX,
				GROUND_MARKER_TYPE_CIRCLE
			};


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
			explicit GroundMarkerComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~GroundMarkerComponent();

			QSF_DEFINE_SIMPLE_GETTER(Scale, float, mScale);
			void setScale(float scale);

			QSF_DEFINE_SIMPLE_GETTER(Type, GroundMarkerType, mType);
			void setType(GroundMarkerType type);

			QSF_DEFINE_SIMPLE_GETTER(GroundMarkerColor, Color4, mGroundMarkerColor);
			void setGroundMarkerColor(const Color4& color);
			void setGroundMarkerFadeAlpha(float alpha);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			virtual bool implementsOnComponentPropertyChange() const override  { return true; }
			virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags) override;
			virtual bool onStartup() override;
			virtual void onShutdown() override;
			virtual void onSetSimulating(bool simulating) override;
			virtual void onSetActive(bool active) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::game::GroundMarkerComponent methods ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Get the rotation offset (in radians); the default implementation returns zero
			*/
			virtual float getRotationOffset() const;

			/**
			*  @brief
			*    Request an marker update
			*
			*  @note
			*    - The default implementation is empty
			*/
			virtual void onMarkerUpdate(const JobArguments& jobArguments);


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			void updateRotation();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void updateAnimation(const JobArguments& jobArguments);
			void setGroundMarkerActive(bool enable);
			void updateTransformation();
			void updateColor();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ogre::SceneNode*		mOgreSceneNode;
			Ogre::v1::BillboardSet*	mOgreBillboardSet;
			bool					mGroundMarkerEnabled;
			Color4					mGroundMarkerColor;
			float					mFadeAlpha;				///< Additional alpha multiplier applied to ground marker color
			GroundMarkerType		mType;
			float					mScale;

			// Job proxies for regular update
			JobProxy				mAnimationJobProxy;		///< Regular animation job proxy; for updates once a frame


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::GroundMarkerComponent)
