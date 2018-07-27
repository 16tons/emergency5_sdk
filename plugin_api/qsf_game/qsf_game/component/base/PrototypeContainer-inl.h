// Copyright (C) 2012-2017 Promotion Software GmbH


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
		inline PrototypeContainer::~PrototypeContainer()
		{
			// Nothing to do in here
		}

		inline const LinkProxy& PrototypeContainer::getLinkProxy() const
		{
			return mContentLinkProxy;
		}

		inline const boost::container::flat_map<LinkAnchorId, ContainerLink*>& PrototypeContainer::getLinks() const
		{
			// The connection map is actually a "boost::container::flat_map<LinkAnchorId, Link*>", but it contains only container link instances, so casting is safe here
			return reinterpret_cast<const boost::container::flat_map<LinkAnchorId, ContainerLink*>&>(mContentLinkProxy.getLinkConnectionMap());
		}


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		inline std::size_t PrototypeContainer::PrototypeIdArrayInterface::size() const
		{
			return getOwner().mContainedEntityIds.size();
		}

		inline const uint64& PrototypeContainer::PrototypeIdArrayInterface::get(std::size_t index) const
		{
			return getOwner().mContainedEntityIds[index];
		}

		inline void PrototypeContainer::PrototypeIdArrayInterface::set(std::size_t index, const uint64& value)
		{
			// Remove old one, insert new one
			{
				getOwner().removePrototypeFromContainerById(value);

				// Ignore certain IDs that make no sense
				if (isInitialized(value) && value != 0)
				{
					getOwner().addPrototypeToContainerById(value, 0);
				}
			}

			getOwner().mContainedEntityIds[index] = value;
		}

		inline void PrototypeContainer::PrototypeIdArrayInterface::insert(std::size_t before, const uint64& value)
		{
			// Ignore certain IDs that make no sense
			if (isInitialized(value) && value != 0)
			{
				getOwner().addPrototypeToContainerById(value, 0);
			}

			getOwner().mContainedEntityIds.insert(getOwner().mContainedEntityIds.begin() + before, value);
		}

		inline void PrototypeContainer::PrototypeIdArrayInterface::remove(std::size_t index)
		{
			const uint64 prototypeId = getOwner().mContainedEntityIds[index];
			getOwner().removePrototypeFromContainerById(prototypeId);

			getOwner().mContainedEntityIds.erase(getOwner().mContainedEntityIds.begin() + index);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
