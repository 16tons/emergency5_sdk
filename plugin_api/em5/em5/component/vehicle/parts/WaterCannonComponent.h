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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class TransformComponent;
	class LinkComponent;
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
	*    EMERGENCY 5 water cannon component. Consists in a cannon part with this component and an optional cannon part.
	*/
	class EM5_API_EXPORT WaterCannonComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::WaterCannonComponent" unique component ID


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
		inline explicit WaterCannonComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~WaterCannonComponent();

		inline uint64 getCannonEntityId() const;
		void setCannonEntityId(uint64 cannonEntityId);

		uint64 getTargetEntityId() const;
		void setTargetEntityId(uint64 targetEntityId);

		inline void setIsOnBoat(bool isOnBoat);

		bool getIsCannonInFinalRotation() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetSimulating(bool simulating) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateJob(const qsf::JobArguments& jobArguments);
		void setLocalRotation(float yaw, float pitch);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64								  mCannonEntityId;
		qsf::WeakPtr<qsf::Entity>			  mTargetEntity;
		qsf::WeakPtr<qsf::TransformComponent> mTargetTransformComponent;
		qsf::WeakPtr<qsf::TransformComponent> mCannonTransformComponent;
		qsf::WeakPtr<qsf::LinkComponent>	  mCannonLinkComponent;
		qsf::WeakPtr<qsf::LinkComponent>	  mBaseLinkComponent;
		float		mFinalYaw;
		float		mFinalPitch;
		bool		mIsCannonInFinalRotation;
		bool		mIsOnBoat;

		// Internal
		qsf::JobProxy	mUpdateJobProxy;


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
#include "em5/component/vehicle/parts/WaterCannonComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::WaterCannonComponent)
