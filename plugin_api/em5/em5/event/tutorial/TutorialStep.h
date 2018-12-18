// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/message/MessageSystem.h>
#include <qsf/reflection/type/CampQsfTime.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class Tutorial;
	class Objective;
}
namespace qsf
{
	class Time;
	class CameraComponent;
	class CameraControlComponent;
	class Entity;
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
	*    EMERGENCY 5 tutorial step base class
	*/
	class TutorialStep
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		explicit TutorialStep(Tutorial& tutorial);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TutorialStep();

		/**
		*  @brief
		*    Startup call for the tutorial step
		*/
		virtual void startup();

		/**
		*  @brief
		*    Updates the tutorial step
		*
		*  @param[in] timePassed
		*    The time passed since the last update call
		*
		*  @return
		*    "True" when step has finished otherwise false
		*/
		virtual bool update(const qsf::Time& timePassed) = 0;

		/**
		*  @brief
		*    Shutdown call for the tutorial step
		*/
		virtual void shutdown();

		virtual const std::string getLayerId() const;
		virtual const std::string getEraseLayerId() const;

		virtual const qsf::Entity* getFocusEntity();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline Tutorial& getTutorial();

		qsf::Entity* acquireTaggedEntity(const std::string& tag);
		qsf::CameraComponent* getCameraComponent() const;
		qsf::CameraControlComponent* getCameraControlComponent() const;

		void clearObjectives();
		void changeEventName(const std::string& eventName);
		Objective& createObjective(uint32 objectiveId, const std::string& objectiveText);
		void markObjectiveAsAccomplished(const uint32 objectiveId);

		bool hasSupervisorEnded(const qsf::Time& timePassed);
		void cancelSupervisorMessage();
		bool hasWaitTimeEnded(qsf::Time& remainingWaitTime, const qsf::Time& timePassed);
		bool hasPauseBetweenSubStepsEnded(const qsf::Time& timePassed) const;

		void showTextWindow(bool show) const;
		void clearTextWindow() const;
		void showObjectiveWindow(bool show) const;

		bool hasCameraStoppedAutomaticMovement();
		void flyToCamera(const qsf::Entity& targetCameraEntity, float maximumSpeed = 200.0f);
		void flyToEntity(const qsf::Entity& targetEntity, bool jumpDirectly = false);
		void startFollowEntity(const qsf::Entity& targetEntity, float distance = 25.0f);
		void stopFollowEntity();

		bool isEntitySelected(uint64 entityId);

		void clearSelection();

		void showUserInterface(bool show) const;

		void showSupervisorMessage(const std::string& textId);

		void showOrderAreaMarker(qsf::Entity* orderAreaEntity, bool show);

		void makeEntitySelectable(qsf::Entity& entity, bool selectable);

		void setupUnitAfterSpawn(qsf::Entity& unitVehicle);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		uint64			mOnlyCommandAllowed;
		qsf::Entity*	mOnlyCommandTargetAllowed;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onPreCommandCheckForCancel(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Tutorial&			mTutorial;	///< The tutorial instance to which this step belongs
		qsf::MessageProxy	mCancelCommandMessageProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/event/tutorial/TutorialStep-inl.h"
