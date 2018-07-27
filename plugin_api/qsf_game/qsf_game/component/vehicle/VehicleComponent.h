// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/component/base/PrototypeContainer.h"

#include <qsf/component/Component.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Vehicle component base class
		*/
		class QSF_GAME_API_EXPORT VehicleComponent : public Component, public PrototypeContainer
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;			///< "qsf::game::VehicleComponent" unique component ID
			static const uint32 CATEGORY_PASSENGER;		///< "0" unique identifier inside for the passenger category
			static const uint32 CATEGORY_PERSONNEL;		///< "1" unique identifier inside for the personnel category
			static const uint32 CATEGORY_VEHICLE;		///< "2" unique identifier inside for the personnel category


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*
			*  @param[in] prototype
			*    The prototype this component is in, no null pointer allowed
			*/
			inline explicit VehicleComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~VehicleComponent();


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


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/component/vehicle/VehicleComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::VehicleComponent)
