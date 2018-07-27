// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/reflection/object/Object.h"
#include "qsf/reflection/type/CampGlmVec3.h"
#include "qsf/reflection/type/CampGlmQuat.h"
#include "qsf/math/Transform.h"


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
	//[ Types                                                 ]
	//[-------------------------------------------------------]
	typedef uint32 NodeId;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Node class
	*/
	class QSF_API_EXPORT Node : public Object
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline Node();

		/**
		*  @brief
		*    Constructor accepting a local space node position
		*
		*  @param[in] position
		*    Local space node position
		*/
		inline explicit Node(const glm::vec3& position);

		/**
		*  @brief
		*    Constructor accepting a local space node position and radius
		*
		*  @param[in] position
		*    Local space node position
		*  @param[in] radius
		*    The local space node radius
		*/
		inline Node(const glm::vec3& position, float radius);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] source
		*    Source to copy from
		*/
		inline Node(const Node& source);

		/**
		*  @brief
		*    Return the position
		*
		*  @return
		*    The local space position, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec3& getPosition() const;

		/**
		*  @brief
		*    Set the position
		*
		*  @param[in] position
		*    The local space position
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setPosition(const glm::vec3& position);

		/**
		*  @brief
		*    Return the rotation
		*
		*  @return
		*    The rotation, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::quat& getRotation() const;

		/**
		*  @brief
		*    Set the rotation
		*
		*  @param[in] rotation
		*    The rotation
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setRotation(const glm::quat& rotation);

		/**
		*  @brief
		*    Return the scale
		*
		*  @return
		*    The local space scale, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec3& getScale() const;

		/**
		*  @brief
		*    Set the scale
		*
		*  @param[in] scale
		*    The local space scale
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setScale(const glm::vec3& scale);

		/**
		*  @brief
		*    Return the node transform
		*/
		inline Transform getTransform() const;

		/**
		*  @brief
		*    Set node transform
		*/
		inline void setTransform(const Transform& transform);

		/**
		*  @brief
		*    Return the radius
		*
		*  @return
		*    The local space node radius
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*
		*  @remarks
		*    The concrete semantic of the local space node radius depends on the concrete node usage. Examples:
		*    - Street section component: Curve radius of the node used for the procedural street mesh generation
		*    - Decals component: The size/radius of the decal represented by the node
		*    - AI lane world generation: The free space (radius) around the node
		*/
		inline float getRadius() const;

		/**
		*  @brief
		*    Set the radius
		*
		*  @param[in] radius
		*    The local space node radius
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*
		*  @see
		*    - "qsf::Node::getRadius()"
		*/
		inline void setRadius(float radius);

		/**
		*  @brief
		*    Return the tag
		*
		*  @return
		*    The tag of the node
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*
		*  @remarks
		*    The concrete semantic of the tag depends on the concrete node usage. Examples:
		*    - Decals component: Index of the texture in texture atlas
		*/
		inline int getTag() const;

		/**
		*  @brief
		*    Set tag
		*
		*  @param[in] tag
		*    The tag of the node
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*
		*  @see
		*    - "qsf::Node::getTag()"
		*/
		inline void setTag(int tag);

		/**
		*  @brief
		*    Get status
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*
		*  @see
		*    - "qsf::Node::setStatus()"
		*/
		inline bool getStatus() const;

		/**
		*  @brief
		*    Set status
		*
		*  @param[in] status
		*    The status of the node
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*
		*  @see
		*    - "qsf::Node::getStatus()"
		*/
		inline void setStatus(bool status);

		/**
		*  @brief
		*    Returns whether this node should automatically be connected to nearby structures.
		*    The exact meaning is defined by the systems working with nodes.
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool shouldAutoConnect() const;

		/**
		*  @brief
		*    Sets whether this node should automatically be connected to nearby structures.
		*    The exact meaning is defined by the systems working with nodes.
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setAutoConnect(bool enable);

		//[-------------------------------------------------------]
		//[ Serialization                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Serialize or deserialize the node using a binary serializer
		*
		*  @param[in] serializer
		*    The serializer, which can be either in read or write mode
		*/
		void serialize(BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Assignment operators                                  ]
	//[-------------------------------------------------------]
	public:
		inline Node& operator =(const Node& source);
		inline bool operator ==(const Node& source) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		glm::vec3 mPosition;	///< Local space node position
		glm::quat mRotation;	///< Rotation
		glm::vec3 mScale;		///< Scale
		float	  mRadius;		///< Radius
		bool	  mStatus;		///< Status enabled/disabled
		bool	  mAutoConnect;	///< Flag whether this node should automatically connect to nearby structures during conversion to AI data
		int		  mTag;			///< Tag


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
#include "qsf/component/nodes/Node-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::Node)
