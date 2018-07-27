// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ParameterGroup::~ParameterGroup()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline ParameterGroup::ParameterGroup(ParameterGroupManager* parameterGroupManager) :
		Pluginable(parameterGroupManager)
	{
		// Nothing to do in here
	}

	template <typename T>
	bool ParameterGroup::assignAndPromoteChange(T& variable, const T& newValue, uint32 propertyId)
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
