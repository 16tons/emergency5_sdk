// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/link/type/LinkType.h"
#include "qsf/reflection/CampClass.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class LinkAnchor;
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
	*    Link base class
	*/
	class QSF_API_EXPORT Link
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class LinkSystem;
	friend class LinkTypeManager;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline Link();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Link();

		/**
		*  @brief
		*    Get associated link type
		*/
		inline const LinkType& getLinkType() const;

		/**
		*  @brief
		*    Get associated link type ID
		*/
		inline uint32 getLinkTypeId() const;

		/**
		*  @brief
		*    Return the source link anchor
		*/
		inline LinkAnchor& getSourceAnchor() const;

		/**
		*  @brief
		*    Return the source link anchor ID
		*/
		uint64 getSourceAnchorId() const;

		/**
		*  @brief
		*    Return the target link anchor
		*/
		inline LinkAnchor& getTargetAnchor() const;

		/**
		*  @brief
		*    Return the target link anchor ID
		*/
		uint64 getTargetAnchorId() const;

		/**
		*  @brief
		*    Return the source or target link anchor
		*/
		inline LinkAnchor& getAnchor(link::Direction linkDirection) const;

		/**
		*  @brief
		*    Return the source or target link anchor; inverse to getAnchor()
		*/
		inline LinkAnchor& getRemoteAnchor(link::Direction linkDirection) const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Link methods                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    React to disconnection of the link
		*
		*  @note
		*    - This virtual methods gets called just before the link is diconnected and then destroyed
		*/
		inline virtual void onDisconnect();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const LinkType* mLinkType;		///< Link type
		LinkAnchor*		mSource;		///< Source link anchor
		LinkAnchor*		mTarget;		///< Target link anchor


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/link/link/Link-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::Link)
