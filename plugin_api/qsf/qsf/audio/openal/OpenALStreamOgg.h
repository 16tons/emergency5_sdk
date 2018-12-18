// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/audio/openal/OpenALStream.h"
#include "qsf/base/manager/FastPodAllocator.h"

#include <vorbis/vorbisfile.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace openal
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    OpenAL Ogg stream
		*
		*  @remarks
		*    Ogg Vorbis (or just OGG) is an open-source audio compression format similar to MP3.
		*    Actually, the file OGG-file format can contain other things, but let's just assume
		*    it is audio data in here. One of the biggest advantages it has over MP3 is that it
		*    is patent-free. This means you do not need to pay a license fee in order to encode
		*    or decode OGG files.
		*
		*  @todo
		*    - TODO(co) Review background streaming, had to be hacked into for the first EMERGENCY 5 playable
		*/
		class OpenALStreamOgg : public OpenALStream, public FastPodAllocator<OpenALStreamOgg>
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] openALSource
			*    OpenAL audio source using this stream
			*  @param[in] openALAudioBuffer
			*    OpenAL audio buffer this stream is using
			*/
			OpenALStreamOgg(ALuint openALSource, const OpenALAudioBuffer& openALAudioBuffer);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~OpenALStreamOgg();

			void processQueue();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::openal::OpenALStream methods      ]
		//[-------------------------------------------------------]
		public:
			virtual bool init() override;
			virtual bool isInitialized() const override;
			virtual void deInit() override;
			virtual bool update() override;
			virtual Time getActiveBufferBaseTime() const override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void deInitOggStream(bool isCalledFromThread = false);

			/**
			*  @brief
			*    Open the stream
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			bool openStream();

			/**
			*  @brief
			*    Fill an OpenAL buffer
			*
			*  @param[in] openALBuffer
			*    OpenAL buffer to fill
			*  @param[in] zeroReadSizeIsError
			*    Indicates how reading zero bytes from the stream should be treated as an error(true) or not(false)
			*
			*  @return
			*    "true" if all went fine, else "false" (end of stream reached?)
			*/
			bool fillOpenALBuffer(ALuint openALBuffer, bool zeroReadSizeIsError = true);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			OggVorbis_File	   mOggStream;			///< Ogg stream
			vorbis_info*	   mVorbisInfo;			///< Vorbis information, can be a null pointer, don't destroy the instance
			ALenum			   mFormat;				///< Format
			ALuint			   mOpenALBuffers[2];	///< Back and front OpenAL buffer
			Time			   mBuffersBaseTime[2];	///< Base times of the front and back buffers, respectively
			uint32			   mSwapSize;			///< Swap buffer size in bytes
			std::vector<uint8> mSwap;				///< Buffer used within "qsf::openal::OpenALStreamOgg::fillOpenALBuffer()"


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // openal
} // qsf
