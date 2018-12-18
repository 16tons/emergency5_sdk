// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/component/Component.h>
#include <qsf/renderer/map/JobProxyMovableObjectRenderingListener.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 fade effect component
	*/
	class EM5_API_EXPORT FadeEffectComponent : public qsf::Component, public qsf::JobProxyMovableObjectRenderingListener
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::FadeEffectComponent" unique component ID


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
		explicit FadeEffectComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FadeEffectComponent();

		void fadeIn(qsf::Time effectDuration = qsf::Time::fromSeconds(1.0f));

		void fadeOut(qsf::Time effectDuration = qsf::Time::fromSeconds(1.0f), bool deleteOwner = false);

		void init(bool fadeIn = true, qsf::Time effectDuration = qsf::Time::fromSeconds(1.0f), bool deleteOwner = false);

		/**
		*  @brief
		*    Check if the fading effect is running
		*/
		bool isFading() const;

		bool isFadeIn() const;
		const qsf::Time& getFadeTime() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual qsf::Component* getMovableObjectRenderingListenerComponent() override;
		virtual void onObjectRendering(const qsf::Component& component, const Ogre::Camera& ogreCamera) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void activateJob();
		void deactivateJob();
		void updateJob(const qsf::JobArguments& jobArguments);

		void updateFadeIn(const qsf::Time& secondsPassed);
		void updateFadeOut(const qsf::Time& secondsPassed);

		/**
		*  @brief
		*    Set entity visibility
		*
		*  @param[in] visibility
		*    Visibility of entity, 0 .. 1, not visible .. fully visible
		*/
		void setFadeEffect(float visibility);

		void synchronizeInternalAndRendererVisibility();
		void updateMaterialAndInternalFadeAmount(bool enable, float visibility) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::JobProxy	mUpdateJobProxy;
		qsf::Time		mEffectDuration;
		qsf::Time		mRunningTime;
		bool			mDeleteOwner;
		bool			mFadeIn;		///< "true" if the fade in is active, else the fade out effect is active
		// Internal and renderer fade visibility
		float			mInternalVisibility;
		float			mRendererVisibility;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::FadeEffectComponent)
