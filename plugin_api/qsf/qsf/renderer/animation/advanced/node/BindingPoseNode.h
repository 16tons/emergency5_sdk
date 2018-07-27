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
	*    Binding pose animation node, node who gives as a binding pose
	*/
	class QSF_API_EXPORT BindingPoseNode : public AnimationNode
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
		explicit BindingPoseNode(AdvancedAnimationComponent& advancedAnimationComponent);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~BindingPoseNode();


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
#include "qsf/renderer/animation/advanced/node/BindingPoseNode-inl.h"
