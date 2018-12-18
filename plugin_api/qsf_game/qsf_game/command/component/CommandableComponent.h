// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"
#include "qsf_game/reflection/type/CampCommandIdentifier.h"

#include <qsf/component/Component.h>

#include <vector>


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
		*    Commandable component class
		*/
		class QSF_GAME_API_EXPORT CommandableComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::game::CommandableComponent" unique component ID

			typedef std::vector<CommandIdentifier> CommandIdentifierArray;	///< Shortcut typedef for an array of command identifiers


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
			inline explicit CommandableComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~CommandableComponent();

			/**
			*  @brief
			*    Check for a certain command ID
			*/
			inline bool hasCommandId(uint32 commandId) const;

			/**
			*  @brief
			*    Getter method for mCommandIdentifiers
			*/
			inline const CommandIdentifierArray& getCommandIds() const;

			/**
			*  @brief
			*    Setter method for mCommandIdentifiers
			*/
			inline void setCommandIds(const CommandIdentifierArray& commandIds);

			/**
			*  @brief
			*    Get the unit type
			*/
			inline uint32 getUnitType() const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::CommandableComponent methods      ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Set the unit type
			*
			*  @param[in] unitType
			*    The string hashed ID of the unique unit name. e.g. setUnitType(qsf::StringHash("ambulance"))
			*/
			virtual void setUnitType(uint32 unitType);


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


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			// Connected to CAMP
			CommandIdentifierArray mCommandIdentifiers;
			uint32 mUnitType;						///< String hash of the unit name, use this as identifier for specific units


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
		QSF_CAMP_ARRAY_PROPERTY(CommandArray, CommandableComponent, CommandIdentifier)	// Adds public member variable "CommandArray" which is the middle part of the commands <-> CAMP interconnection


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/command/component/CommandableComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::CommandableComponent)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(qsf::game::CommandableComponent::CommandArrayInterface)
