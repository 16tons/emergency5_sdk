// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/BoostAssertHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Component::~Component()
	{
		QSF_ASSERT(isRunning() == false, "Component must not be running when it is destroyed", QSF_REACT_NONE);
	}

	inline Prototype& Component::getPrototype() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mPrototype, "The prototype instance is invalid", QSF_REACT_THROW);
		return *mPrototype;
	}

	inline uint32 Component::getId() const
	{
		return mId;
	}

	inline bool Component::isSimulating() const
	{
		return mFlags.isSet(SIMULATING);
	}

	inline bool Component::isActive() const
	{
		return mFlags.isSet(ACTIVE);
	}

	inline bool Component::isDebug() const
	{
		return mFlags.isSet(DEBUG);
	}

	inline bool Component::isHidden() const
	{
		return mFlags.isSet(HIDDEN);
	}

	inline bool Component::isGloballyHidden() const
	{
		return mFlags.isSet(GLOBALLY_HIDDEN);
	}

	inline bool Component::isReallyHidden() const
	{
		return mFlags.isSet(REALLY_HIDDEN);
	}

	inline bool Component::isRunning() const
	{
		return mFlags.isSet(RUNNING);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	inline void Component::onInvalidation()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	inline bool Component::ignoreOwnerRunningState() const
	{
		// Default implementation: Respect the owner running state
		return false;
	}

	inline bool Component::implementsOnComponentPropertyChange() const
	{
		// Default implementation: No callback required
		return false;
	}

	inline void Component::onComponentPropertyChange(const Component&, uint32)
	{
		// Nothing to do in here
	}

	inline void Component::onComponentTransformChange(const Transform&, TransformChangeFlags)
	{
		// Nothing to do in here
	}

	inline void Component::onSetSimulating(bool)
	{
		// Nothing to do in here
	}

	inline void Component::onSetActive(bool)
	{
		// Nothing to do in here
	}

	inline void Component::onSetDebug(bool)
	{
		// Nothing to do in here
	}

	inline void Component::onSetHidden(bool)
	{
		// Nothing to do in here
	}

	inline bool Component::onStartup()
	{
		// Nothing to do in here

		// Done
		return true;
	}

	inline void Component::onShutdown()
	{
		// Nothing to do in here
	}

	template <typename T, typename S>
	bool Component::assignAndPromoteChange(T& variable, const S& newValue, uint32 propertyId)
	{
		if (variable != newValue)
		{
			// Backup the new value
			variable = newValue;

			// Promote the property change
			promotePropertyChange(propertyId);

			// Done
			return true;
		}

		// No change
		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
