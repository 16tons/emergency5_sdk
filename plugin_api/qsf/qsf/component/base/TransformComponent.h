// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/reflection/type/CampGlmVec3.h"
#include "qsf/reflection/type/CampGlmQuat.h"
#include "qsf/math/Transform.h"


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
	*    Transform component class
	*/
	class QSF_API_EXPORT TransformComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MovableComponent;		// May access and change mAnimationTransform pointer
		friend class LinkComponent;			// May access and change transformations


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;			///< "qsf::TransformComponent" unique component ID
		static const uint32 POSITION;				///< "Position" unique class property ID inside the class
		static const uint32 ROTATION;				///< "Rotation" unique class property ID inside the class
		static const uint32 SCALE;					///< "Scale" unique class property ID inside the class
		static const uint32 TRANSFORM;				///< "Transform" unique class property ID inside the class
		static const uint32 ANIMATION_TRANSFORM;	///< "AnimationTransform" unique class property ID inside the class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		inline explicit TransformComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TransformComponent();

		/**
		*  @brief
		*    Return the position
		*
		*  @return
		*    The position, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec3& getPosition() const;

		/**
		*  @brief
		*    Set the position
		*
		*  @param[in] position
		*    The position
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setPosition(const glm::vec3& position);

		/**
		*  @brief
		*    Return the rotation
		*
		*  @return
		*    The rotation, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::quat& getRotation() const;

		/**
		*  @brief
		*    Set the rotation
		*
		*  @param[in] rotation
		*    The rotation quaternion
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setRotation(const glm::quat& rotation);

		/**
		*  @brief
		*    Return the scale
		*
		*  @return
		*    The scale, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec3& getScale() const;

		/**
		*  @brief
		*    Set the scale
		*
		*  @param[in] scale
		*    The scale
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setScale(const glm::vec3& scale);

		/**
		*  @brief
		*    Return the transform
		*
		*  @return
		*    The transform, do no destroy the returned instance
		*/
		inline Transform& getTransform();

		/**
		*  @brief
		*    Return the transform
		*
		*  @return
		*    The transform, do no destroy the returned instance
		*/
		inline const Transform& getTransform() const;

		/**
		*  @brief
		*    Set the transform
		*
		*  @param[in] transform
		*    The transform
		*/
		void setTransform(const Transform& transform);

		//[-------------------------------------------------------]
		//[ Ease-of-use                                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set the position and rotation
		*
		*  @param[in] position
		*    The position
		*  @param[in] rotation
		*    The rotation quaternion
		*/
		void setPositionAndRotation(const glm::vec3& position, const glm::quat& rotation);

		//[-------------------------------------------------------]
		//[ Animation transform                                   ]
		//[-------------------------------------------------------]
		inline const Transform& getAnimationTransform() const;
		void setAnimationTransform(const Transform& transform);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void promoteTransformChange(const Transform& transform, uint8 flags);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		Transform  mTransform;					///< Transform part, only position, rotation quaternion and scale are connected to the CAMP reflection system
		Transform  mReservedAnimationTransform;
		Transform* mAnimationTransform;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/component/base/TransformComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::TransformComponent)
