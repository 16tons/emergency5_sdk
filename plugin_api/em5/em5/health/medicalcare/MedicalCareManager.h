// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/health/medicalcare/MedicalCare.h"

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
	*    Manager for medical care definitions
	*/
	class EM5_API_EXPORT MedicalCareManager : public qsf::ElementManager<uint32, MedicalCare>
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MedicalCareManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MedicalCareManager();

		/**
		*  @brief
		*    Load definitions from assets
		*/
		void loadAssets();

		/**
		*  @brief
		*    Save medical care JSON file
		*
		*  @param[in] filename
		*    Path of the file to save
		*/
		bool saveByFilename(const std::string& filename);

		/**
		*  @brief
		*    Find medical care by ID
		*
		*  @param[in] id
		*    ID to search for
		*
		*  @return
		*    The medical care if there is one with the given ID, a null pointer otherwise; do not destroy the instance
		*/
		const MedicalCare* getMedicalCareById(const qsf::StringHash& id) const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
