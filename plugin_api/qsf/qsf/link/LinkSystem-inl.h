// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/link/anchor/prototype/PrototypeLinkAnchorAdapter.h" // For the definition of template link::GetLinkAnchorAdapter


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LinkTypeManager& LinkSystem::getLinkTypeManager()
	{
		return mLinkTypeManager;
	}

	template<typename T>
	LinkAnchor* LinkSystem::getLinkAnchor(const T& object) const
	{
		// Get an error here? See LinkSystem::getLink method below!
		return getLinkAnchorById(typename link::GetLinkAnchorAdapter<T>::Adapter(object).getId());
	}

	template<typename T>
	LinkAnchor& LinkSystem::getOrCreateLinkAnchor(const T& object) const
	{
		// Get an error here? See LinkSystem::getLink method below!
		return getOrCreateLinkAnchorByAdapter(typename link::GetLinkAnchorAdapter<T>::Adapter(object));
	}

	template<typename T>
	bool LinkSystem::destroyLinkAnchor(const T& object)
	{
		// Get an error here? See LinkSystem::getLink method below!
		return destroyLinkAnchorById(typename link::GetLinkAnchorAdapter<T>::Adapter(object).getId());
	}

	template<typename A, typename B>
	Link* LinkSystem::getLink(const A& source, const B& target, const StringHash& linkTypeId) const
	{
		// If you get an error that "qsf::link::GetLinkAnchorAdapter" is not defined,
		// include the correct link anchor adapter header (e.g. "qsf/link/anchor/prototype/PrototypeLinkAnchorAdapter.h")
		return getLinkByAdapters(typename link::GetLinkAnchorAdapter<A>::Adapter(source), typename link::GetLinkAnchorAdapter<B>::Adapter(target), linkTypeId);
	}

	template<typename A, typename B>
	Link* LinkSystem::getOrCreateLink(const A& source, const B& target, const StringHash& linkTypeId)
	{
		// Get an error here? See LinkSystem::getLink method above!
		return getOrCreateLinkByAdapters(typename link::GetLinkAnchorAdapter<A>::Adapter(source), typename link::GetLinkAnchorAdapter<B>::Adapter(target), linkTypeId);
	}

	template<typename A, typename B>
	bool LinkSystem::destroyLink(const A& source, const B& target, const StringHash& linkTypeId)
	{
		// Get an error here? See LinkSystem::getLink method above!
		return destroyLinkByAdapters(typename link::GetLinkAnchorAdapter<A>::Adapter(source), typename link::GetLinkAnchorAdapter<B>::Adapter(target), linkTypeId);
	}

	template<typename T>
	const LinkConnectionMap& LinkSystem::getLinkConnectionMap(const T& object, const StringHash& linkTypeId, link::Direction linkDirection) const
	{
		// Get an error here? See LinkSystem::getLink method above!
		return getLinkConnectionMapByAdapter(typename link::GetLinkAnchorAdapter<T>::Adapter(object), linkTypeId, linkDirection);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* LinkSystem::getName() const
	{
		return "Link system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
