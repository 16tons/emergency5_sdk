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
#include <qsf/math/EnumBitmask.h>


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
	*    EMERGENCY 5 hydrant component class
	*/
	class EM5_API_EXPORT HydrantComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::HydrantComponent" unique component ID

		/** Hydrant types */
		// Order of hydrant types defines which ones are preferred, i.e. if there is a fixed hydrant in range of a fire, the vehicle hydrants will be ignored
		enum HydrantType
		{
			VEHICLE_HOSE_CONNECTOR = 0,	///< Hose connector on a vehicle like RW
			FIXED_HYDRANT = 1,			///< Fixed hydrant object standing in the world
			GROUND_HYDRANT				///< Hydrant on ground
		};


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
		inline explicit HydrantComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~HydrantComponent();

		inline HydrantType getHydrantType() const;
		inline void setHydrantType(HydrantType hydrantType);

		/**
		*  @brief
		*    Return the state of the reservation from the hydrant
		*
		*  @return
		*    "true" if the hydrant is reserved; "false" if it's free
		*/
		inline bool isReserved() const;

		/**
		*  @brief
		*    Set the state of the reservation from the hydrant
		*
		*  @param[in] reserved
		*    Set it to "true" to reservate the hydrant or "false" to release it
		*/
		inline void setReserved(bool reserved);

		/**
		*  @brief
		*    Check if a fireman is currently connected to the hydrant
		*
		*  @return
		*    "true" if there is a fireman connected, "false" if the hydrant is free (but may still be reserved!)
		*/
		inline bool isConnected() const;

		/**
		*  @brief
		*    Get the fireman that is currently connected to the hydrant
		*
		*  @return
		*    Entity pointer to the connected fireman, may be a null pointer if there is none
		*/
		inline qsf::Entity* getConnectedFireman() const;

		/**
		*  @brief
		*    Set the connected fireman
		*
		*  @param[in] firemanEntity
		*    The fireman entity to connect to the hydrant
		*/
		void connectFireman(qsf::Entity& firemanEntity);

		/**
		*  @brief
		*    Clear the connected fireman pointer, so that nobody is connected
		*/
		void disconnectFireman();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Inactive hydrant component means that the component is not used for firefighter to connect them.
		*    In case the active is "false" a firefighter how is using this interrupts this extinguish process and lose the connection to the hydrant.
		*/
		virtual void onSetActive(bool active) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum InternalFlags
		{
			RESERVE_HYDRANT		///< Set it to reserve the hydrant for a fireman
		};
		typedef qsf::EnumBitmask<uint8, InternalFlags> InternalFlagSet;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		HydrantType				  mHydrantType;			///< Type of the hydrant: a fixed hydrant object in the world or a hose connector

		// Internal
		qsf::WeakPtr<qsf::Entity> mConnectedFireman;	///< The fireman entity who is currently using the hydrant, if there is one
		InternalFlagSet			  mInternalFlags;		///< Set of internal flags, see enum "InternalFlags"


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
#include "em5/fire/component/HydrantComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::HydrantComponent)
QSF_CAMP_TYPE_NONCOPYABLE(em5::HydrantComponent::HydrantType)
