// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <boost/noncopyable.hpp>


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
	*    Abstract system class
	*
	*  @remarks
	*    A "system" encapsulates a certain aspect of the framework. Examples:
	*    - Operation system (Windows, Linux etc.)
	*    - Renderer (e.g. OGRE, not low level like OpenGL/DirectX)
	*    - Audio (e.g. OpenAL)
	*    - Physics (e.g. Bullet)
	*    - Script (e.g. Lua)
	*    - AI
	*
	*    All systems should be derived from this class. This makes it easier
	*    to find all systems within e.g. the automatic code documentation.
	*
	*    System lifecycle:
	*    - "onStartup()"
	*    - "onShutdown()"
	*/
	class QSF_API_EXPORT System : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~System();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get the name of the system
		*
		*  @return
		*    Null terminated ASCII name of the system, never a null pointer, do not manipulate or destroy the returned name
		*/
		virtual const char* getName() const = 0;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the system
		*
		*  @param[in] serverMode
		*    "true" if the system should start in server mode, else "false"
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - Server mode means that e.g. there's no need for audio & visual stuff because there's no display or sound hardware in the first place
		*/
		virtual bool onStartup(bool serverMode) = 0;

		/**
		*  @brief
		*    Shutdown the system
		*
		*  @note
		*    - This method is only called case "onStartup()" returned successfully
		*/
		virtual void onShutdown() = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		inline System();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/base/System-inl.h"
