// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/gangster/GangsterType.h"

#include <qsf/base/manager/ElementManager.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Manager for gangster type definitions
	*/
	class EM5_API_EXPORT GangsterTypeManager : public qsf::ElementManager<uint32, GangsterType>
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		GangsterTypeManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GangsterTypeManager();

		/**
		*  @brief
		*    Load definitions from assets
		*/
		void loadAssets();

		/**
		*  @brief
		*    Find gangster type by ID
		*
		*  @param[in] id
		*    ID to search for
		*
		*  @return
		*    The gangster type if there is one with the given ID, a null pointer otherwise; do not destroy the instance
		*/
		const GangsterType* getGangsterTypeById(const qsf::StringHash& id) const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
