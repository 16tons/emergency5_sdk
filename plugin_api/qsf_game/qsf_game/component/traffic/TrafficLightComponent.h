// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf_ai/worldModel/trafficControl/TrafficLightComponent.h>


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
		*    Visual traffic light component class
		*/
		class QSF_GAME_API_EXPORT TrafficLightComponent : public ai::TrafficLightComponent
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::game::TrafficLightComponent" unique component ID

			/**
			*  @brief
			*    Light type
			*/
			enum LightType
			{
				RED_TRAFFIC_LIGHT,		///< Red traffic light
				YELLOW_TRAFFIC_LIGHT,	///< Yellow traffic light
				GREEN_TRAFFIC_LIGHT,	///< Green traffic light
				_NUM_LIGHT_TYPES		///< Number of traffic light types
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
			explicit TrafficLightComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~TrafficLightComponent();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::ai::TrafficLightComponent methods ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Set the traffic light to the given AI logic state
			*/
			virtual void setManagedState(const ai::worldElement::State& state) override;


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
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			typedef std::vector<uint64> EntityIdArray;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Set the traffic light to a state
			*/
			void setTrafficLightStates(bool red, bool yellow, bool green);

			/**
			*  @brief
			*    Activate or deactivate all lights of the given type
			*/
			void setLightsActive(LightType lightType, bool active, bool force = false);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			EntityIdArray mLightIds[_NUM_LIGHT_TYPES];
			bool		  mLightsActive[_NUM_LIGHT_TYPES];


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

		public:
			QSF_CAMP_ARRAY_INTERFACE_CLASS_WITH_VARIABLE(EntityIdArrayInterface, TrafficLightComponent, std::vector<uint64>, uint64);
			EntityIdArrayInterface RedTrafficLightArray;
			EntityIdArrayInterface YellowTrafficLightArray;
			EntityIdArrayInterface GreenTrafficLightArray;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/component/traffic/TrafficLightComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::TrafficLightComponent)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(qsf::game::TrafficLightComponent::EntityIdArrayInterface)
