// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/link/link/Link.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Prototype;
	class Entity;
}


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
	*    Prototype link base class
	*
	*  @remarks
	*    This is a base class for all link classes connecting prototypes
	*/
	class QSF_API_EXPORT PrototypeLink : public Link
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PrototypeLink();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PrototypeLink();

		/**
		*  @brief
		*    Return the link's source prototype
		*/
		Prototype& getSourcePrototype() const;

		/**
		*  @brief
		*    Return the link's target prototype
		*/
		Prototype& getTargetPrototype() const;

		/**
		*  @brief
		*    Return the link's source prototype, casted as entity
		*
		*  @note
		*    - This will throw an exception in case the source prototype is not an entity
		*/
		Entity& getSourceEntity() const;

		/**
		*  @brief
		*    Return the link's target prototype, casted as entity
		*
		*  @note
		*    - This will throw an exception in case the target prototype is not an entity
		*/
		Entity& getTargetEntity() const;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PrototypeLink)
