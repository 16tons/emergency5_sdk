// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/logic/cleanup/Cleanup.h"


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
	*    Cleanup for the contamination clouds
	*/
	class EM5_API_EXPORT ContaminationCloudCleanup : public Cleanup
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ContaminationCloudCleanup" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		ContaminationCloudCleanup();


	//[-------------------------------------------------------]
	//[ Protected virtual em5::Cleanup methods                ]
	//[-------------------------------------------------------]
	protected:
		virtual Cleanup::CleanupResult onReadyForCleanupCheap() const override;
		virtual ExecuteResult onExecuteCleanup() override;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::ContaminationCloudCleanup)