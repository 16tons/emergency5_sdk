// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/audio/AudioProxy.h"

#include <qsf/renderer/map/JobProxyMovableObjectRenderingListener.h>
#include <qsf/component/Component.h>
#include <qsf/job/JobProxy.h>


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
	*    EMERGENCY 5 audio component class
	*
	*  @note
	*    - Enables efficient random and looped audio
	*
	*  @todo
	*    - TODO(fw): We should really rename this component (without breaking editing of course),
	*       as it produces a lot of confusion when people mix it up with the "qsf::AudioComponent"
	*/
	class EM5_API_EXPORT AudioComponent : public qsf::Component, public qsf::JobProxyMovableObjectRenderingListener
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "em5::AudioComponent" unique component ID

		enum AudioSet
		{
			AUDIOSET_NONE,
			AUDIOSET_COW_RANDOM,
			AUDIOSET_SKI_LOOPED,
			AUDIOSET_DIVESPOT_LOOPED,
			AUDIOSET_DEMONSTRATION_MOB_HISTORIC,
			AUDIOSET_DEMONSTRATION_MOB_MODERN
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
		inline explicit AudioComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~AudioComponent();

		//[-------------------------------------------------------]
		//[ CAMP getters and setters                              ]
		//[-------------------------------------------------------]
		inline AudioSet getAudioSet() const;
		inline void setAudioSet(AudioSet audioSet);

		inline float getRandomWaitIntervalStart() const;
		inline void setRandomWaitIntervalStart(float randomWaitIntervalStart);

		inline float getRandomWaitIntervalEnd() const;
		inline void setRandomWaitIntervalEnd(float randomWaitIntervalEnd);

		inline float getRandomProbability() const;
		inline void setRandomProbability(float randomProbability);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetActive(bool active) override;
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[---------------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[---------------------------------------------------------------]
	protected:
		virtual Component* getMovableObjectRenderingListenerComponent() override;
		virtual void registerMovableObjectRenderingListener() override;
		virtual void unregisterMovableObjectRenderingListener() override;


	//[-----------------------------------------------------------------------]
	//[ Protected virtual qsf::JobProxyMovableObjectRenderingListener methods ]
	//[-----------------------------------------------------------------------]
	protected:
		virtual void onUpdateMovableObjectRenderingJob(const qsf::JobArguments& jobArguments) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool checkCountdownAndProbability(const qsf::Time& timePassed);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AudioProxy	mAudioProxy;
		qsf::Time	mTimeUntilNextPlayback;

		// Camp export
		AudioSet	mAudioSet;
		float		mRandomWaitIntervalStart;
		float		mRandomWaitIntervalEnd;
		float		mRandomProbability;


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
#include "em5/audio/AudioComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::AudioComponent)
QSF_CAMP_TYPE_NONCOPYABLE(em5::AudioComponent::AudioSet)
