// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"

#include <boost/noncopyable.hpp>

#include <vector>


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
	*    System container base class
	*/
	class QSF_API_EXPORT SystemContainer : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline SystemContainer();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SystemContainer();

		/**
		*  @brief
		*    Startup all registered systems in the order of registeration
		*
		*  @param[in] serverMode
		*    "true" if the system should start in server mode, else "false"
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool startupSystems(bool serverMode);

		/**
		*  @brief
		*    Shutdown all registered systems in reverse registration order
		*
		*  @note
		*    - Do only call this method in case "startupSystems()" returned successfully
		*/
		void shutdownSystems();

		/**
		*  @brief
		*    Return the list of systems
		*
		*  @return
		*    Reference to the internal list of systems, do not manipulate the list or destroy the systems
		*/
		inline const std::vector<System*>& getSystems() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::SystemContainer methods           ]
	//[-------------------------------------------------------]
	public:
		virtual const char* getSystemContainerName() const = 0;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::SystemContainer methods        ]
	//[-------------------------------------------------------]
	protected:
		virtual void logSystemMessage(const std::string& message);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		template<typename T> T* createAndInsertSystem();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		std::vector<System*> mSystems;	///< The systems, do not destroy the instances


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/plugin/SystemContainer-inl.h"
