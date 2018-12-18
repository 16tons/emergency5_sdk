// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/BoostAssertHandler.h>
#include <qsf/base/GetUninitialized.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline SequenceTrackEntityComponentProperty::SequenceTrackEntityComponentProperty(Sequence* sequence, uint32 id) :
			SequenceTrack(sequence, id),
			mEntityId(getUninitialized<uint64>()),
			mComponentType(nullptr),
			mComponentProperty(nullptr)
		{
			// Nothing to do in here
		}

		inline uint64 SequenceTrackEntityComponentProperty::getEntityId() const
		{
			return mEntityId;
		}

		inline void SequenceTrackEntityComponentProperty::setEntityId(uint64 entityId)
		{
			mEntityId = entityId;
		}

		inline const camp::Class* SequenceTrackEntityComponentProperty::getComponentType() const
		{
			return mComponentType;
		}

		inline const camp::Class& SequenceTrackEntityComponentProperty::getComponentTypeSafe() const
		{
			// TODO(co) Is the usage of this method safe? Introduced it because there were several placed directly using "mComponentType" without a sanity check.
			QSF_CHECK(nullptr != mComponentType, "The component type instance is invalid", QSF_REACT_THROW);
			return *mComponentType;
		}

		inline const camp::Property* SequenceTrackEntityComponentProperty::getComponentProperty() const
		{
			return mComponentProperty;
		}

		inline const camp::Property& SequenceTrackEntityComponentProperty::getComponentPropertySafe() const
		{
			// TODO(co) Is the usage of this method safe? Introduced it because there were several placed directly using "mComponentProperty" without a sanity check.
			QSF_CHECK(nullptr != mComponentProperty, "The component property instance is invalid", QSF_REACT_THROW);
			return *mComponentProperty;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf
