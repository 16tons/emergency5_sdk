// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/operation/Operation.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			class QSF_EDITOR_BASE_API_EXPORT MapOperation : public Operation
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Destructor
				*/
				virtual ~MapOperation();

				/**
				*  @brief
				*    Return the map of the operation
				*
				*  @return
				*    The map of the operation
				*/
				uint32 getMapId() const;

				void setMapId(uint32 mapId);
				void setEnforcedMapId(uint32 mapId);	// Breaks the operation seal

				Map& getMap() const;


			//[-------------------------------------------------------]
			//[ Public virtual qsf::editor::Operation methods         ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Return the memory consumption of this operation
				*
				*  @return
				*    The memory consumption in bytes of this operation
				*
				*  @note
				*    - The memory consumption of cooperative work operations is usually lower because
				*      there's no need to store data required for undoing an operation
				*    - The default implementation returns 0
				*/
				virtual size_t getMemoryConsumption() const;


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			protected:
				/**
				*  @brief
				*    Default constructor
				*/
				explicit MapOperation();


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				uint32 mMapId;	///< The map of the operation, "qsf::getUninitialized<uint32>()" if invalid


			//[-------------------------------------------------------]
			//[ CAMP reflection system                                ]
			//[-------------------------------------------------------]
			QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::MapOperation)
