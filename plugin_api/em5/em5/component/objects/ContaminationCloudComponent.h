// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/health/injury/Injury.h"

#include <qsf/component/Component.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/time/Time.h>
#include <qsf/job/JobProxy.h>
#include <qsf/debug/DebugDrawProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ParameterGroup;
	class JobArguments;
	class TransformComponent;
}
namespace em5
{
	class HealthComponent;
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
	*    EMERGENCY 5 contamination cloud component class
	*/
	class EM5_API_EXPORT ContaminationCloudComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::ContaminationCloudComponent" unique component ID

		enum State
		{
			STATE_INACTIVE,
			STATE_STARTUP,
			STATE_ACTIVE,
			STATE_SHUTDOWN
		};

		// Shortcuts
		typedef Injury::ContaminationType ContaminationType;


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
		explicit ContaminationCloudComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ContaminationCloudComponent();

		/**
		*  @brief
		*    Start the simulation after a timer delay and activate particles effect
		*/
		void startSimulation();

		/**
		*  @brief
		*    Return current state of the simulation
		*/
		inline State getSimulationState() const;

		/**
		*  @brief
		*    Stop the simulation after a timer delay and activate particles effect
		*/
		void stopSimulation();

		/**
		*  @brief
		*    Get the minimum contamination delay
		*/
		inline float getMinContaminationDelay() const;

		/**
		*  @brief
		*    Set the minimum contamination delay
		*/
		inline void setMinContaminationDelay(float contaminationDelay);

		/**
		*  @brief
		*    Get the maximum contamination delay
		*/
		inline float getMaxContaminationDelay() const;

		/**
		*  @brief
		*    Set the maximum contamination delay
		*/
		inline void setMaxContaminationDelay(float contaminationDelay);

		/**
		*  @brief
		*    Get the contamination radius
		*/
		inline float getContaminationRadius() const;

		/**
		*  @brief
		*    Set the contamination radius
		*/
		void setContaminationRadius(float contaminationRadius);

		/**
		*  @brief
		*    Check if the position is inside the contamination radius
		*/
		bool isInsideContaminationRadius(const glm::vec3& position) const;

		/**
		*  @brief
		*    Check if the position is inside the contamination radius
		*/
		bool getHealthComponentsInsideContaminationRadius(std::vector<HealthComponent*>& healthComponents) const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onSetSimulating(bool simulating) override;
		virtual void onSetDebug(bool debug) override;
		inline virtual bool implementsOnComponentPropertyChange() const override  { return true; }
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<uint64> CloudIdArray;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void addRequest(const qsf::TransformComponent& transformComponent);
		void onPropertyChange();

		/**
		*  @brief
		*    Start the contamination spreading
		*/
		void startContaminationSpreading(const qsf::MessageParameters& parameters = qsf::MessageParameters());

		/**
		*  @brief
		*    Stop the contamination spreading
		*/
		void stopContaminationSpreading(const qsf::MessageParameters& parameters = qsf::MessageParameters());

		void setParticlesState(bool active, bool emitterEnabled);

		void setTimer(const qsf::MessageConfiguration& message, const qsf::Time& timerTime);

		void manageContaminationSpreading(const qsf::JobArguments& jobArguments);

		void onDebugGroupPropertyChanged(const qsf::ParameterGroup& parameterGroup, uint32 propertyId);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		State				mState;						///< State of the component
		float				mMinContaminationDelay;		///< Minimum delay before or after the contamination affects the game
		float				mMaxContaminationDelay;		///< Maximum delay before or after the contamination affects the game
		float				mContaminationRadius;		///< Radius in which persons get effected by this cloud
		uint32				mTimerId;
		qsf::JobProxy		mSimulationJobProxy;
		qsf::MessageProxy	mMessageProxy;
		qsf::DebugDrawProxy	mDebugDrawProxy;


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/objects/ContaminationCloudComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::ContaminationCloudComponent)
