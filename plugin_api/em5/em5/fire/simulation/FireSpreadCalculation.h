// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/math/Color4.h>
#include <qsf/time/Time.h>

#include <thread>
#include <atomic>
#include <vector>
#include <condition_variable>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FireReceiverComponent;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Structs                                               ]
	//[-------------------------------------------------------]
	namespace firesimulation
	{
		// This struct holds all data needed inside the multi-threaded fire simulation
		struct ComponentData
		{
			FireReceiverComponent*	mComponent;					///< Fire receiver component (or fire component) to which this data block belongs; note that this will be set to null pointer if the fire receiver gets deactivated
			bool					mIsFireSender;				///< Indicates if the entity is emitting heat energy (it is burning)
			bool					mIsBurning;					///< Indicates of the entity is already burning
			bool					mIsDestroyed;
			float					mHardRadius;				///< The hard radius of the fire component (is zero for fire receiver)
			float					mSoftRadius;				///< The soft radius of the fire component (is zero for fire receiver)
			float					mFireEnergy;				///< The amount of fire energy which the component has
			glm::vec3				mPosition;					///< Position of the component, needed for getting the neighbours of a fire component, is optional (e.g. complex fire components have no position)
			bool					mHasPosition;				///< "true" if position of the component is set at all
			float					mCalculatedSpreadEnergy;	///< Calculated amount of fire energy a fire receiver gets from other entities
			bool					mIsExploded;				///< Indicates if the entity exploded
			float					mCoolingEnergy;				///< The amount of cooling energy which the component has currently applied
			ComponentData const*	mSource;					///< Component data instance of the fire receiver component which caused the first fire energy delivery (may be nullptr)
			float					mFireResistance;			///< The resistance to fire energy
		};
	}


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Fire spread calculation helper
	*
	*  @remarks
	*    This class uses a thread for the calculation
	*/
	class FireSpreadCalculation
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef firesimulation::ComponentData ComponentData;

		// Holds the data for the fire spread lines
		struct DebugRequestData
		{
			glm::vec3				mStartPosition;		///< Start position of the line
			glm::vec3				mEndPosition;		///< End position of the line
			qsf::Color4				mLineColor;			///< Line color
			float					mFireEnergy;		///< How much fire energy gets transferred via this "line"
			uint64					mTargetEntityId;	///< The entity ID of the target fire receiver (This entity is selectable)
			uint64					mSourceEntityId;	///< The entity ID of the source fire component (This entity is selectable)
			float					mFactor;			///< Percent value how much the transferred energy contributes to the whole transferred energy to the target

			// Need a default constructor otherwise vector::emplace_back(<val>, <val2>) doesn't work...
			inline DebugRequestData() :
				mFireEnergy(0.0f),
				mTargetEntityId(qsf::getUninitialized<uint64>()),
				mSourceEntityId(qsf::getUninitialized<uint64>()),
				mFactor(0.0f)
			{}

			// Need a constructor otherwise vector::emplace_back(<val>, <val2>) doesn't work...
			// Note: the mPosition element in the given ComponentDatas must be valid
			DebugRequestData(const ComponentData& sourceComponent, const ComponentData& targetComponent, const qsf::Color4& lineColor, const float& fireEnergy);
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FireSpreadCalculation();

		/**
		*  @brief
		*    Returns the result of the last calculation
		*
		*  @remarks
		*    This method waits until the calculation thread has completed its computation
		*/
		// We don't return a const reference to the container because the caller swaps out the content of the container
		std::vector<ComponentData>& getCalculationResult();

		/**
		*  @brief
		*    Prepare for the next calculation run, remove old component data
		*/
		void prepareNextCalculationRun();

		/**
		*  @brief
		*    Start the next calculation run
		*/
		void startNextCalculationRun(float secondsPassed);

		/**
		*  @brief
		*    Add a new uninitialized component data element, and return a reference to it
		*/
		ComponentData& addNewComponentData();

		/**
		*  @brief
		*    Stops the calculation thread (the thread has ended after this call)
		*/
		void stopCalculation();

		/**
		*  @brief
		*    Invalidate a component for fire spread calculation
		*/
		void invalidateComponent(FireReceiverComponent& component);

		/**
		*  @brief
		*    Returns a list of DebugRequestData items for displaying fire receiver debug draw info
		*/
		inline const std::vector<DebugRequestData>& getDebugDrawRequests() const;

		/**
		*  @brief
		*    Setter for setting the information of debug request data should be generated for fire receiver lines
		*/
		inline void setDebugActive(bool isActive);

		/**
		*  @brief
		*    Returns the time (in microseconds) how much time the thread spent to do the calculation
		*/
		inline int64 getCalculationTime() const;

		/**
		*  @brief
		*    Returns the time passed value which was used for the calculation (for time based interpolation)
		*/
		inline const qsf::Time getTimePassed() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Is called inside the thread to do the calculation
		*/
		void doFireSpreadCalculation();

		void getItemsInCircle(const glm::vec3& center, float radius, std::vector<ComponentData*>& result);

		void applyCoolingEnergy(ComponentData& componentData);

		void updateSpreadFire(const ComponentData& componentData);

		// Note: the mPosition member of the components data parameters must be valid
		bool calcSpreadingEnergy(const ComponentData& sourceComponent, ComponentData& targetComponent, float& amountSpreadEnergy);

		void calculateDebugInformation();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::atomic<bool>				mRunning;
		bool							mShowDebug;
		int64							mCalculationTime;			///< Holds the time how long a calculation run took, has only valid data if "QSF_PROFILING" preprocessor definition is enabled
		float							mSecondsPassed;				///< Holds the time passed between two calculation runs
		float							mFireEnergyMultiplier;		///< Factor multiplied to fire energy; usually 1.0f, but can be set differently for harder or easier gameplay
		std::unique_ptr<std::thread>	mCalculationThread;			///< A pointer to the thread object
		std::vector<ComponentData>		mComponentList;				///< List containing Data about all FireReceiver(s)/FireComponent(s)
		std::condition_variable			mConditionalVariable;		///< Conditional variable used to signal the thread to do the next calculation run
		std::mutex						mMutex;						///< Mutex for synchronization between the calculation thread and an calling thread
		std::vector<DebugRequestData>	mDebugDrawRequestsCache;	///< Holds all show fire receiver debug requests generated by the calculation


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/fire/simulation/FireSpreadCalculation-inl.h"
