// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/component/Component.h>
#include <qsf/job/JobProxy.h>
#include <qsf/time/Time.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ActionComponent;
}
namespace em5
{
	class CivilistSpawnPointComponent;
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
	*    EMERGENCY 5 move to despawn point component
	*/
	class EM5_API_EXPORT MoveToDespawnComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "em5::MoveToDespawnComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		explicit MoveToDespawnComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MoveToDespawnComponent();

		uint32 getActionPriority() const;
		void setActionPriority(uint32 priority);

		uint32 getMovementMode() const;
		void setMovementMode(uint32 movementMode);

		/**
		*  @brief
		*    Get lists of all valid despawn and transit points
		*
		*  @param[out] exitSpawnPointComponentVector
		*    Gives back a list of all despawn points, all discovered points are attached to the vector
		*  @param[out] transitSpawnPointComponentVector
		*    Gives back a list of all transit points all discovered points are attached to the vector
		*
		*  @note
		*    - Use this function if you want to choose to move a unit to a despawn or transit point in another part
		*/
		void getExitAndTransitSpawnPoints(std::vector<CivilistSpawnPointComponent*>& exitSpawnPointComponentVector, std::vector<CivilistSpawnPointComponent*>& transitSpawnPointComponentVector);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetSimulating(bool simulating) override;
		virtual void onSetActive(bool active) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateJob(const qsf::JobArguments& jobArguments);

		/**
		* Implemented as unregistering the component and registering again with the time delay defined as parameter
		*/
		void setUpdateFrequency(qsf::Time updateInterval);

		void startMoveToDespawnPoint();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		* Defines the number of tries to move to one random point.
		* Since entities starting in very encapsulated areas might actually have only a few options it might take some tries before these actually choose a target they can reach.
		* This looks like they are standing still and we want to avoid that in general.
		* Therefore, after the number of tries defined here, they instead just try to reach any legal spawn point.
		*/
		static const uint32 sNumberOfRandomTries;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32		  mCountDespawnTries;
		qsf::JobProxy mUpdateJobProxy;
		uint32		  mPriority;		///< The priority of the actions pushed
		uint32		  mMovementMode;	///< Movement mode ID to e.g. enforce using the free area; usually "qsf::getUninitialized<uint32>()"
		qsf::WeakPtr<qsf::ActionComponent> mActionComponent;

		bool		  mStartedMovement;	///< Set to "true" when the entity starts searching for a way for the first time
		glm::vec3	  mStartPosition;	///< The original entity position of first path search


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MoveToDespawnComponent)
