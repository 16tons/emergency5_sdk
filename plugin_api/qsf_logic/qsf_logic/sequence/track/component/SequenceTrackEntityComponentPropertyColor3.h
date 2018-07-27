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

#include <qsf/reflection/type/CampGlmVec3.h>
#include <qsf/reflection/type/CampQsfColor3.h>

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
		class QSF_LOGIC_API_EXPORT SequenceTrackKeyEntityComponentPropertyColor3 : public SequenceTrackKey
		{


		//[-------------------------------------------------------]
		//[ Public data                                           ]
		//[-------------------------------------------------------]
		public:
			Color3	  bezierControlPointIncomingValue;	///< See the class documentation for info about control point values
			glm::vec3 bezierControlPointIncomingTime;	///< See the class documentation for info about control point times
			Color3	  value;
			Color3	  bezierControlPointOutgoingValue;	///< See the class documentation for info about control point values
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
					bool bezierSplitTangentsR;
					bool bezierSplitTangentsG;
					bool bezierSplitTangentsB;
				};
			};


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			explicit SequenceTrackKeyEntityComponentPropertyColor3(uint32 frameNumber) :
				SequenceTrackKey(frameNumber),
				bezierControlPointIncomingTime(0.0f),
				bezierControlPointOutgoingTime(0.0f),
				bezierSplitTangentsR(false),
				bezierSplitTangentsG(false),
				bezierSplitTangentsB(false)
			{}


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};

		/**
		*  @brief
		*    Track type for controlling a qsf::Color3 entity component property
		*/
		class QSF_LOGIC_API_EXPORT SequenceTrackEntityComponentPropertyColor3 : public SequenceTrackEntityComponentProperty
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			SequenceTrackEntityComponentPropertyColor3(Sequence* sequence, uint32 id);
			virtual ~SequenceTrackEntityComponentPropertyColor3();


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
			typedef std::vector<SequenceTrackKeyEntityComponentPropertyColor3*> KeyList;


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
QSF_CAMP_TYPE(qsf::logic::SequenceTrackKeyEntityComponentPropertyColor3)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::logic::SequenceTrackEntityComponentPropertyColor3)