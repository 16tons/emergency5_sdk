// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/logic/cleanup/Cleanup.h"

#include <OGRE/Math/Simple/OgreAabb.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Layer;
}


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
	*    Cleanup for layers
	*	 They are de-activated when all entities in there are not visible.
	*/
	class EM5_API_EXPORT LayerCleanup : public Cleanup
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;			// "em5::LayerCleanup" game logic type ID


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void createLayerCleanup(qsf::Layer& layer, qsf::Layer* replacementLayer = nullptr);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		LayerCleanup();

		void init(qsf::Layer& layer, qsf::Layer* replacementLayer = nullptr);


	//[-------------------------------------------------------]
	//[ Protected virtual em5::Cleanup methods                ]
	//[-------------------------------------------------------]
	protected:
		virtual CleanupResult readyForCleanupCheap() const override;
		virtual Cleanup::CleanupResult onReadyForCleanupCheap() const override;
		virtual ExecuteResult onExecuteCleanup() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void calculateLayerBoundingBox();
		bool isLayerVisible() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Layer*	mLayer;				///< Layer to remove
		qsf::Layer*	mReplacementLayer;	///< Replacement layer to activate on cleanup
		Ogre::Aabb	mLayerBoundingBox;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::LayerCleanup)
