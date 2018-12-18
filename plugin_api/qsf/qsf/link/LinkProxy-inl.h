// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/GetUninitialized.h"
#include "qsf/link/anchor/prototype/PrototypeLinkAnchorAdapter.h" // For the definition of template link::GetLinkAnchorAdapter


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LinkProxy::LinkProxy() :
		mLinkTypeId(getUninitialized<uint32>()),
		mLinkDirection(link::FORWARD),
		mLinkAnchor(nullptr),
		mConnectedContainer(nullptr)
	{
		// Nothing here
	}

	inline LinkProxy::~LinkProxy()
	{
		unregister();
	}

	inline link::Direction LinkProxy::getLinkDirection() const
	{
		return mLinkDirection;
	}

	template<typename T>
	void LinkProxy::registerAt(const T& anchorObject, const StringHash& linkTypeId, link::Direction linkDirection)
	{
		// If you get an error that "qsf::link::GetLinkAnchorAdapter" is not defined,
		// include the correct link anchor adapter header (e.g. "qsf/link/anchor/prototype/PrototypeLinkAnchorAdapter.h")
		registerAtAdapter(typename link::GetLinkAnchorAdapter<T>::Adapter(anchorObject), linkTypeId, linkDirection);
	}

	inline void LinkProxy::clearCallbackForLinkAdded()
	{
		mLinkAddedCallback.clear();
	}

	inline void LinkProxy::clearCallbackForLinkRemoved()
	{
		mLinkRemovedCallback.clear();
	}

	inline void LinkProxy::setCallbackForLinkAdded(const CallbackBinding& functionBinding)
	{
		mLinkAddedCallback = functionBinding;
	}

	inline void LinkProxy::setCallbackForLinkRemoved(const CallbackBinding& functionBinding)
	{
		mLinkRemovedCallback = functionBinding;
	}

	template<typename T>
	Link* LinkProxy::getLink(const T& anchorObject) const
	{
		return getLinkByAnchor(typename link::GetLinkAnchorAdapter<T>::Adapter(anchorObject));
	}

	template<typename T>
	T* LinkProxy::getRemoteAnchor() const
	{
		return static_cast<T*>(getRemoteAnchor());
	}

	template<typename T>
	Link* LinkProxy::connectTo(const T& anchorObject)
	{
		return connectToAnchor(typename link::GetLinkAnchorAdapter<T>::Adapter(anchorObject));
	}

	template<typename T>
	bool LinkProxy::disconnectFrom(const T& anchorObject)
	{
		return disconnectFromAnchor(typename link::GetLinkAnchorAdapter<T>::Adapter(anchorObject));
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline void LinkProxy::setConnectedContainer(LinkContainer* linkContainer)
	{
		mConnectedContainer = linkContainer;
	}

	inline void LinkProxy::invalidate()
	{
		mLinkAnchor = nullptr;
		setUninitialized(mLinkTypeId);
	}

	inline void LinkProxy::onLinkAdded(const Link& link)
	{
		if (!mLinkAddedCallback.empty())
			mLinkAddedCallback(link);
	}

	inline void LinkProxy::onLinkRemoved(const Link& link)
	{
		if (!mLinkRemovedCallback.empty())
			mLinkRemovedCallback(link);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
