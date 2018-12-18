// Copyright (C) 2012-2018 Promotion Software GmbH


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
	*    Blood puddle component class
	*/
	class EM5_API_EXPORT BloodPuddleComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///<  unique component ID


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Creates a new entity with a blood puddle component and a blood puddle mesh
		*
		*  @param[in] bloodSourceEntity
		*    Entity where the blood of the puddle comes from
		*
		*  @return
		*    A reference to the entity that was created
		*/
		static const qsf::Entity& createBloodPuddleEntity(qsf::Entity& bloodSourceEntity);


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
		explicit BloodPuddleComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BloodPuddleComponent();

		/**
		*  @brief
		*    Set the entity ID from which this component gets its blood
		*/
		void setBloodFromEntityId(uint64 entityid);

		/**
		*  @brief
		*    Set the maximal distance the source of this blood can move away so that the puddle will still grow
		*/
		void setMaxDistanceToBloodSourceToGrow(float distance);

		/**
		*  @brief
		*    Set time parameter for growing the puddle, how long it will stay in the map and how long it takes to fade it away
		*
		*  @param[in] timeToGrow
		*    Maximal time for the blood puddle to grow, if the source is moved away it will stop immediatly
		*  @param[in] timeToLay
		*    Time the puddle will stay still
		*  @param[in] timeToFade
		*    Time until the puddle will fade away
		*/
		void setGrowStayLayTime(qsf::Time timeToGrow, qsf::Time timeToLay, qsf::Time timeToFade);

		/**
		*  @brief
		*   The blood puddle will fade out if it is growing or laying around
		*/
		void fadeOutNow();


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
		virtual void onSetActive(bool active) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateBloodPuddle(const qsf::JobArguments& jobArguments);

		static float lerp(float f1, float f2, float a);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::JobProxy	mUpdateJobProxy;					///< Job proxy
		qsf::Time		mTimePassed;
		uint64			mBloodFromEntityId;					///< Entity from the source the blood came from
		qsf::Time		mTimeToGrowFromStart;				///< Time from start until the puddle should have maximal growth
		qsf::Time		mTimeToLayFromStart;				///< mTimeToGrowFromStart + Time the puddle will lay around with the same size
		qsf::Time		mTimeToFadeFromStart;				///< mTimeToLayFromStart + Time the puddle needs to fade out
		float			mAlphaGrow;							///< Grows with the puddle, but is used as reference point when puddle is faded away
		float			mMaxDistanceToBloodSourceToGrow;	///< Maximal distance to the blood source so that the puddle will grow


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::BloodPuddleComponent)
