// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/base/manager/PagedAllocator.h"

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AudioManager;
}


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
	*    Audio mixer class
	*
	*  @remarks
	*    The audio mixer consists of, like a real audio mixer, different channels. Every channel has
	*    its own volume which can be configured from 0% to 100%. Each channel also has a output volume
	*    which is the result of the channels own volume when it is mixed with all channels onto its way
	*    to the master channel #0. To perform routing, every channel has a "Send to" channel. By default this
	*    is the master channel #0. But more complex mixing can be done by routing channels and thus grouping
	*    channels together and having the power to change the volume of every group individually.
	*    Changes to a channels volume automatic propagate correctly and also do real-time adjustments
	*    of the volume of active sound sources, when the parent audio manager is configured to do so (by default it should be)
	*
	*   @todo
	*     - TODO(co) Code style cleanup
	*/
	class QSF_API_EXPORT AudioMixer
	{


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Mixer channel
		*/
		class MixerChannel
		{
		public:
			typedef std::vector<MixerChannel*> MixerChannelReferences;

		public:
			/**
			*  @brief
			*    Constructs a channel from a channel id and does initial integration
			*    of the channel into the send tree
			*/
			MixerChannel(uint32 id, MixerChannel* sendTo);

			/**
			*  @brief
			*    Destroys the channel and takes care to unregister this channel
			*    from the send tree
			*/
			~MixerChannel();

			/**
			*  @brief
			*    Changes the send-to target channel for this channel. Keeps track
			*    of send tree integrity, e.g. puts the channel into the senders list of the target
			*
			*  @return
			*    Returns "true" when the target mixer channel actually changed, "false" if nothing was changed
			*
			*  @note
			*    - When the target has changed also the output volume is recomputed
			*/
			bool setSendTo(MixerChannel* channel);

			/**
			*  @brief
			*    Adjusts the volume of this channel and recomputes output volumes of all dependent channels
			*
			*  @return
			*    Returns "true" when the volume was actually changed, "false" if no volumes were changed
			*/
			bool adjustVolume(float newVolume);

			/**
			*  @brief
			*    Adjusts the internal volume multiplier of this channel that implicitly affects the this channel's volume
			*/
			void setVolumeMultiplier(float newVolumeMultiplier);

			/**
			*  @brief
			*   Enumerates all channels which output volumes are dependent on the volume of this channel
			*   By definition, any channel itself is also dependent on itself.
			*/
			void enumerateVolumeDependentChannels(std::vector<uint32>& outDependentChannelIndices);

			inline uint32 getId() const				  { return mId; }
			inline float getVolume() const			  { return mVolume; }
			inline float getVolumeMultiplier() const  { return mVolumeMultiplier; }
			inline float getOutputVolume() const	  { return mOutputVolume; }

		private:
			/**
			*  @brief
			*    Removed a channel from the receiver lists and ensures it no longer
			*    points to this channel as the send target
			*/
			void removeFromReceivers(MixerChannel* channel);

			/**
			*  @brief
			*    Recomputes the output volumes of all dependent channels
			*/
			void recomputeOutputVolumes();

		private:
			uint32 mId;						///< Index of this channel
			float mVolume;					///< Volume of this channel alone (e.g. the volume knob of this channel)
			float mVolumeMultiplier;		///< Gets implicitly multiplied on the output volume of this channel
			float mOutputVolume;			///< Computed channel volume, taking into account channel sending

			MixerChannel* mTargetChannel;	///< Index of the mixer channel for "send to". Output volume of the channel is influenced by the output volume of the target channel

			MixerChannelReferences mSendingChannels;	///< List of channels which send to this channel
		};

		typedef boost::container::flat_map<uint32, MixerChannel*> MixerChannelMap;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		AudioMixer(AudioManager& parentManager);

		~AudioMixer();

		float getMixerChannelVolume(uint32 channelIndex) const;
		void setMixerChannelVolume(uint32 channelIndex, float volume);
		float getMixerChannelMixedVolume(uint32 channelIndex) const;

		/**
		*  @brief
		*    Changes the target mixer channel for a mixer channel
		*
		*  @param[in] channel
		*    The id of the mixer channel for which the target mixer channel should be changed
		*  @param[in] targetSendToChannel
		*    The id of the mixer channel which is the new the target mixer channel
		*
		*  @note
		*    - When the target mixer channel wad changed, the output volumes of any audio sources attached to the mixer channel (referenced by channel parameter) are updated to reflect any change which might occur due to the target mixer channel change
		*/
		void setChannelSendTarget(uint32 channel, uint32 targetSendToChannel);

		float getMixerChannelVolumeMultiplier(uint32 channelIndex) const;
		void setMixerChannelVolumeMultiplier(uint32 channelIndex, float volumeMultiplier);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		MixerChannel& getOrCreateChannel(uint32 channelIndex);
		MixerChannel* getChannel(uint32 channelIndex) const;
		void notifyOnMixerChannelVolumeChanged(MixerChannel& channel);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MixerChannelMap	mMixerChannels;		///< Sparse mixer channel list
		AudioManager& mParentAudioManager;
		PagedAllocator<MixerChannel> mMixerChannelAllocator;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
