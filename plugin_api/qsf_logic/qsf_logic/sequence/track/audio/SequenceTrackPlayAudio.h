// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_logic/sequence/track/SequenceTrack.h"
#include "qsf_logic/sequence/track/SequenceTrackKey.h"

#include <qsf/reflection/type/CampUint64.h>	// The 64 bit unsigned integer ID of the sequencer entity component property track class is connected to the CAMP reflection system, so we need this header to tell CAMP about this data type


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		class QSF_LOGIC_API_EXPORT SequenceTrackKeyPlayAudio : public SequenceTrackKey
		{


		//[-------------------------------------------------------]
		//[ Public data                                           ]
		//[-------------------------------------------------------]
		public:
			std::string mAudioLocalAssetName;
			float		mVolume;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			explicit SequenceTrackKeyPlayAudio(uint32 frameNumber) : SequenceTrackKey(frameNumber), mVolume(1.0f) {}


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};

		/**
		*  @brief
		*    Play audio sequence track
		*/
		class QSF_LOGIC_API_EXPORT SequenceTrackPlayAudio : public SequenceTrack
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			SequenceTrackPlayAudio(Sequence* sequence, uint32 id);
			virtual ~SequenceTrackPlayAudio();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::logic::SequenceTrack methods      ]
		//[-------------------------------------------------------]
		public:
			virtual void serializeOriginalControlledValue(BinarySerializer& serializer) override;
			virtual uint32 getKeyCount() const override;
			virtual const SequenceTrackKey* getKeyFromIndex(uint32 index) const override;
			virtual SequenceTrackKey* getKeyFromIndex(uint32 index) override;
			virtual SequenceTrackKeyWithIndex createKeyAtFrame(uint32 frame) override;
			virtual void removeKeyAtIndex(uint32 index) override;
			virtual void deleteAllKeys() override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::logic::SequenceTrack methods   ]
		//[-------------------------------------------------------]
		protected:
			virtual void notifyTimePassed(float oldTime, float newTime) const override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			typedef std::vector<SequenceTrackKeyPlayAudio*> KeyList;
			KeyList mKeys;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(qsf::logic::SequenceTrackKeyPlayAudio)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::logic::SequenceTrackPlayAudio)
