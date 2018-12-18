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

#include <qsf/reflection/type/CampGlmVec3.h>

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
		class QSF_LOGIC_API_EXPORT SequenceTrackKeyEntityComponentPropertyVec3 : public SequenceTrackKey
		{


		//[-------------------------------------------------------]
		//[ Public data                                           ]
		//[-------------------------------------------------------]
		public:
			glm::vec3 bezierControlPointIncomingValue;	///< See the class documentation for info about control point values
			glm::vec3 bezierControlPointIncomingTime;	///< See the class documentation for info about control point times
			glm::vec3 value;
			glm::vec3 bezierControlPointOutgoingValue;	///< See the class documentation for info about control point values
			glm::vec3 bezierControlPointOutgoingTime;	///< See the class documentation for info about control point times

			/**
			*  @remarks
			*    Yeah, unions are a bit ugly, but we need two simple interfaces to the flag:
			*    a) A serializable representation (the 3 individual values)
			*    b) A indexible representation (mainly for the editor and speed-of-access)
			*    And unions make this pretty easy on fixed-size data
			*/
			union
			{
				// Array-readable interface to the tangents (name is required by the Bezier key concept!)
				bool bezierSplitTangents[3];

				// Individual aliases for each field for easy reflection
				struct
				{
					bool bezierSplitTangentsX;
					bool bezierSplitTangentsY;
					bool bezierSplitTangentsZ;
				};
			};


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			explicit SequenceTrackKeyEntityComponentPropertyVec3(uint32 frameNumber) :
				SequenceTrackKey(frameNumber),
				bezierControlPointIncomingTime(0.0f),
				bezierControlPointOutgoingTime(0.0f),
				bezierSplitTangentsX(false),
				bezierSplitTangentsY(false),
				bezierSplitTangentsZ(false)
			{}


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};

		/**
		*  @brief
		*    Track type for controlling a glm::vec3 entity component property
		*/
		class QSF_LOGIC_API_EXPORT SequenceTrackEntityComponentPropertyVec3 : public SequenceTrackEntityComponentProperty
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			SequenceTrackEntityComponentPropertyVec3(Sequence* sequence, uint32 id);
			virtual ~SequenceTrackEntityComponentPropertyVec3();


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
			typedef std::vector<SequenceTrackKeyEntityComponentPropertyVec3*> KeyList;


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
QSF_CAMP_TYPE(qsf::logic::SequenceTrackKeyEntityComponentPropertyVec3)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::logic::SequenceTrackEntityComponentPropertyVec3)
