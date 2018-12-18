// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/reflection/object/Object.h>


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
		*    Sequence track key class
		*
		*  @note
		*    About Bezier curve control points: The times/values of the control points are interpreted as an offset
		*    to the key time/value. So to calculate the global time of a control point, the key time has to be added
		*    to the control point time first: globalControlPointTime = bezierControlPoint*Time + getTime()
		*    The same is true for the value: globalControlPointValue = bezierControlPoint*Value + value
		*/
		class SequenceTrackKey : public Object
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class CampReflectionPropertyHelperSequenceTrackKey;


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			// Compare methods for sorting and finding
			// @{
			inline static bool compareKeyTimes(const SequenceTrackKey* a, const SequenceTrackKey* b);
			inline static bool compareKeyWithTime(const SequenceTrackKey* a, const uint32& b);
			inline static bool compareTimeWithKey(const uint32& a, const SequenceTrackKey* b);
			// @}


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			inline explicit SequenceTrackKey(uint32 frameNumber);

			/**
			*  @brief
			*    Frame number is readonly; if you need to modify the time of a key, you have to remove and re-insert it into the track
			*/
			inline uint32 getFrameNumber() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint32 mFrameNumber;	///< The position of the key in the timeline; noted as frame number


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
#include "qsf_logic/sequence/track/SequenceTrackKey-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(qsf::logic::SequenceTrackKey)
