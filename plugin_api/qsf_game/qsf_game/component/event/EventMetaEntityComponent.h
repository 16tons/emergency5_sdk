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
#include <qsf/reflection/type/CampQsfAssetProxy.h>

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
		*    QSF game event meta entity component class
		*/
		class QSF_GAME_API_EXPORT EventMetaEntityComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::game::EventMetaEntityComponent" unique component ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			inline explicit EventMetaEntityComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline ~EventMetaEntityComponent();

			inline const std::vector<uint64>& getEntityIdArray() const;


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			typedef std::vector<uint64> EntityIdArray;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			struct EventEntityIdArrayData
			{
				// Connected to CAMP
				EntityIdArray entityIdArray;		///< Array of child destroyed entity IDs

				inline EventEntityIdArrayData();
			}
			mEventEntityIdArray;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

		public:
			QSF_CAMP_ARRAY_INTERFACE_CLASS_WITH_VARIABLE(EventEntityIdArrayInterface, EventMetaEntityComponent, std::vector<uint64>, uint64);
			EventEntityIdArrayInterface EventEntityIdArray;		///< Array of event entity IDs


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/component/event/EventMetaEntityComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::EventMetaEntityComponent)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(qsf::game::EventMetaEntityComponent::EventEntityIdArrayInterface)
