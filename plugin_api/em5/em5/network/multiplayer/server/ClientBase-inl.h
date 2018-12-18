// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{
		namespace server
		{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			inline ClientBase::ClientBase() :
				mPlayerIndex(qsf::getUninitialized<uint32>())
			{
			}

			inline ClientBase::~ClientBase()
			{
			}

			inline uint32 ClientBase::getPlayerIndex() const
			{
				return mPlayerIndex;
			}

			inline void ClientBase::setPlayerIndex(uint32 playerIndex)
			{
				mPlayerIndex = playerIndex;
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // server
	} // multiplayer
} // em5
