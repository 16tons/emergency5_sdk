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

#include <boost/container/flat_set.hpp>

#include <vector>

namespace qsf
{
	namespace ai
	{
		// A group component adds information to an entity that it is a group.
		// It is noncopyable like all components by inheritance.
		class QSF_AI_API_EXPORT GroupComponent : public Component
		{
		public:
			QSF_DECLARE_CAMP_COMPONENT_ID; // unique CAMP type id
			static const unsigned int MEMBER_LIST_PROPERTY_ID; // unique CAMP id of the member list property

			explicit GroupComponent(Prototype* prototype);

			// Member management by entity ID.
			//@{
			void addMember(unsigned int id);
			void removeMember(unsigned int id);
			bool isMember(unsigned int id) const;
			//@}
		private:
			boost::container::flat_set<unsigned int> mMemberIds;
			QSF_CAMP_RTTI();
		};


		// inline implementation
		inline GroupComponent::GroupComponent(Prototype* prototype) :
			Component(prototype)
		{}

		inline bool GroupComponent::isMember(const unsigned int id) const
		{
			return mMemberIds.count(id) > 0;
		}
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::GroupComponent);
