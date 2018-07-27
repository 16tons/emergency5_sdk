// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/job/JobProxy.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Look at component class
	*/
	class QSF_API_EXPORT LookAtComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "qsf::LookAtComponent" unique component ID
		static const uint32 ACTIVE;				///< "Active" unique class property ID inside the class
		static const uint32 TARGET_ENTITY_ID;	///< "TargetEntityId" unique class property ID inside the class


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
		inline LookAtComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~LookAtComponent();

		/**
		*  @brief
		*    Return the ID of the target entity
		*
		*  @return
		*    The ID of the target entity, "qsf::getUninitialized<uint64>()" if invalid
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline uint64 getTargetEntityId() const;

		/**
		*  @brief
		*    Set the ID of the target entity
		*
		*  @param[in] targetEntityId
		*    The ID of the target entity, "qsf::getUninitialized<uint64>()" if invalid
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setTargetEntityId(const uint64 targetEntityId);

		/**
		*  @brief
		*    Apply looking at the target entity right now
		*/
		void applyLookAt();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(BinarySerializer& serializer) override;


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
		/**
		*  @brief
		*    Main update function that is passed to the job manager
		*
		*  @param[in] jobArguments
		*    Job arguments
		*/
		void updateJob(const JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		uint64 mTargetEntityId;			///< The ID of the target entity, "qsf::getUninitialized<uint64>()" if invalid
		// Internal
		JobProxy mAnimationJobProxy;	///< Regular animation job proxy; for updates once a frame


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/component/rotation/LookAtComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::LookAtComponent)
