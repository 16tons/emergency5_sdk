// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/operation/MapOperation.h"

#include <qsf/reflection/type/CampUint64.h>	// The 64 bit unsigned integer ID of the entity operation class is connected to the CAMP reflection system, so we need this header to tell CAMP about this data type

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class Component;
	class Prototype;
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
			/**
			*  @brief
			*    Abstract entity operation base class
			*
			*  @note
			*    - Works on the primary QSF map ("qsf::Qsf::getMap()")
			*    - 64 bit are required to unique identify the entity
			*/
			class QSF_EDITOR_BASE_API_EXPORT EntityOperation : public MapOperation
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Destructor
				*/
				virtual ~EntityOperation();

				/**
				*  @brief
				*    Return the ID of the entity this operation is performed on
				*
				*  @return
				*    The ID of the entity this operation is performed on, "qsf::getUninitialized<uint64>()" if invalid
				*/
				uint64 getEntityId() const;

				void setEntityId(uint64 entityId);

				/**
				*  @brief
				*    Return the entity this operation is performed on
				*
				*  @return
				*    The entity this operation is performed on, can be a null pointer, do not destroy the instance
				*
				*  @note
				*    - Ease-of-use method
				*/
				Entity* getEntity() const;


			//[-------------------------------------------------------]
			//[ Public qsf::editor::base::Operation methods           ]
			//[-------------------------------------------------------]
			public:
				virtual size_t getMemoryConsumption() const override;


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			protected:
				EntityOperation();


			//[-------------------------------------------------------]
			//[ Protected data                                        ]
			//[-------------------------------------------------------]
			protected:
				uint64 mEntityId;	///< The unique entity identifier, "qsf::getUninitialized<uint64>()" if invalid


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::EntityOperation)
