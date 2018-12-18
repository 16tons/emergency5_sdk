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

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BasePrototypeManager;
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
	*    Prototype manager helper class
	*/
	class QSF_API_EXPORT PrototypeManagerHelper : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototypeManager
		*    Prototype manager or map to use, must stay valid as long as this prototype manager helper instance exists
		*/
		explicit inline PrototypeManagerHelper(const BasePrototypeManager& prototypeManager);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~PrototypeManagerHelper();

		/**
		*  @brief
		*    Hide multiple particle prototypes given by their IDs
		*
		*  @param[in] particleIds
		*    Array of particle prototype IDs
		*/
		void hideMultipleParticlesById(const std::vector<uint64>& particleIds) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const BasePrototypeManager& mPrototypeManager;	///< Prototype manager to use, always valid, do not destroy the instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/prototype/helper/PrototypeManagerHelper-inl.h"
