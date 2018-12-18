// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/link/link/prototype/ChildLink.h"
#include "qsf/job/JobProxy.h"

#include <glm/glm.hpp>
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
	*    Bone link class for attachment of an entity to a bone of another entity
	*/
	class QSF_API_EXPORT BoneLink : public ChildLink
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BoneLink();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BoneLink();

		/**
		*  @brief
		*    Initialization
		*
		*  @param[in] boneName
		*    Name of the bone at the parent (= source) entity to attach to
		*  @param[in] localPosition
		*    Local position relative to the bone
		*  @param[in] localRotation
		*    Local rotation relative to the bone
		*  @param[in] isChildRendered
		*    Give over the information if the child entity is still rendered
		*/
		void initBoneLink(const std::string& boneName, const glm::vec3& localPosition, const glm::quat& localRotation, bool isChildRendered);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Link methods                   ]
	//[-------------------------------------------------------]
	protected:
		// Do not call this function from inside, this could be a base class with other connections who would get disconnected too
		virtual void onDisconnect() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::BoneLink methods               ]
	//[-------------------------------------------------------]
	protected:
		virtual void updateJob(const JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		// We need this function for bone to bone link
		bool isInitialized();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		JobProxy	mUpdateJobProxy;
		uint16		mParentBoneHandle;
		glm::vec3	mLocalPosition;			///< Local position offset relative to the bone
		glm::quat	mLocalRotation;			///< Local rotation offset relative to the bone
		bool		mChildRendered;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::BoneLink)
