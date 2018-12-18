// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/audio/AudioManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace null
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Null audio backend class
		*/
		class NullAudioManager : public AudioManager
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			NullAudioManager();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~NullAudioManager();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::AudioManager methods              ]
		//[-------------------------------------------------------]
		public:
			virtual std::string getDescription() const override;
			virtual void getFormatList(std::vector<Format>& formats) const override;
			virtual float getVolume() const override;
			virtual void setVolume(float volume) override;
			virtual float getPitch() const override;
			virtual void setPitch(float pitch) override;
			virtual float getDopplerFactor() const override;
			virtual void setDopplerFactor(float factor) override;
			virtual void pauseAllActiveAudioSources() override;
			virtual void resumeAllActiveAudioSources() override;
			virtual void update() override;
			//[-------------------------------------------------------]
			//[ Create audio buffer/source                            ]
			//[-------------------------------------------------------]
			virtual AudioBuffer* createAudioBuffer(const std::string& filename, bool stream = false) override;
			virtual AudioBuffer* createAudioBuffer() override;
			virtual void destroyAudioBuffer(AudioBuffer& audioBuffer) override;
			virtual AudioSource* createAudioSourceWithAudioBuffer(AudioBuffer* audioBuffer = nullptr, bool isSpatial = false) override;
			virtual AudioSource* createAudioSource(const std::string& filename, bool stream = false, bool isSpatial = false) override;
			virtual void destroyAudioSource(AudioSource& audioSource) override;
			virtual bool queryDurationOfAudioAsset(const std::string& audioFile, Time& outDuration) override;
			//[-------------------------------------------------------]
			//[ Listener                                              ]
			//[-------------------------------------------------------]
			virtual const glm::vec3& getListenerAttribute(ListenerAttribute listenerAttribute) const override;
			virtual bool setListenerAttribute(ListenerAttribute listenerAttribute, const glm::vec3& value) override;
			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			virtual bool startup() override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::AudioManager methods           ]
		//[-------------------------------------------------------]
		protected:
			virtual void notifyChannelVolumeChanged(uint32 channelIndex, float newVolume) override;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // null
} // qsf
