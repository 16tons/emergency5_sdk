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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class File;
}


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
		*    OpenAL Wav stream class
		*/
		class OpenALStreamWav : public OpenALStream, public FastPodAllocator<OpenALStreamWav>
		{


		//[-------------------------------------------------------]
		//[ Public structures                                     ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Header of the WAV-file format
			*/
			typedef struct
			{
				ALubyte	 riff[4];
				ALsizei	 riffSize;
				ALubyte	 wave[4];
				ALubyte	 fmt[4];
				ALuint	 fmtSize;
				ALushort format;
				ALushort channels;
				ALuint	 samplesPerSec;
				ALuint	 bytesPerSec;
				ALushort blockAlign;
				ALushort bitsPerSample;
				ALubyte	 data[4];
				ALuint	 dataSize;
			} Header;


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
			OpenALStreamWav(ALuint openALSource, const OpenALAudioBuffer& openALAudioBuffer);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~OpenALStreamWav();


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
			*
			*  @return
			*    "true" if all went fine, else "false" (end of stream reached?)
			*/
			bool fillOpenALBuffer(ALuint openALBuffer);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			File*		 mFile;					///< Used on file streaming, can be a null pointer, destroy the instance if you no longer need it
			const uint8* mData;					///< Used on memory streaming, can be a null pointer, do not destroy the data because we just share it
			ALuint		 mStreamSize;			///< Stream size in bytes
			ALuint		 mStreamPosition;		///< Current stream position
			ALenum		 mFormat;				///< Format
			ALuint		 mFrequency;			///< Frequency
			ALuint		 mOpenALBuffers[2];		///< Back and front OpenAL buffer
			Time		 mBuffersBaseTime[2];	///< Base times of the front and back buffers, respectively
			uint32		 mSwapSize;				///< Swap buffer size in bytes
			uint8*		 mSwap;					///< Buffer used within "qsf::openal::OpenALStreamWav::fillOpenALBuffer()", can be a null pointer, destroy the buffer if you no longer need it


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // openal
} // qsf
