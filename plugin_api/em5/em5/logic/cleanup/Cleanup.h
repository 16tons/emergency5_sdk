// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/logic/gamelogic/GameLogic.h>
#include <qsf/job/JobProxy.h>


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
	*    Cleanup base class
	*
	*  @note
	*    - After this logic was created it waits for a defined time value until it checks if the entity must be cleaned up
	*    - If the cheap check or the expensive check return the reset timer result, the logic waits a defined amount of time until it starts checking again
	*/
	class EM5_API_EXPORT Cleanup : public qsf::GameLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum CleanupResult
		{
			RESULT_OK,			// Cleanup is allowed
			RESULT_FAILED,		// CleanUp is not allowed
			RESULT_RESETTIMER	// CleanUp is not allowed and the timer is reset
		};

		enum ExecuteResult
		{
			EXECUTE_OK,
			EXECUTE_DELETEOWNER
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Clean up the entity
		*/
		void executeCleanup();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor with default values
		*/
		explicit Cleanup(uint32 typeId, const qsf::Time& startWaitTime = qsf::Time::fromSeconds(300.0f), const qsf::Time& resetWaitTime = qsf::Time::fromSeconds(20.0f));

		/**
		*  @brief
		*    Check if the cleanup can be done cheap
		*/
		virtual CleanupResult readyForCleanupCheap() const;

		/**
		*  @brief
		*    React to the ready for cleanup cheap
		*/
		virtual CleanupResult onReadyForCleanupCheap() const;

		/**
		*  @brief
		*    Check if the cleanup can be done expensive
		*/
		virtual CleanupResult readyForCleanupExpensive() const;

		/**
		*  @brief
		*    React to the ready for cleanup expansive
		*/
		virtual CleanupResult onReadyForCleanupExpensive() const;

		/**
		*  @brief
		*    React to execute cleanup method
		*/
		virtual ExecuteResult onExecuteCleanup();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateCleanup(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		qsf::Time		mWaitTimeLeft;		///< Wait time until the entity is checked again
		qsf::Time		mResetWaitTime;		///< The time to wait after the timer was reset
		qsf::JobProxy	mCleanupProxy;		///< Proxy to update the cleanup regularly


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::Cleanup)
