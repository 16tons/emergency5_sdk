// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/asset/type/AssetTypes.h>
#include <qsf/base/NamedIdentifier.h>


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
	*    Static EMERGENCY 5 plugin asset types collection class
	*/
	class EM5_API_EXPORT AssetTypes : public qsf::AssetTypes
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Plugin;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Health
		static const qsf::NamedIdentifier EM5_DIAGNOSIS;	///< Asset type name "em5_diagnosis"
		static const qsf::NamedIdentifier EM5_INJURY;		///< Asset type name "em5_injury"
		static const qsf::NamedIdentifier EM5_MEDICALCARE;	///< Asset type name "em5_medicalcare"
		// Gangster type
		static const qsf::NamedIdentifier EM5_GANGSTERTYPE;	///< Asset type name "em5_gangstertype"
		// Fire material
		static const qsf::NamedIdentifier EM5_FIREMATERIAL;	///< Asset type name "em5_firematerial"
		// Event
		static const qsf::NamedIdentifier EM5_EVENTPOOL;	///< Asset type name "em5_eventpool"
		static const qsf::NamedIdentifier EM5_EVENTDENSITY;	///< Asset type name "em5_eventdensity"
		// Specifications
		static const qsf::NamedIdentifier EM5_SPECS;		///< Asset type name "em5_specs"
		// Spawning
		static const qsf::NamedIdentifier EM5_SPAWNPOOL;	///< Asset type name "em5_spawnpool"
		static const qsf::NamedIdentifier EM5_SPAWNGROUP;	///< Asset type name "em5_spawngroup"
		// Units
		static const qsf::NamedIdentifier EM5_ORDERINFO;	///< Asset type name "em5_order_info"
		static const qsf::NamedIdentifier EM5_UNITPOOL;		///< Asset type name "em5_unitpool"
		// Game mode
		static const qsf::NamedIdentifier EM5_GAMEMODE;		///< Asset type name "em5_gamemode"
		// Soundtrack
		static const qsf::NamedIdentifier EM5_SOUNDTRACK;	///< Asset type name "em5_soundtrack"
		// Activity
		static const qsf::NamedIdentifier EM5_ACTIVITY;		///< Asset type name "em5_activity"


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		AssetTypes();

		/**
		*  @brief
		*    Destructor
		*
		*  @brief
		*    - Not virtual by intent
		*/
		~AssetTypes();

		/**
		*  @brief
		*    Register the asset types
		*/
		static void registerAssetTypes();

		/**
		*  @brief
		*    Unregister the asset types
		*/
		static void unregisterAssetTypes();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
