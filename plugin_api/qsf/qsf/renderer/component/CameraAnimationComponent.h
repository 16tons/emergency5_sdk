// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/job/JobProxy.h"
#include "qsf/renderer/component/CameraComponent.h"


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
	*    Camera animation component class
	*/
	class QSF_API_EXPORT CameraAnimationComponent : public Component
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
			SHAKE = 0	///< Pulsing
		};


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "qsf::CameraAnimationComponent" unique component ID
		static const uint32 ANIMATION_TYPE;		///< "AnimationType" unique class property ID inside the class
		static const uint32 ANIMATION_CYCLE;	///< "AnimationCycle" unique class property ID inside the class
		static const uint32 POSITION_INTENSITY;	///< "PositionIntensity" unique class property ID inside the class
		static const uint32 ROTATION_INTENSITY;	///< "RotationIntensity" unique class property ID inside the class
		static const uint32 PLAYTIME;			///< "PlayTime" unique class property ID inside the class


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
		explicit CameraAnimationComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~CameraAnimationComponent();

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
		*    Return the position intensity factor
		*/
		inline float getPositionIntensity() const;

		/**
		*  @brief
		*    Set the position intensity factor
		*
		*  @param[in] intensity
		*    The position intensity
		*/
		inline void setPositionIntensity(float positionIntensity);

		/**
		*  @brief
		*    Return the rotation intensity factor
		*/
		inline float getRotationIntensity() const;

		/**
		*  @brief
		*    Set the rotation intensity factor
		*
		*  @param[in] intensity
		*    The intensity
		*/
		inline void setRotationIntensity(float rotationIntensity);

		/**
		*  @brief
		*    Return the play time of the animation
		*/
		inline const qsf::Time& getPlayTime() const;

		/**
		*  @brief
		*    Set the play time of the animation
		*
		*  @param[in] playTime
		*    The play time: a value of zero means unlimit play time (no automatic stop)
		*/
		inline void setPlayTime(const qsf::Time& playTime);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;
		virtual void onSetActive(bool active) override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
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
		*    Register job proxy
		*/
		void registerJobProxy();

		/**
		*  @brief
		*    Unregister job proxy
		*/
		void unregisterJobProxy();

		/**
		*  @brief
		*    Main update function that is passed to the job manager
		*/
		void updateJob(const JobArguments& jobArguments);

		/**
		*  @brief
		*   Linear interpolation of a vector array
		*
		*  @param[in] vector
		*    float vector
		*  @param[in] value
		*    value between 0 and 1
		*/
		float lerpVector(const std::vector<float>& vector, float value);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		AnimationType	mAnimationType;		///< The type of animation
		float			mAnimationCycle;	///< The pulse speed
		float			mPositionIntensity;	///< Position intensity
		float			mRotationIntensity;	///< Rotation intensity
		qsf::Time		mPlayTime;			///< How long should the animation be run (automatic stop when time is over)

		// Internal
		float			mCyclePassed;		///< Time passed since the start
		JobProxy		mAnimationJobProxy;	///< Regular animation job proxy; for updates once a frame
		qsf::Time		mRemainingPlayTime;
		bool			mLimitedPlayTime;


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
#include "qsf/renderer/component/CameraAnimationComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::CameraAnimationComponent)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::CameraAnimationComponent::AnimationType)
