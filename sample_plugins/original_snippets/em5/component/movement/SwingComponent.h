// Copyright (C) 2012-2017 Promotion Software GmbH


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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class MovableComponent;
}


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
	*    EMERGENCY 5 swing component class manipulating the "qsf::MovableComponent" instance of the same entity
	*/
	class EM5_API_EXPORT SwingComponent : public qsf::Component, public qsf::JobProxyMovableObjectRenderingListener
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "em5::SwingComponent" unique component ID
		static const uint32 ANIMATION_TYPE;		///< "AnimationType" unique class property ID inside the class
		static const uint32 ANIMATION_CYCLE;	///< "AnimationCycle" unique class property ID inside the class
		static const uint32 ANIMATION_INTENSITY;///< "AnimationIntensity" unique class property ID inside the class
		static const uint32 VELOCITY_SKEW;		///< "VelocitySkew" unique class property ID inside the class

		static const float MINIMUM_SQUARED_VELOCITY;	///< Minimum squared speed for propeller particle effects
		static const float ACCELERATION_SPEED;			///< Maximum acceleration speed

		// TODO(co) Lookout! Violation of the constant naming convention. Sadly, already edited so we can't change it in here.
		enum AnimationType
		{
			PULSE = 0,
			BOATBOBBING,
			HELICOPTERSWING,
			CARACCELERATION
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
		explicit SwingComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~SwingComponent();

		/**
		*  @brief
		*    Get/Set the animation type
		*/
		inline AnimationType getAnimationType() const;
		inline void setAnimationType(AnimationType animationType);

		/**
		*  @brief
		*    Get/Set the animation cycle in seconds
		*/
		inline float getAnimationCycle() const;
		inline void setAnimationCycle(float animationCycle);

		/**
		*  @brief
		*    Get/Set the animation intensity factor
		*/
		inline float getAnimationIntensity() const;
		inline void setAnimationIntensity(float animationIntensity);

		/**
		*  @brief
		*    Get/Set if velocity will skew the unit; also used for acceleration!
		*/
		inline float getVelocitySkew() const;
		inline void setVelocitySkew(float velocitySkew);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetActive(bool active) override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual qsf::Component* getMovableObjectRenderingListenerComponent() override;
		virtual void onObjectRenderingStartup() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::JobProxyMovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onUpdateMovableObjectRenderingJob(const qsf::JobArguments& jobArguments) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void registerJobProxy();
		float getVelocity(const qsf::MovableComponent& movableComponent) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		AnimationType	mAnimationType;			///< The type of animation
		float			mAnimationCycle;		///< The pulse speed
		float			mAnimationIntensity;	///< Animation intensity
		float			mVelocitySkew;			///< Velocity skewness (also used for acceleration)

		// Internal
		float								mCyclePassed;		///< Time passed since the start
		float								mOldSkew;			///< Value to smooth skewness
		qsf::WeakPtr<qsf::MovableComponent> mMovableComponent;


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/movement/SwingComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::SwingComponent)
QSF_CAMP_TYPE_NONCOPYABLE(em5::SwingComponent::AnimationType)
