// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/reflection/object/Object.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
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
	*    Street edge class
	*
	*  @remarks
	*    Represents the smallest line segment in streets. Saves begin/end node.
	*    Begin/end indices represent the node index in used street component.
	*    Further properties are a type of street which is used by the AI to decide which entities might move here and
	*    a tag which may be used to signal that this lane is regulated by a traffic light with the matching tag inside the same crossing.
	*/
	class QSF_API_EXPORT StreetEdge : public Object
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		// TODO(co) Comment those. "qsf::getUninitialized<uint32>()" if invalid.
		// TODO(mk) initialize laneType with "qsf::getUninitialized<uint32>()"
		inline StreetEdge(); // Sets the lane type and tag to 0 as a default
		inline StreetEdge(uint32 beginNode, uint32 endNode, uint32 laneType = 0, uint32 tag = 0);

		/**
		*  @brief
		*    Copy constructor
		*/
		inline StreetEdge(const StreetEdge& source);

		/**
		*  @brief
		*    Return "true" if begin and end node is initialize
		*  @todo(mk)
		*    Add identity check
		*/
		inline bool isValid() const;

		/**
		*  @brief
		*    Return the index of the begin node. "qsf::getUninitialized<uint32>()" if invalid.
		*/
		inline uint32 getBeginNode() const;
		inline void setBeginNode(uint32 beginNode);

		/**
		*  @brief
		*    Return the index of the end node. "qsf::getUninitialized<uint32>()" if invalid.
		*/
		inline uint32 getEndNode() const;
		inline void setEndNode(uint32 endNode);

		/**
		*  @brief
		*    Return the lane type. "qsf::getUninitialized<uint32>()" if invalid.
		*/
		inline uint32 getLaneType() const;
		inline void setLaneType(uint32 laneType);

		/**
		*  @brief
		*    Access to the tag property which is used by the AI to build a relation to a traffic light with the same tag inside one crossing.
		*    This relation means that the traffic light manages the passability of this lane.
		*    The default is zero which means that it is not associated to a traffic light at all.
		*/
		//@{
		inline uint32 getTag() const;
		inline void setTag(uint32 tag);
		//@}

		//[-------------------------------------------------------]
		//[ Serialization                                         ]
		//[-------------------------------------------------------]
		void serialize(BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Assignment operators                                  ]
	//[-------------------------------------------------------]
	public:
		inline StreetEdge& operator =(const StreetEdge& source);
		inline bool operator ==(const StreetEdge& source) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		uint32 mBeginNode;	///< Index of the begin node from street crossing component, "qsf::getUninitialized<uint32>()" if invalid
		uint32 mEndNode;	///< Index of the end node from street crossing component, "qsf::getUninitialized<uint32>()" if invalid
		uint32 mLaneType;	///< Lane type, "qsf::getUninitialized<uint32>()" if invalid
		uint32 mTag;		///< Tag, Used by the AI to associate lanes to traffic lights that manage them. 0 if not used.


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
#include "qsf/component/street/crossing/StreetEdge-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::StreetEdge)
