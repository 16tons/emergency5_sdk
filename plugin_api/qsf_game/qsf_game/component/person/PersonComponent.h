// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/component/Component.h>
#include <qsf/base/PropertyHelper.h>


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
		*    Person component class
		*/
		class QSF_GAME_API_EXPORT PersonComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::game::PersonComponent" unique component ID

			enum Gender
			{
				GENDER_UNDEFINED = 0,
				GENDER_MALE,
				GENDER_FEMALE,
				GENDER_ANIMAL
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
			inline explicit PersonComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~PersonComponent();

			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(Gender, Gender, mGender)


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Component methods                 ]
		//[-------------------------------------------------------]
		public:
			virtual void serialize(qsf::BinarySerializer& serializer) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Connected to CAMP
			Gender mGender;

			// TODO(fw): Support for equipment/inventory
			// TODO(co): Can vehicles or animals (ground squirrel armed with atomic weapons for example, or a boring search and resue dog) have equipment/inventory as well?


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/component/person/PersonComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::PersonComponent)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::PersonComponent::Gender)
