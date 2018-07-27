// Copyright (C) 2012-2017 Promotion Software GmbH


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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class QsfProtocol;
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
	*    Abstract network package class
	*/
	class QSF_API_EXPORT QsfPacket : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline QsfPacket();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~QsfPacket();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::QsfPacket methods                 ]
	//[-------------------------------------------------------]
	public:
		// TODO(co) Document those, "qsf::QsfPacket" is introducing them
		virtual bool receive(const void* data, size_t size) = 0;
		virtual bool send(QsfProtocol* transport) const = 0;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/network/layered/packet/QsfPacket-inl.h"
