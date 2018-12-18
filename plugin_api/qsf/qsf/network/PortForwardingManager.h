// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"

#include <string>
#include <map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class NetworkSystem;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Port forwarding manager class
	*
	*  @note
	*    - The external library "miniupnpc" ( http://miniupnp.free.fr/ ) is used
	*/
	class QSF_API_EXPORT PortForwardingManager : public Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class NetworkSystem;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum Protocol
		{
			UDP,
			TCP
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return "true" if an internet gateway device has been detected, "false" otherwise
		*
		*  @return
		*    "true" if an internet gateway device has been detected, "false" otherwise
		*/
		bool isInternetGatewayDeviceDetected() const;

		/**
		*  @brief
		*    Return "true" if port forwarding is possible in this network, "false" otherwise
		*
		*  @return
		*    "true" if port forwarding is possible in this network, "false" otherwise
		*/
		bool isPortforwardingPossible() const;

		/**
		*  @brief
		*    Rescan the LAN for internet gateway devices that support UPnP
		*
		*  @param[in] useIpv6
		*    Provide "true" to use IPv6 for internet gateway device detection instead of IPv4
		*
		*  @return
		*    "true" if an internet gateway device has been detected, "false" otherwise
		*
		*  @note
		*    - A call to this method will clear all portforwardings that were created using qsf::PortForwardingManager prior to this call
		*/
		bool rescanLocalAreaNetwork(bool useIpv6 = false);

		/**
		*  @brief
		*    Try to enable port forwarding for the given local port and protocol
		*
		*  @param[in] localPort
		*    Local port to enable port forwarding to
		*  @param[in] protocol
		*    Protocol to enable port forwarding for
		*  @param[out] externalPort
		*    External port the local port will be reachable at from the WAN (might differ from local port), will only be set if the port forwarding was successfully established
		*  @param[in] description
		*    Human readable description to set for the port forwarding (might e.g. be used to identify the port forwarding in administrative interfaces of the internet gateway device)
		*
		*  @return
		*    "true" if the port forwarding was successfully registered, "false" otherwise
		*
		*  @note
		*    - If the given local port/protocol combination was previously forwarded that forwarding will be reused
		*/
		bool addPortForwarding(uint16 localPort, Protocol protocol, uint16& externalPort, const std::string& description = "QSF");

		/**
		*  @brief
		*    Remove a port forwarding that was established using the port forwarding manager
		*
		*  @param[in] localPort
		*    Local port to remove port forwarding for
		*  @param[in] protocol
		*    Protocol to remove port forwarding for
		*/
		void removePortForwardingByLocalPort(uint16 localPort, Protocol protocol);

		/**
		*  @brief
		*    Remove a port forwarding that was established using the port forwarding manager
		*
		*  @param[in] externalPort
		*    External port to remove port forwarding for
		*  @param[in] protocol
		*    Protocol to remove port forwarding for
		*/
		void removePortForwardingByExternalPort(uint16 externalPort, Protocol protocol);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		PortForwardingManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PortForwardingManager();

		void removeAllPortForwardings();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string									  mControlUrl;
		std::string									  mServiceType;
		std::string									  mLocalIpAddress;
		std::map<std::pair<uint16, Protocol>, uint16> mActiveForwardings;	///< key=pair<localPort, protocol>, value=externalPort


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
