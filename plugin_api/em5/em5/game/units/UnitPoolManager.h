// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/units/UnitPool.h"			// Not directly needed, but whoever needs the manager will probably also need the unit pool included
#include "em5/game/units/UnitPoolDefinition.h"

#include <qsf/base/manager/ElementManager.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class EM5_API_EXPORT UnitPoolManager : public qsf::ElementManager<uint32, UnitPoolDefinition>
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		UnitPoolManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~UnitPoolManager();

		/**
		*  @brief
		*    Load definitions from assets
		*/
		void loadAssets();

		/**
		*  @brief
		*    Find unit pool definition by its ID
		*
		*  @param[in] id
		*    ID to search for
		*
		*  @return
		*    The unit pool definition if there is one with the given ID, a null pointer otherwise; do not destroy the instance
		*/
		const UnitPoolDefinition* getUnitPoolDefinitionById(const qsf::StringHash& id) const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
