// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/health/diagnosis/Diagnosis.h"

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
	*    Manager for diagnosis definitions
	*/
	class EM5_API_EXPORT DiagnosisManager : public qsf::ElementManager<uint32, Diagnosis>
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		DiagnosisManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DiagnosisManager();

		/**
		*  @brief
		*    Load definitions from assets
		*/
		void loadAssets();

		/**
		*  @brief
		*    Save diagnosis JSON file
		*
		*  @param[in] filename
		*    Path of the file to save
		*/
		bool saveByFilename(const std::string& filename);

		/**
		*  @brief
		*    Find diagnosis by ID
		*
		*  @param[in] id
		*    ID to search for
		*
		*  @return
		*    The diagnosis if there is one with the given ID, a null pointer otherwise; do not destroy the instance
		*/
		const Diagnosis* getDiagnosisById(const qsf::StringHash& id) const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
