// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"

#include <qsf/reflection/CampDefines.h>
#include <qsf/component/Component.h>

namespace qsf
{
	namespace ai
	{
		// Adds information to an entity that it is a member in a group.
		// It is currently not allowed to be part of several AI groups since the decisions of the groups could be conflicting otherwise.
		// It is noncopyable like all components by inheritance.
		class QSF_AI_API_EXPORT GroupMemberComponent : public Component
		{
		public:

			// unique camp Ids for the component and its properties
			//@{
			QSF_DECLARE_CAMP_COMPONENT_ID;
			static const unsigned int GROUP_PROPERTY_ID;
			//@}

			static const unsigned int DEFAULT_GROUP;

			explicit GroupMemberComponent(Prototype* prototype);

			// The group ID is the ID of the abstract group entity.
			//@{
			void setGroupId(unsigned int id);
			unsigned int getGroupId() const;
			//@}
		private:
			unsigned int mGroupId;

			QSF_CAMP_RTTI();
		};


		// inline implementation
		inline GroupMemberComponent::GroupMemberComponent(Prototype* prototype) :
			Component(prototype),
			mGroupId(DEFAULT_GROUP)
		{}

		inline void GroupMemberComponent::setGroupId(unsigned int id)
		{
			assignAndPromoteChange(mGroupId, id, GROUP_PROPERTY_ID);
		}

		inline unsigned int GroupMemberComponent::getGroupId() const
		{
			return mGroupId;
		}
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::GroupMemberComponent);
