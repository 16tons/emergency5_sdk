// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/ElementManager.h"
#include "qsf/physics/collision/CollisionType.h"
#include "qsf/asset/AssetSystemTypes.h"
#include "qsf/base/UniqueInstance.h"


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
	*    Manager for collision type definitions
	*/
	class CollisionTypeManager : public ElementManager<uint32, CollisionType>, public UniqueInstance<CollisionTypeManager>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class PhysicsSystem;	// Must be able to create and destroy collision type manager instances


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Find collision type by ID
		*
		*  @param[in] id
		*    ID to search for
		*
		*  @return
		*    The collision type if there is one with the given ID, a null pointer otherwise; do not destroy the instance
		*/
		const CollisionType* getCollisionTypeById(const StringHash& id);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		CollisionTypeManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CollisionTypeManager();

		/**
		*  @brief
		*    Load definitions from assets
		*/
		void loadAssets();

		void onAssetsMounted(const Assets& assets);
		void onAssetsUnmounted(const Assets& assets);
		void resolveDirtyState();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool mCollisionTypesDirty;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
