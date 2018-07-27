// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/node/AdditiveBlendingNode.h"
#include "qsf/renderer/animation/advanced/BoneMaskHandle.h"


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
	class QSF_API_EXPORT MaskedAdditiveBlendingNode : public AdditiveBlendingNode
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
		explicit MaskedAdditiveBlendingNode(AdvancedAnimationComponent& advancedAnimationComponent, uint16 numberOfInputSlots);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MaskedAdditiveBlendingNode();

		/**
		*  @brief
		*    Return bone mask
		*
		*  @param[in] inputId
		*    ID of input slots
		*  @return
		*    bone mask handle, not valid handle for invalid inputIds
		*/
		inline BoneMaskHandle getBoneMask(uint16 inputId) const;

		/**
		*  @brief
		*    Set the bone mask of an input slot
		*
		*  @param[in] inputId
		*    ID of input slots, nothing is done with invalid inputIds
		*  @param[in] boneMask
		*    bone mask handle, invalid handle will remove a bone mask
		*/
		void setBoneMask(uint16 inputId, BoneMaskHandle& boneMask);


		//[-------------------------------------------------------]
		//[ Protected methods from qsf::AdditiveBlendingNode      ]
		//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Give back the weighting of a bone of an input
		*/
		virtual float getBoneWeightForCalculation(uint16 inputId, uint16 boneId);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:

		// Input Parameter
		BoneMaskHandle				mBoneMasks[MAXIMAL_NUMBER_OF_INPUTS];	// The bone mask for every input

	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/node/MaskedAdditiveBlendingNode-inl.h"
