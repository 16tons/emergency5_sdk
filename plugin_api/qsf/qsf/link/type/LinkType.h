// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/link/LinkDefinitions.h"

#include <vector>


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
	*    Link type class
	*/
	class QSF_API_EXPORT LinkType
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class LinkTypeManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<uint32> TypeIdArray;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		LinkType(const StringHash& typeId, const StringHash& baseTypeId, const StringHash& classIdentifier, bool multipleForwardConnections, bool multipleBackwardConnections);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~LinkType();

		/**
		*  @brief
		*    Return link type ID
		*/
		inline uint32 getId() const;

		/**
		*  @brief
		*    Return base type ID
		*/
		inline uint32 getBaseId() const;

		/**
		*  @brief
		*    Check whether this link type supports multiple connections in the one or other direction
		*/
		inline bool supportsMultipleConnections(link::Direction linkDirection) const;

		/**
		*  @brief
		*    Return an array with all inherited type IDs, including this type's own ID as last element
		*/
		inline const TypeIdArray& getInheritedTypeIds() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void assignBaseType(const LinkType& base);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32			mId;						///< Type ID for this link type
		uint32			mBaseId;					///< Type ID of the base link type
		uint32			mClassIdentifier;			///< Class identifier, which usually is the hashed class name of an "qsf::Link" sub-class
		bool			mMultipleConnections[2];	///< "true" in case multiple link connections are allowed per source (index "qsf::link::FORWARD") or target (index "qsf::link::BACKWARD")
		const LinkType* mBase;						///< Base link type the link type is inherited; may be a null pointer in case there is no base link type
		TypeIdArray		mInheritedIds;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/link/type/LinkType-inl.h"
