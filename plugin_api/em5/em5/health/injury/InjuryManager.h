// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/health/injury/Injury.h"

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
	*    Manager for injury definitions
	*/
	class EM5_API_EXPORT InjuryManager : public qsf::ElementManager<uint32, Injury>
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		InjuryManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~InjuryManager();

		/**
		*  @brief
		*    Load definitions from assets
		*/
		void loadAssets();

		/**
		*  @brief
		*    Save injury JSON file
		*
		*  @param[in] filename
		*    Path of the file to save
		*/
		bool saveByFilename(const std::string& filename);

		/**
		*  @brief
		*    Find injury by ID
		*
		*  @param[in] id
		*    ID to search for
		*
		*  @return
		*    The injury if there is one with the given ID, a null pointer otherwise; do not destroy the instance
		*/
		const Injury* getInjuryById(const qsf::StringHash& id) const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
