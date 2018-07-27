// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_logic/sequence/track/SequenceTrackKey.h"
#include "qsf_logic/sequence/track/component/SequenceTrackEntityComponentProperty.h"

#include <vector>


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
		/**
		*  @todo
		*    -TODO(oi) Put key micro-classes into it own files??
		*/
		class QSF_LOGIC_API_EXPORT SequenceTrackKeyEntityComponentPropertyBool : public SequenceTrackKey
		{


		//[-------------------------------------------------------]
		//[ Public data                                           ]
		//[-------------------------------------------------------]
		public:
			bool value;	///< Raw property value


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			explicit SequenceTrackKeyEntityComponentPropertyBool(uint32 frameNumber) :
				SequenceTrackKey(frameNumber),
				value(false)
			{}


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};

		/**
		*  @brief
		*    Track type for controlling a bool entity component property
		*/
		class QSF_LOGIC_API_EXPORT SequenceTrackEntityComponentPropertyBool : public SequenceTrackEntityComponentProperty
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			SequenceTrackEntityComponentPropertyBool(Sequence* sequence, uint32 id);
			virtual ~SequenceTrackEntityComponentPropertyBool();


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
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			typedef std::vector<SequenceTrackKeyEntityComponentPropertyBool*> KeyList;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
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
QSF_CAMP_TYPE(qsf::logic::SequenceTrackKeyEntityComponentPropertyBool)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::logic::SequenceTrackEntityComponentPropertyBool)
