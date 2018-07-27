/*=============================================================================
Description: DirectNet interfaces

Revision history:
* Created by Lin Luo, November 16, 2005
=============================================================================*/

#pragma once

#ifdef WIN32
#	include <tchar.h>
#else
typedef char TCHAR;
#   define _T(x) x
#endif

typedef const TCHAR *LPCTSTR;

#ifdef WIN32
    typedef uintptr_t SOCKET;
#else
    typedef int SOCKET;
#endif

namespace DirectNet
{
	extern bool MasterServerHasBeenBusy;
	typedef uint64_t DNID;

#define INVALID_DNID ( (DNID)-1 )

	typedef uint16_t DNPORT;

	struct IServerEventHandler;
	struct IServerSlotEventHandler;
	struct IClientEventHandler;

	class IServer;
	class IServerSlot;
	class IClient;

	void Initialize();
	void Deinitialize();

	//!< paired connection info
	struct CONNECTIONINFO
	{
		std::basic_string<TCHAR> localAddr;
		DNPORT localPort;

		std::basic_string<TCHAR> remoteAddr;
		DNPORT remotePort;
	};

	//!< this interface must be implemented by user application
	struct IServerEventHandler
	{
		//!< this method is invoked when a client connection is established
		virtual void OnCreateServerSlot(IServerSlot* serverSlot_) = 0;

		//!< this method is invoked when a client connection is dropped
		virtual void OnDeleteServerSlot(IServerSlot* serverSlot_) = 0;

		virtual ~IServerEventHandler() {};
	};

	//!< the interface to manage the server
	class IServer
	{
	public:
		//!< the static constructor to create a server instance
		static IServer* CreateInstance();

		//!< the final method to release the server resources
		void Release();

		//!< starts a server listening on port serverPort_, and takes a server event handler; if maximumClients_ is 0, then there is no limit
		virtual bool Start(DNPORT serverPort_, IServerEventHandler* eventHandler_, bool usePingPong_, unsigned maximumClients_ = 0) = 0;
		virtual SOCKET GetSocket() const = 0;

		//!< updates the running server inside the game loop; when any event occur, the appropriate user event handler will be called
		virtual bool Update(float deltaTime_) = 0;

		//!< closes the running server, disconnects all the clients
		virtual void Close() = 0;

	protected:
		virtual ~IServer() {}
	};

	//!< this interface must be implemented by user application
	struct IServerSlotEventHandler
	{
		//!< this method will be invoked when a full user data packet is received in the form of a bit stream
		virtual void OnReceivePacket(const IBitStream* packetStream_, uint32_t packetSequence_) = 0;

		virtual ~IServerSlotEventHandler() {};
	};

	//!< the server slot management interface; a server slot represents a client connection on the server
	class IServerSlot
	{
	public:
		//!< this method sets a server slot event handler to the current server slot
		virtual bool Initialize(IServerSlotEventHandler* eventHandler_, size_t initialBandwidth_ = 0) = 0;

		//!< this method retrieves the server slot ID
		virtual DNID GetID() = 0;

		//!< this method retrieves the owning server of this slot
		virtual IServer* GetServer() = 0;

		//!< this method sends a user data packet in the form of a bit stream over the connection
		virtual bool Send(const IBitStream* packetStream_) = 0;

		//!< this method retrieves the connection information of the server slot
		virtual bool GetConnectionInfo(CONNECTIONINFO& connectionInfo_) = 0;

		//!< this method retrieves the bandwidth of the connection
		virtual size_t GetBandwidth() = 0;

		//!< retrieves the number of bytes waiting in the send queue
		virtual int GetNumBytesQueued() const = 0;

		//!< this method disconnects the current connection with the corresponding client
		virtual void Disconnect() = 0;

	protected:
		virtual ~IServerSlot() {}
	};

	//!< this interface must be implemented by user application
	struct IClientEventHandler
	{
		//!< this method will be invoked when the connection attempt is finished (success or failure as input parameter)
		virtual void OnConnectComplete(bool succeeded_) = 0;

		//!< this method will be invoked when a full user data packet is received in the form of a bit stream
		virtual void OnReceivePacket(const IBitStream* packetStream_, uint32_t packetSequence_) = 0;

		//!< this method will be invoked when the connection is dropped with a reason in the form of a C string
		virtual void OnTerminateSession() = 0;

		virtual ~IClientEventHandler() {};
	};

	//!< the client management interface
	class IClient
	{
	public:
		//!< the static constructor
		static IClient* CreateInstance();

		//!< the final method to release the client resources
		void Release();

		//!< this method tries to establish a connection with a remote server
		virtual bool Connect(LPCTSTR host_, DNPORT port_, IClientEventHandler* eventHandler_, bool usePingPong_, size_t initialBandwidth_ = 0) = 0;

		virtual void ClearEventHandler() = 0;

		//!< this method sends a user data packet in the form of a bit stream
		virtual bool Send(const IBitStream* packetStream_) = 0;

		//!< this method updates the client inside the game loop, calls back user application when any event occur
		virtual bool Update(float deltaTime_) = 0;

		//!< this method retrieves the connection information of the client
		virtual bool GetConnectionInfo(CONNECTIONINFO& connectionInfo_) = 0;

		//!< this method retrieves the bandwidth of the connection
		virtual size_t GetBandwidth() = 0;

		//!< retrieves the number of bytes waiting in the send queue
		virtual int GetNumBytesQueued() const = 0;

		//!< this method closes the connection with the server, if established
		virtual void Close() = 0;

	protected:
		virtual ~IClient() {}
	};

}

