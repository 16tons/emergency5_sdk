// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/video/VideoCodec.h"

#include <glm/glm.hpp>

#include <ogg/ogg.h>

#include <theora/theora.h>
#include <theora/codec.h>
#include <theora/theoradec.h>

#include <vorbis/codec.h>


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
	*    Abstract video buffer base class
	*/
	class QSF_API_EXPORT TheoraVideoCodec : public VideoCodec
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		TheoraVideoCodec();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TheoraVideoCodec();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::VideoCodec methods                ]
	//[-------------------------------------------------------]
	public:
		// Reset
		virtual void reset() override;

		// Streaming update
		virtual bool updateStreaming(bool ignoreVideo = false, bool ignoreAudio = false, bool enableVideoFrameskip = false) override;

		// Seeking
		virtual void seek(float time) override;

		// Query position
		virtual float getVideoPosition() override;
		virtual float getAudioPosition() override;
		virtual float getFilePosition() override;

		// Query streaming
		virtual bool isStreaming() const override { return mIsStreaming; }


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void cleanupVorbisStreaming();
		void cleanupTheoraStreaming();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::VideoCodec methods             ]
	//[-------------------------------------------------------]
	protected:
		virtual LoadingError startVideoStreamingImpl(std::istream* istream) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		uint64 bufferData();
		LoadingError openStreams(std::istream* istream);
		int decodeTheoraPacket(ogg_packet& packet, bool enableFrameSkip);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool			mIsStreaming;
		std::istream*	mInputStream;
		ogg_int64_t		mTheoraGranulePos;	///< "Cursor" position of the theora decoder
		ogg_int64_t		mVorbisGranulePos;	///< "Cursor" position of the vorbis decoder
		int32			mTheoraMaxPPLevel;	///< Maximum available theora post-processing level

		// Data structures needed by vorbis/theora decoders and the ogg container
		ogg_sync_state		mSyncState;
		ogg_stream_state*	mTheoraStreamState;
		ogg_stream_state*	mVorbisStreamState;
		th_info*			mTheoraInfo;
		th_comment*			mTheoraComment;
		th_dec_ctx*			mTheoraDecoderContext;
		vorbis_info*		mVorbisInfo;
		vorbis_comment*		mVorbisComment;
		vorbis_dsp_state*	mVorbisDspState;
		vorbis_block*		mVorbisBlock;

		uint32				mIntermediateFramesProcessedSoFar;
		uint32				mLastKeyframeProcessed;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
