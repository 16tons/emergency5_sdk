// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <boost/noncopyable.hpp>


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
	*    Audio resource base class
	*/
	class QSF_API_EXPORT AudioResource : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Resource type
		*/
		enum Type
		{
			BUFFER_TYPE = 0,	///< Audio buffer
			SOURCE_TYPE = 1		///< Audio source
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AudioResource();

		/**
		*  @brief
		*    Return the owner audio manager
		*
		*  @return
		*    The owner audio manager, do not destroy the instance
		*/
		inline AudioManager& getAudioManager() const;

		/**
		*  @brief
		*    Return the audio resource type
		*
		*  @return
		*    Audio resource type
		*/
		inline Type getType() const;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] audioManager
		*    Owner audio manager, must stay valid as long as this audio resource instance exists
		*  @param[in] type
		*    Audio resource type
		*/
		AudioResource(AudioManager& audioManager, Type type);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AudioManager* mAudioManager;	///< Owner audio manager, always valid, do not destroy the instance
		Type		  mType;			///< Resource type


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/audio/AudioResource-inl.h"
