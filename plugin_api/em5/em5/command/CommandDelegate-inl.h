// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline CommandDelegate::~CommandDelegate()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::CommandDelegate methods           ]
	//[-------------------------------------------------------]
	inline bool CommandDelegate::areCommandsAllowed()
	{
		// Default implementation: No filtering
		return true;
	}

	inline bool CommandDelegate::isCommandAllowed(const qsf::game::Command& command)
	{
		// Default implementation: No filtering
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
