// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"
#include "qsf/renderer/animation/advanced/SkeletonGroup.h"


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
	*    Skeleton manager to manage different skeleton groups for advanced animation component
	*/
	class SkeletonManager : public Manager
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline SkeletonManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SkeletonManager();

		/**
		*  @brief
		*    Give a pointer to a skeleton group with the OGRE skeleton name, create a new group if we don't have
		*
		*  @param[in] ogreSkeletonName
		*    OGRE skeleton definition name
		*
		*  @return
		*    Give back a pointer to the skeleton group, don't delete it
		*/
		SkeletonGroup* getSkeletonGroup(const std::string& ogreSkeletonName);

		/**
		*  @brief
		*    Release the skeleton group if no other component uses this skeleton group
		*
		*  @param[in] skeletonGroup
		*    Pointer to the skeleton group
		*/
		void freeSkeletonGroup(SkeletonGroup& skeletonGroup);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<SkeletonGroup*>	mSkeletonGroups;	// Managed pointer to skeleton groups with reference counter


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/SkeletonManager-inl.h"
