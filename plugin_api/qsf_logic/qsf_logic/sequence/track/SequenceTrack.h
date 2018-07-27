// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_logic/Export.h"

#include <qsf/reflection/object/Object.h>

#include <utility>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
	namespace logic
	{
		class Sequence;
		class SequenceTrackKey;
	}
}


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
		*  @brief
		*    Abstract sequence track base class
		*/
		class QSF_LOGIC_API_EXPORT SequenceTrack : public Object
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class Sequence;


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef std::pair<SequenceTrackKey*, uint32> SequenceTrackKeyWithIndex;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			inline virtual ~SequenceTrack();

			/**
			*  @brief
			*    Return sequence which as ownership of this track. Do not destroy the returned instance!
			*/
			inline Sequence& getSequence() const;
			inline uint32 getId() const;

			inline bool isActive() const;
			inline void setActive(bool active);

			inline const std::string& getName() const;
			inline void setName(const std::string& name);

			inline const std::string& getDescription() const;
			inline void setDescription(const std::string& description);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::logic::SequenceTrack methods      ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Serialize the original value of the controlled thing before the sequence track touched it
			*/
			virtual void serializeOriginalControlledValue(BinarySerializer& serializer) = 0;

			virtual uint32 getKeyCount() const = 0;
			virtual const SequenceTrackKey* getKeyFromIndex(uint32 index) const = 0;
			virtual SequenceTrackKey* getKeyFromIndex(uint32 index) = 0;
			virtual SequenceTrackKeyWithIndex createKeyAtFrame(uint32 frame) = 0;
			virtual void removeKeyAtIndex(uint32 index) = 0;
			virtual void deleteAllKeys() = 0;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::logic::SequenceTrack methods   ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Notify the track of passed time so it can make use of the full time range instead of a single point in time which may not be applicable to all track types
			*
			*  @note
			*    - The default implementation is empty
			*/
			inline virtual void notifyTimePassed(float oldTime, float newTime) const;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			inline SequenceTrack(Sequence* sequence, uint32 id);

			template <typename T>
			bool isInsideKeyInterval(const std::vector<T*>& keys, float oldFrame, float newFrame) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Sequence*	mSequence;	// Owner sequence, must be valid, do not destroy the instance
			uint32		mId;
			bool		mActive;
			std::string	mName;
			std::string	mDescription;


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_logic/sequence/track/SequenceTrack-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::logic::SequenceTrack)
