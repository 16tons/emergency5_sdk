// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/component/base/BoneLinkComponent.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Use this component to attach up to 2 bones of the entity to bones of its parent
		*/
		class QSF_GAME_API_EXPORT BoneToBoneLinkComponent : public BoneLinkComponent
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;						///< "qsf::game::BoneToBoneLinkComponent" unique component ID

			// Link 1
			static const uint32 PARENT_BONE_NAME_1;					///< "ParentBoneName1" unique property ID
			static const uint32 CHILD_BONE_NAME_1;					///< "ChildBoneName1" unique property ID
			static const uint32 LOCAL_POSITION_1;					///< "LocalPosition1" unique property ID
			static const uint32 LOCAL_ROTATION_1;					///< "LocalRotation1" unique property ID
			static const uint32 SET_PARENT_TRANSFORM_TO_CHILD_1;	///< "SetParentTransformToChild1" unique property ID
			// Link 2
			static const uint32 PARENT_ENTITY_2;					///< "ParentEntity2" unique property ID
			static const uint32 PARENT_BONE_NAME_2;					///< "ParentBoneName2" unique property ID
			static const uint32 CHILD_BONE_NAME_2;					///< "ChildBoneName2" unique property ID
			static const uint32 LOCAL_POSITION_2;					///< "LocalPosition2" unique property ID
			static const uint32 LOCAL_ROTATION_2;					///< "LocalRotation2" unique property ID
			static const uint32 SET_PARENT_TRANSFORM_TO_CHILD_2;	///< "SetParentTransformToChild2" unique property ID


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
			inline explicit BoneToBoneLinkComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~BoneToBoneLinkComponent();

			/**
			*  @brief
			*    Getter and setter for all parameters
			*/
			// Link 1
			inline const std::string& getParentBoneName1() const;
			void setParentBoneName1(const std::string& parentBoneName);

			inline const std::string& getChildBoneName1() const;
			void setChildBoneName1(const std::string& childBoneName);

			inline const glm::vec3& getLocalPosition1() const;
			void setLocalPosition1(const glm::vec3& localPosition);

			inline const glm::quat& getLocalRotation1() const;
			void setLocalRotation1(const glm::quat& localRotation);

			inline bool getSetParentTransformToChild1() const;
			void setSetParentTransformToChild1(bool setParentTransformToChild);

			// Link 2
			inline uint64 getParentEntity2() const;
			void setParentEntity2(uint64 parentEntity);

			inline const std::string& getParentBoneName2() const;
			void setParentBoneName2(const std::string& parentBoneName);

			inline const std::string& getChildBoneName2() const;
			void setChildBoneName2(const std::string& childBoneName);

			inline const glm::vec3& getLocalPosition2() const;
			void setLocalPosition2(const glm::vec3& localPosition);

			inline const glm::quat& getLocalRotation2() const;
			void setLocalRotation2(const glm::quat& localRotation);

			inline bool getSetParentTransformToChild2() const;
			void setSetParentTransformToChild2(bool setParentTransformToChild);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			inline virtual bool implementsOnComponentPropertyChange() const override;
			virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;


		//[-------------------------------------------------------]
		//[ Protected qsf::BoneLinkComponent methods              ]
		//[-------------------------------------------------------]
		protected:
			virtual void refreshBoneLink() override;
			virtual void destroyBoneLink() override;


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			// Link 1
			std::string	mParentBoneName1;				///< Name of the bone to attach to
			std::string	mChildBoneName1;				///< Name of the bone that gets attached
			glm::vec3	mLocalPosition1;				///< Local position offset relative to the bone
			glm::quat	mLocalRotation1;				///< Local rotation offset relative to the bone
			bool		mSetParentTransformToChild1;	// Indicates if the transform (position/rotation) of the parent should be set to the client transform, this avoids flickering of the child when the parent moves with an active movable component

			// Link 2
			uint64		mParentEntity2;					///< ID of the parent entity for bone 2
			std::string	mParentBoneName2;				///< Name of the bone to attach to
			std::string	mChildBoneName2;				///< Name of the bone that gets attached
			glm::vec3	mLocalPosition2;				///< Local position offset relative to the bone
			glm::quat	mLocalRotation2;				///< Local rotation offset relative to the bone
			bool		mSetParentTransformToChild2;	// Indicates if the transform (position/rotation) of the parent should be set to the client transform, this avoids flickering of the child when the parent moves with an active movable component


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/component/base/BoneToBoneLinkComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::BoneToBoneLinkComponent)
