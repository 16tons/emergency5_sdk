// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/WeakPtr.h>
#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class LinkComponent;
}
namespace em5
{
	class TreeComponent;
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
	*    EMERGENCY 5 tree fall down animation
	*/
	class EM5_API_EXPORT TreeFallDownAnimation
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		TreeFallDownAnimation(TreeComponent& treeComponent, qsf::Entity& firefighterEntity);

		/**
		*  @brief
		*    Destructor
		*/
		~TreeFallDownAnimation();

		void start();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateAnimation(const qsf::JobArguments& jobArgs);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum State
		{
			STATE_INIT,
			STATE_WAITING,
			STATE_TREE_FALLING,
			STATE_TREE_CUT
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::WeakPtr<TreeComponent>		 mTreeComponent;
		qsf::WeakPtr<qsf::LinkComponent> mLinkComponent;
		qsf::WeakPtr<qsf::Entity>		 mFirefighter;
		qsf::JobProxy mUpdateJob;

		// State
		State	  mState;
		float	  mSecondsPassed;
		float	  mLastDegree;
		glm::vec3 mFallAxis;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
