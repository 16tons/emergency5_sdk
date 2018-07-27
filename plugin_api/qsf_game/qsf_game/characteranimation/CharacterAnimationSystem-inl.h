// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/StringHash.h>


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
		inline CharacterAnimationSystem::CharacterAnimationSystem()
		{
			// Nothing to do in here
		}

		inline CharacterAnimationSystem::~CharacterAnimationSystem()
		{
			// Nothing to do in here
		}

		inline CharacterAnimationSetManager& CharacterAnimationSystem::getCharacterAnimationSetManager()
		{
			return mCharacterAnimationSetManager;
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::System methods                    ]
		//[-------------------------------------------------------]
		inline const char* CharacterAnimationSystem::getName() const
		{
			return "Character animation system";
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
