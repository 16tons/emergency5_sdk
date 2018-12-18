// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline uint32 Command::getId() const
		{
			return mCommandId;
		}

		inline uint32 Command::getPriority() const
		{
			return mPriority;
		}

		inline bool Command::getIsScriptCommand() const
		{
			return mIsScriptCommand;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
