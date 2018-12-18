// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_logic/sequence/track/SequenceTrackKey.h"
#include "qsf_logic/sequence/track/component/SequenceTrackEntityComponentProperty.h"

#include <qsf/reflection/type/CampGlmVec2.h>

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
		class QSF_LOGIC_API_EXPORT SequenceTrackKeyEntityComponentPropertyVec2TimeOfDay : public SequenceTrackKey
		{


		//[-------------------------------------------------------]
		//[ Public data                                           ]
		//[-------------------------------------------------------]
		public:
			float bezierControlPointIncomingValue;	///< See the class documentation for info about control point values
			float bezierControlPointIncomingTime;	///< See the class documentation for info about control point times
			float value;
			float bezierControlPointOutgoingValue;	///< See the class documentation for info about control point values
			float bezierControlPointOutgoingTime;	///< See the class documentation for info about control point times
			float bezierSplitTangents;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			explicit SequenceTrackKeyEntityComponentPropertyVec2TimeOfDay(uint32 frameNumber) :
				SequenceTrackKey(frameNumber),
				bezierControlPointIncomingValue(0.0f),
				bezierControlPointIncomingTime(0.0f),
				value(0.0f),
				bezierControlPointOutgoingValue(0.0f),
				bezierControlPointOutgoingTime(0.0f),
				bezierSplitTangents(false)
			{}

			glm::vec2 getValue() const
			{
				const float hour = std::floor(value);
				const float minute = (value - hour) * 60.0f;
				return glm::vec2(hour, minute);
			}

			void setValue(const glm::vec2& time)
			{
				value = time.x + time.y / 60.0f;
			}


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};

		/**
		*  @brief
		*    Track type for controlling a glm::vec3 entity component property
		*/
		class QSF_LOGIC_API_EXPORT SequenceTrackEntityComponentPropertyVec2TimeOfDay : public SequenceTrackEntityComponentProperty
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			SequenceTrackEntityComponentPropertyVec2TimeOfDay(Sequence* sequence, uint32 id);
			virtual ~SequenceTrackEntityComponentPropertyVec2TimeOfDay();


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
			typedef std::vector<SequenceTrackKeyEntityComponentPropertyVec2TimeOfDay*> KeyList;


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
QSF_CAMP_TYPE(qsf::logic::SequenceTrackKeyEntityComponentPropertyVec2TimeOfDay)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::logic::SequenceTrackEntityComponentPropertyVec2TimeOfDay)
