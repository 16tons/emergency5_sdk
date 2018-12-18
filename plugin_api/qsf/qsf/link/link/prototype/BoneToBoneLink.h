// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/link/link/prototype/BoneLink.h"
#include "qsf/job/JobProxy.h"


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
	*    Bone to bone link class for multiple bone to bone connections
	*/
	class QSF_API_EXPORT BoneToBoneLink : public BoneLink
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint16 NUMBER_OF_LINKS = 2;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BoneToBoneLink();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BoneToBoneLink();

		/**
		*  @brief
		*    Initialization of bone to bone links
		*
		*  @param[in] index
		*    Index of the link [0, NUMBER_OF_LINKS - 1]
		*  @param[in] parentEntityId
		*    ID of entity we want to use. Keep it uninitialized to use parent from the link component.
		*  @param[in] parentBoneName
		*    Name of the bone of the parent entity
		*  @param[in] childBoneName
		*    Name of the bone of the child entity
		*  @param[in] localPosition
		*    Local position relative to the parent bone
		*  @param[in] localRotation
		*    Local rotation relative to the parent bone
		*  @param[in] isChildRendered
		*    Give over the information if the child entity is still rendered
		*  @param[in] setParentTransformToChild
		*    Indicates if the parent transform (position/rotation) should be applied to the client transform, this avoids jumping of the child when the parent moves with an active movable component
		*/
		void initBoneToBoneLink(int index, uint64 parentEntityId, const std::string& parentBoneName1, const std::string& childBoneName1,
			const glm::vec3& localPosition1, const glm::quat& localRotation1, bool isChildRendered, bool setParentTransformToChild);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Link methods                   ]
	//[-------------------------------------------------------]
	protected:
		virtual void onDisconnect() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::BoneLink methods               ]
	//[-------------------------------------------------------]
	protected:
		virtual void updateJob(const JobArguments& jobArguments) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Bone to bone class for attachment of an entity to a bone of another entity
		*/
		class QSF_API_EXPORT BoneToBone
		{
		public:
			BoneToBone();
			virtual ~BoneToBone();
			void initBoneToBone(Entity& sourceEntity, Entity& targetEntity, const std::string& parentBoneName, const std::string& childBoneName, const glm::vec3& localPosition, const glm::quat& localRotation, bool isChildRendered, bool setParentTransformToChild);
			void disconnect();
			bool isInitialized() const;
			void updateJob(const JobArguments& jobArguments);

		private:
			uint16		mParentBoneHandle;
			uint16		mChildBoneHandle;
			glm::vec3	mLocalPosition;
			glm::quat	mLocalRotation;
			Entity*		mSourceEntity;	// We can use pointer, BoneToBoneLink will destroy us if the entity gets changed
			Entity*		mTargetEntity;
			bool		mChildRendered;
			bool		mSetParentTransformToChild; // Indicates if the transform (position/rotation) of the parent should be set to the client transform, this avoids jumping of the child when the parent moves with an active movable component

		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<BoneToBone>	mBoneToBoneLinks;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::BoneToBoneLink)
