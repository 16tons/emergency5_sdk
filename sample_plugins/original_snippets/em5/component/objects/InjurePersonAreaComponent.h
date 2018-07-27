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
	*    EMERGENCY 5 injure person area component class
	*/
	class EM5_API_EXPORT InjurePersonAreaComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::InjurePersonAreaComponent" unique component ID


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
		explicit InjurePersonAreaComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~InjurePersonAreaComponent();

		/**
		*  @brief
		*    Return current state of the simulation
		*/
		inline bool getSimulationState() const;

		/**
		*  @brief
		*    Get the affected area radius
		*/
		inline float getAffectedAreaRadius() const;

		/**
		*  @brief
		*    Set the affected area radius
		*/
		void setAffectedAreaRadius(float affectedAreaRadius);

		/**
		*  @brief
		*    Get the used inbjuries as comma separated string
		*/
		inline const std::string& getInjuriesToUse() const;

		/**
		*  @brief
		*    Set the used inbjuries as comma separated string
		*/
		void setInjuriesToUse(const std::string& injuriesToUse);

		/**
		*  @brief
		*    Check if the position is inside the affected area radius
		*/
		bool isInsideAffectedAreaRadius(const glm::vec3& position);


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
		virtual void onSetActive(bool active) override;
		virtual void onSetDebug(bool debug) override;
		inline virtual bool implementsOnComponentPropertyChange() const override  { return true; }
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void internalSetActive(bool active);
		void addRequest(const qsf::TransformComponent& transformComponent);
		void onPropertyChange();

		void manageContaminationSpreading(const qsf::JobArguments& jobArguments);

		void onDebugGroupPropertyChanged(const qsf::ParameterGroup& parameterGroup, uint32 propertyId);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to camp
		float				mAffectedAreaRadius;		///< Radius in which persons get effected by this component
		std::string			mInjuriesToUse;				///< Comma separated list of injuries to use

		// Internal
		std::vector<uint32> mInjuriesToUseList;

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
#include "em5/component/objects/InjurePersonAreaComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::InjurePersonAreaComponent)
