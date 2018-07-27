// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/node/KeyFrameAnimationNode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Key frame animation node, it will read a key frame skeleton file and add the interpolated offset to a skeleton
	*/
	class QSF_API_EXPORT KeyFrameAnimationOffsetNode : public KeyFrameAnimationNode
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] advancedAnimationComponent
		*    A reference to an advanced animation component where this animation node lies
		*/
		explicit KeyFrameAnimationOffsetNode(AdvancedAnimationComponent& advancedAnimationComponent);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~KeyFrameAnimationOffsetNode();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::AnimationNode methods          ]
	//[-------------------------------------------------------]
	protected:
		virtual void calculate() override;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/node/KeyFrameAnimationOffsetNode-inl.h"
