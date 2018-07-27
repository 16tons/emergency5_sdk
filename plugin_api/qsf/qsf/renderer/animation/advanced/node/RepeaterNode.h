// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/AnimationNode.h"


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
	*    Repeater animation node, node which just repeats a skeleton
	*/
	class QSF_API_EXPORT RepeaterNode : public AnimationNode
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
		explicit RepeaterNode(AdvancedAnimationComponent& advancedAnimationComponent);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~RepeaterNode();


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
#include "qsf/renderer/animation/advanced/node/RepeaterNode-inl.h"
