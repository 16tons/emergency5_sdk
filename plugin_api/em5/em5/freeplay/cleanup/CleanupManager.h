// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/manager/Manager.h>
#include <qsf/message/MessageProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class Cleanup;
}
namespace qsf
{
	class GameLogicComponent;
	class Entity;
	class Map;
	class TransformComponent;
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
	*    Manager for cleanups
	*
	*  @remarks
	*    Cleans all the damaged buildings and vehicles or injured persons
	*/
	class EM5_API_EXPORT CleanupManager : public qsf::Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class Cleanup;	// May register and unregister itself


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<Cleanup*> CleanupArray;
		typedef std::pair<qsf::TransformComponent*, bool> BoxInformation;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		CleanupManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CleanupManager();

		/**
		*  @brief
		*    Call cleanup method of all registered observers
		*/
		void cleanupCompleteMap();

		/**
		*  @brief
		*    Startup of the manager
		*/
		void startup();

		/**
		*  @brief
		*    Startup of the manager
		*/
		void shutdown();

		/**
		*  @brief
		*    Enable or disable all regular cleanup updates
		*/
		void setCleanupUpdatesActive(bool active);

		/**
		*  @brief
		*   Removes all civilians(cars/persons) from the map
		*
		*  @note
		*   Only civilians with an MoveToDespawnComponent gets removed
		*/
		void removeAllCivilians();

		/**
		*  @brief
		*   Removes all gangster (with gangsterflag) from the map
		*/
		void removeAllGangster();

		/**
		*  @brief
		*   Removes all barrier tapes from the map
		*/
		void removeAllBarrierTapes();

		void cleanupFreeplayEntities(std::vector<qsf::Entity*>& cleanupEntities);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    If everything is ok create an cleanup observer
		*/
		void onGetsDamaged(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    If everything is ok create an cleanup observer
		*/
		void onStartBurning(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    If everything is ok create an cleanup observer
		*/
		void onStartContamination(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Create the correct cleanup observer for this job
		*
		*  @return
		*    "true" if a cleanup observer is available, "false" otherwise
		*/
		bool createCleanupObserver(uint64 messageId, qsf::GameLogicComponent& gameLogicComponent);

		/**
		*  @brief
		*    Register a new cleanup observer to the manager
		*/
		void registerCleanupObserver(Cleanup& cleanup);

		/**
		*  @brief
		*    Unregister a cleanup observer of the manager
		*/
		void unregisterCleanupObserver(Cleanup& cleanup);

		/**
		*  @brief
		*    Removes all Units from the map
		*/
		void cleanupAllUnits() const;

		/**
		*  @brief
		*    Removes all Buoyes from the map
		*/
		void cleanupAllBuoyes() const;

		/**
		*  @brief
		*    Cleanup all fire components in the map. Non burning entities haven't a firecleanup, but can received heat energy (lowered the combustion value). We have to cleanup them to.
		*/
		void cleanupAllFireComponents();

		/**
		*  @brief
		*    Cleanup all "usable by engineer" components
		*/
		void cleanupAllUsables();

		/**
		*  @brief
		*    Checks if the given position is inside any of the given boxes (Transform and if the box is centered on the y axis)
		*
		*  @param[in] pointPosition
		*    The position to check (in world coordinates)
		*  @param[in] boxInformations
		*    List of information about boxes (transformation and y axis centering) to check the position against
		*
		*  @return
		*    "true" if the point is inside any of the given boxes, "false" otherwise
		*/
		bool isPositionInsideAnyBoxes(const glm::vec3& pointPosition, const std::vector<BoxInformation> &boxInformations) const;

		/**
		*  @brief
		*    Checks if the given position is inside of the given box information (Transform and if the box is centered on the y axis)
		*
		*  @param[in] pointPosition
		*    The position to check (in world coordinates)
		*  @param[in] boxTransform
		*    Transform information about the box to do the check against
		*  @param[in] yAxisCentered
		*    Indicates of the box is centered on the y-axis around the pivot point of the box entity
		*
		*  @return
		*    "true" if the point is inside of the given box information, "false" otherwise
		*/
		bool isPointInsideBox(const glm::vec3& pointPosition, const qsf::TransformComponent& boxTransform, bool yAxisCentered) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::MessageProxy	mMessageProxyPerson;				///< Message proxy for the person cleanup
		qsf::MessageProxy	mMessageProxyContaminatedPerson;	///< Message proxy for contaminated persons cleanup
		qsf::MessageProxy	mMessageProxyVehicle;				///< Message proxy for the vehicle cleanup
		qsf::MessageProxy	mMessageProxyBuilding;				///< Message proxy for the building cleanup
		qsf::MessageProxy	mMessageProxyVegetation;			///< Message proxy for the vegetation cleanup
		qsf::MessageProxy	mMessageProxyFire;					///< Message proxy for the fire cleanup (objects which doesn't fall under any other category as e.g. building)
		qsf::MessageProxy	mMessageProxyContaminationCloud;	///< Message proxy for contamination cloud cleanup

		CleanupArray		mCleanupObservers;					///< All registered cleanup observers are stored here


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
