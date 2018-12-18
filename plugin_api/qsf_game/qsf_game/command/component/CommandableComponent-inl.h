// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>

#include <camp/arraymapper.hpp>


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
		inline CommandableComponent::CommandableComponent(Prototype* prototype) :
			Component(prototype),
			mUnitType(getUninitialized<uint32>()),
			CommandArray(*this)
		{
			// Nothing to do in here
		}

		inline CommandableComponent::~CommandableComponent()
		{
			// Nothing to do in here
		}

		inline bool CommandableComponent::hasCommandId(uint32 commandId) const
		{
			for (auto& commandIdentifier : mCommandIdentifiers)
			{
				if (commandIdentifier.mId == commandId)
					return true;
			}
			return false;
		}

		inline const CommandableComponent::CommandIdentifierArray& CommandableComponent::getCommandIds() const
		{
			return mCommandIdentifiers;
		}

		inline void CommandableComponent::setCommandIds(const CommandIdentifierArray& commandIds)
		{
			mCommandIdentifiers = commandIds;
		}

		inline uint32 CommandableComponent::getUnitType() const
		{
			return mUnitType;
		}


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		inline std::size_t CommandableComponent::CommandArrayInterface::size() const
		{
			return getOwner().mCommandIdentifiers.size();
		}

		inline const CommandIdentifier& CommandableComponent::CommandArrayInterface::get(std::size_t index) const
		{
			return getOwner().mCommandIdentifiers[index];
		}

		inline void CommandableComponent::CommandArrayInterface::set(std::size_t index, const CommandIdentifier& value)
		{
			getOwner().mCommandIdentifiers[index] = value;
		}

		inline void CommandableComponent::CommandArrayInterface::insert(std::size_t before, const CommandIdentifier& value)
		{
			getOwner().mCommandIdentifiers.insert(getOwner().mCommandIdentifiers.begin() + before, value);
		}

		inline void CommandableComponent::CommandArrayInterface::remove(std::size_t index)
		{
			getOwner().mCommandIdentifiers.erase(getOwner().mCommandIdentifiers.begin() + index);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
