// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/Dialog.h"
#include "qsf_editor/dialog/sequence/SequenceMapBackup.h"
#include "qsf_editor/dialog/sequence/SequenceEditorState.h"

#include <qsf/job/JobProxy.h>
#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{
		class Sequence;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Upgrades sequences in-place
		*/
		class SequenceUpgrade
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			static void upgradeSequence(logic::Sequence& sequence);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*   Basically a copy of the main data of SequenceTrackKeyEntityComponentPropertyQuatEuler
			*/
			struct SavedRotationKeyframe
			{
				uint32 frame;
				glm::vec3 value;
				glm::vec3 bezierControlPointIncomingValue;
				glm::vec3 bezierControlPointIncomingTime;
				glm::vec3 bezierControlPointOutgoingValue;
				glm::vec3 bezierControlPointOutgoingTime;

				bool bezierSplitTangentsX;
				bool bezierSplitTangentsY;
				bool bezierSplitTangentsZ;
			};

			typedef std::vector<SavedRotationKeyframe> SavedRotationKeyframeList;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			static void upgradeQuatEulerTrack(const uint32 trackId, logic::Sequence& sequence);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
