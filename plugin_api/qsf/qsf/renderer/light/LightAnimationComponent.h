// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/renderer/map/JobProxyMovableObjectRenderingListener.h"

#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Light animation component class
	*/
	// TODO(sw) This shouldn't be in qsf move it to another project (e.g. qsf_logic), but currently this component is heavily used.
	// But without a proper update strategy when moving components in different libraries/namespaces we cannot move it yet.
	class QSF_API_EXPORT LightAnimationComponent : public Component, public JobProxyMovableObjectRenderingListener
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Light type
		*/
		enum AnimationType
		{
			PULSE		= 0,	///< Pulsing light (simple sinwave pulsing)
			FLICKER		= 1,	///< Flickering light
			SWITCHING	= 2,	///< Light switching between on and off
			DEFECTIVE	= 3,	///< Flickering defective light
			CUSTOM		= 4		///< Custom on/off interval settings
		};


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;					///< "qsf::LightAnimationComponent" unique component ID
		static const uint32 ANIMATION_TYPE;					///< "AnimationType" unique class property ID inside the class
		static const uint32 ANIMATION_CYCLE;				///< "AnimationCycle" unique class property ID inside the class
		static const uint32 MIN_INTENSITY;					///< "MinIntensity" unique class property ID inside the class
		static const uint32 FADE;							///< "Fade" unique class property ID inside the class
		static const uint32 CUSTOM_ANIMATION_VALUES;		///< "CustomAnimationValues" unique class property ID inside the class
		static const uint32 CUSTOM_ANIMATION_START_OFFSET;	///< "CustomAnimationStartOffset" unique class property ID inside the class
		static const uint32 RANDOM_START;					///< "RandomStart" unique class property ID inside the class
		static const uint32 RANDOM_START_DELAY;				///< "RandomStartDelay" unique class property ID inside the class
		static const uint32 START_DELAY;					///< "StartDelay" unique class property ID inside the class
		static const uint32 INVISIBLE_ANIMATION;			///< "InvisibleAnimation" unique class property ID inside the class


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void setLightActive(Prototype& prototype, bool active, bool setPropertyOverrideFlag = false);


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
		explicit LightAnimationComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~LightAnimationComponent();

		//[-------------------------------------------------------]
		//[ Type                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the animation type
		*/
		inline AnimationType getAnimationType() const;

		/**
		*  @brief
		*    Set the animation type
		*
		*  @param[in] animationType
		*    The animation type
		*/
		inline void setAnimationType(AnimationType animationType);

		/**
		*  @brief
		*    Return the animation cycle in seconds
		*/
		inline float getAnimationCycle() const;

		/**
		*  @brief
		*    Set the animation cycle
		*
		*  @param[in] animation cycle
		*    The animation cycle in seconds
		*/
		inline void setAnimationCycle(float animationCycle);

		/**
		*  @brief
		*    Return the minimal intensity factor
		*/
		inline float getMinIntensity() const;

		/**
		*  @brief
		*    Set the minimal intensity factor
		*
		*  @param[in] minIntensity
		*    The minimal Intensity
		*/
		void setMinIntensity(float minIntensity);

		/**
		*  @brief
		*    Return if the light gets faded in/out
		*/
		inline bool getFade() const;

		/**
		*  @brief
		*    Set if the light should be faded in/out
		*
		*  @param[in] fade
		*    "true" when fading should be used otherwise "false"
		*/
		inline void setFade(bool fade);

		/**
		*  @brief
		*    Returns the custom animation values (intensity values separate by space)
		*
		*  @return
		*    A space separated list of intensity values
		*/
		std::string getCustomAnimationValues() const;

		/**
		*  @brief
		*    Set the custom animation values (intensity values separate by space)
		*
		*  @param[in] animationValues
		*    A space separated list of intensity values
		*/
		void setCustomAnimationValues(const std::string& animationValues);

		/**
		*  @brief
		*    Returns the offset from which the custom animation should start (relative to the animation values)
		*
		*  @return
		*    The offset for the custom animation
		*/
		inline int getCustomAnimationStartOffset() const;

		/**
		*  @brief
		*    Set the offset from which the custom animation should start (relative to the animation values)
		*
		*  @param[in] offset
		*    The offset for the custom animation
		*/
		inline void setCustomAnimationStartOffset(int offset);

		/**
		*  @brief
		*    Returns if the start of the animation should be randomly (a value between 0% - 100% of the animation cycle)
		*
		*  @return
		*    "true" for random start is active otherwise "false"
		*/
		inline bool getRandomStart() const;

		/**
		*  @brief
		*    Set the if the start of the animation should be a random value (a value between 0% - 100% of the animation cycle)
		*
		*  @param[in] randomStart
		*    "true" when random start should be used otherwise "false"
		*/
		inline void setRandomStart(bool randomStart);

		/**
		*  @brief
		*    Returns if the start of the animation should be delayed randomly (a value between 0% - 100% of the animation cycle)
		*
		*  @return
		*    "true" for random start delay is active otherwise "false"
		*/
		inline bool getRandomStartDelay() const;

		/**
		*  @brief
		*    Set the if the start of the animation should be delayed by a random value (a value between 0% - 100% of the animation cycle)
		*
		*  @param[in] randomStartDelay
		*    "true" when random start delay should be used otherwise "false"
		*/
		inline void setRandomStartDelay(bool randomStartDelay);

		/**
		*  @brief
		*    Returns the start delay value (a value between 0.0f - 1.0f a percentage value of the animation cycle)
		*
		*  @return
		*    Delay value
		*/
		inline float getStartDelay() const;

		/**
		*  @brief
		*    Set the start delay value (a value between 0.0f - 1.0f a percentage value of the animation cycle)
		*
		*  @param[in] startDelay
		*    Start delay value (in range 0.0f - 1.0f)
		*/
		inline void setStartDelay(float startDelay);

		/**
		*  @brief
		*    Returns whether or not to animate also invisible objects
		*
		*  @return
		*    "true" to animate invisible objects as well, else "false"
		*/
		inline bool getInvisibleAnimation() const;

		/**
		*  @brief
		*    Set whether or not to animate also invisible objects
		*
		*  @param[in] invisibleAnimation
		*    "true" to animate invisible objects as well, else "false"
		*/
		void setInvisibleAnimation(bool invisibleAnimation);

		/**
		*  @brief
		*    Set if the component should not register at the ObjectRenderingListener
		*
		*  @param[in] registerAtObjectRenderingListener
		*    Registers at the object rendering listener ("true") or not ("false") and doing the startup/shutdown of the update job itself
		*
		*  @note
		*    - Use it only rarely because when disabling the registration for many instance the rendering performance might decrease
		*    - When registration is disabled the component gets job proxy updates also when not visible -> can cause updates to dynamic lights/flares which is a performance hog if many lights/flares exists
		*/
		// TODO(sw): Replace this with a method which tracks how long the component was sleeping and on reactivation skip those time parts
		inline void setRegisterAtObjectRenderingListener(bool registerAtObjectRenderingListener);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf:: LightAnimationComponent methods ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*   Sets the active state of the controlled light
		*
		*  @param[in] active
		*    Active state of the light
		*/
		virtual void setLightActive(bool active);

		/**
		*  @brief
		*   Sets the intensity factor value of the controlled light
		*
		*  @param[in] intensity
		*    Intensity factor of the light
		*/
		virtual void setLightIntensityFactor(float intensity);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::JobProxyMovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onUpdateMovableObjectRenderingJob(const JobArguments& jobArguments) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual Component* getMovableObjectRenderingListenerComponent() override;
		virtual void registerMovableObjectRenderingListener() override;
		virtual void unregisterMovableObjectRenderingListener() override;
		virtual void onObjectRenderingShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool implementsOnComponentPropertyChange() const override  { return true; }
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;
		virtual void onSetActive(bool active) override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onSetSimulating(bool simulating) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*   Linear interpolation of a vector array
		*
		*  @param[in] vector
		*    Float vector
		*  @param[in] value
		*    Value between 0 and 1
		*/
		float lerpVector(const std::vector<float>& vector, float value);

		/**
		*  @brief
		*   Called when doing fade in/out of the light
		*/
		void onLightFade(const JobArguments& jobArguments);

		/**
		*  @brief
		*   Setups the values for fading
		*/
		void setupFadeValues();

		void setupCustomAnimationValueData();
		float getIntensityFactorForCustom(const Time& timePassed);
		void calculateStartDelay();
		void startupAnimation();
		void shutdownAnimation();
		void onUpdateAnimation(const JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		AnimationType		mAnimationType;								///< The type of animation
		float				mAnimationCycle;							///< The pulse speed
		float				mMinIntensity;								///< Minimum intensity
		bool				mFade;										///< Indicates of the light should be fade in/out on activation/deactivation
		std::vector<float>	mCustomAnimationValues;						///< Holds a list of intensity factor values used by the custom animation type, each value represents a time slot
		int					mCustomAnimationStartOffset;				///< The offset for the first active time slot on activation/change
		bool				mRandomStart;								///< Indicates if the start has random value (percentage value from the animation cycle)
		bool				mRandomStartDelay;							///< Indicates if the start should be delayed random value (percentage value from the animation cycle)
		float				mAnimationCycleDelay;						///< Delay value before the animation starts
		bool				mInvisibleAnimation;						///< If "true", do also animate in case the animated object is invisible, else "false"

		// Internal
		bool				mDoingFade;									///< Indicates if we are doing a light fading (in or out)
		Time				mFadeTime;									///< The time how long the fading should take
		Time				mRemainingFadeTime;							///< Remaining time for the fading
		float				mFadeIntensityFactor;						///< The factor which gets multiplied to the intensity for fading
		float				mCyclePassed;								///< Time passed since the start
		JobProxy			mInvisibleAnimationJobProxy;
		// Custom animation
		Time				mCustomAnimationTimeSlotTime;				///< Time how long a time slot is
		Time				mCustomAnimationRemainingTimeSlotTime;		///< Remaining time for the active time slot
		size_t				mActiveTimeSlot;							///< Holds the index of the active time slot
		float				mCurrentCustomIntensityFactor;
		// Delay
		Time				mStartDelayTime;							///< The time how long the animation start should be delayed
		Time				mRemainingStartDelayTime;					///< The remaining time how long the animation start should be delayed
		bool				mRegisterAtMovableObjectRenderingListener;	///< Indicates if the base methods for the "qsf::MovableObjectRenderingListener" should be called or not


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/light/LightAnimationComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::LightAnimationComponent)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::LightAnimationComponent::AnimationType)
