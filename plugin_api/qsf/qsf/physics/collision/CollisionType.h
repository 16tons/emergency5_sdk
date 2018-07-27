// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <boost/noncopyable.hpp>


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
	*    Collision type definition class
	*/
	class QSF_API_EXPORT CollisionType : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CollisionTypeManager;	// Must be able to set the collision type name


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline explicit CollisionType(uint32 id);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~CollisionType();

		//[-------------------------------------------------------]
		//[ Properties                                            ]
		//[-------------------------------------------------------]
		inline uint32 getId() const;
		inline const std::string& getName() const;
		inline short getCollisionFilterGroup() const;
		inline short getCollisionMask() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32		mId;
		std::string	mName;
		short		mCollisionFilterGroup;	///< The collision filter group with which this shape is registered in the world. Data type required by bullet.
		short		mCollisionMask;			///< The collision mask used to define with which other entities this item may collide. Data type required by bullet.


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/physics/collision/CollisionType-inl.h"
