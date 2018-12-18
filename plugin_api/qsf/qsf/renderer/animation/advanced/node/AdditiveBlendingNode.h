// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/AnimationNode.h"
#include "qsf/math/BitFlagSet.h"


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
	*    Additive blending animation node, simple blend together different skeletons
	*/
	class QSF_API_EXPORT AdditiveBlendingNode : public AnimationNode
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
		*
		*  @param[in] numberOfInputSlots
		*    Number of input slots between 1 and MAXIMAL_NUMBER_OF_INPUTS it will be corrected with no error !
		*
		*  @note
		*    With 0 input slots we will not create useful output
		*/
		explicit AdditiveBlendingNode(AdvancedAnimationComponent& advancedAnimationComponent, uint16 numberOfInputSlots);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~AdditiveBlendingNode();

		/**
		*  @brief
		*    Return weight of an input skeleton
		*
		*  @param[in] inputId
		*    ID of input slots, return 0 for invalid inputIds
		*/
		inline float getWeight(uint16 inputId) const;

		/**
		*  @brief
		*    Set the weight of an input slot
		*
		*  @param[in] inputId
		*    ID of input slots, nothing is done with invalid inputIds
		*
		*  @param[in] weight
		*    blending weight
		*
		*  @note
		*    If the sum of all weights is not 1.0 it will be changed to 1.0 with "AnimationAverage" set, or the skeletons are blended cumulative if not
		*/
		void setWeight(uint16 inputId, float weight);

		/**
		*  @brief
		*    Return if we want to force the average between skeleton animations by setting the sum of all weights to 1.0
		*/
		inline bool getAnimationAverage() const;

		/**
		*  @brief
		*    Set if we want to force the average between skeletons by setting the sum of all weights to 1.0
		*
		*  @param[in] animationAverage
		*    true if we want to force the average
		*
		*  @note
		*    If all weights are 0.0 animation average will be ignored internally and we will get a zero pose
		*/
		void setAnimationAverage(bool animationAverage);


	//[-------------------------------------------------------]
	//[ Protected methods from qsf::AnimationNode             ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Animation calculation method
		*/
		virtual void calculate();

		/**
		*  @brief
		*    Give back the weighting of a bone of an input
		*/
		virtual float getBoneWeightForCalculation(uint16 inputId, uint16 boneId);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Calculate the sum of all weight and store it (so that we don't need to calculate it every time)
		void calculateWeightSum();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Input parameter
		float mWeights[MAXIMAL_NUMBER_OF_INPUTS];
		bool  mAnimationAverage;

		// Internal data
		float mWeightSum;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/node/AdditiveBlendingNode-inl.h"
