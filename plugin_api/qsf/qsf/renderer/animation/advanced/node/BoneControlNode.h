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

#include <glm/gtc/quaternion.hpp>


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
	*    Bone control animation node, node to control a single bone
	*/
	class QSF_API_EXPORT BoneControlNode : public AnimationNode
	{


	//[-------------------------------------------------------]
	//[ Public defintion                                      ]
	//[-------------------------------------------------------]
	public:
		enum ControlType
		{
			POSITION_CONTROL = 1 << 0,	// Control the derived position of the bone
			ROTATION_CONTROL = 1 << 1	// Control the derived rotation of the bone
		};
		typedef BitFlagSet<uint8, ControlType> ControlBitmask;


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
		explicit BoneControlNode(AdvancedAnimationComponent& advancedAnimationComponent);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~BoneControlNode();

		/**
		*  @brief
		*    Return the bone ID of the bone we want to control
		*/
		inline uint16 getBoneId() const;

		/**
		*  @brief
		*    Set the bone ID of the bone we want to control
		*/
		inline void setBoneId(uint16 boneId);

		/**
		*  @brief
		*    Return the bone rotation
		*/
		inline const glm::vec3& getPosition() const;

		/**
		*  @brief
		*    Set the bone rotation
		*/
		inline void setPosition(const glm::vec3& position);

		/**
		*  @brief
		*    Return the bone rotation
		*/
		inline const glm::quat& getRotation() const;

		/**
		*  @brief
		*    Set the bone rotation
		*/
		inline void setRotation(const glm::quat& rotation);

		/**
		*  @brief
		*    Return the control type bitmask, to check what we wanted to control
		*/
		inline const ControlBitmask& getControlType() const;

		/**
		*  @brief
		*    Set the control type bitmask
		*/
		inline void setControlType(const ControlBitmask& controlType);

		/**
		*  @brief
		*    Return weight of blending the kinematic result to our output skeleton
		*/
		inline float getWeight() const;

		/**
		*  @brief
		*    Set the weight to blend kinematic result to our output skeleton
		*
		*  @param[in] weight
		*    Blending weight
		*/
		inline void setWeight(float weight);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::AnimationNode methods          ]
	//[-------------------------------------------------------]
	protected:
		virtual void calculate() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Input parameter
		uint16			mBoneId;
		ControlBitmask	mControlType;
		glm::quat		mRotation;
		glm::vec3		mPosition;
		float			mWeight;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/node/BoneControlNode-inl.h"
