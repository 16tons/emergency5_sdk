// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf_logic/sequence/SequencePlayer.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{
		class SequenceTrack;
		class SequenceTrackPathMovement;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 sequence player class
	*/
	class EM5_API_EXPORT SequencePlayer : public qsf::logic::SequencePlayer
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SequencePlayer();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SequencePlayer();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::logic::SequencePlayer methods  ]
	//[-------------------------------------------------------]
	protected:
		virtual void onStartSequence() override;
		virtual void onEndSequence() override;
		virtual void onNotifyTimePassed(float oldTime, float newTime) override;


	//[-------------------------------------------------------]
	//[ Private declarations                                  ]
	//[-------------------------------------------------------]
	private:
		struct PathMovementPlaybackData
		{
			const qsf::logic::SequenceTrackPathMovement* mTrack;
			bool mActive;
			bool mMovableWasActive;
			bool mAiNavigationWasActive;
		};
		typedef std::list<PathMovementPlaybackData> PathMovementPlaybackList;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void checkPathMovementPlayback(PathMovementPlaybackData& playbackData, bool shouldBeActive);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PathMovementPlaybackList mPathMovementPlaybackList;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
