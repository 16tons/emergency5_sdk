// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <glm/glm.hpp>

#include <boost/noncopyable.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Node;
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
	*    Path segmenter helper class
	*
	*  @remarks
	*    Use this class for simplified path evaluation, e.g. when implementing a "follow path" feature or similar
	*/
	class QSF_API_EXPORT PathSegmenter : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct SegmentData
		{
			glm::vec3 mStartPosition;
			glm::vec3 mStartDirection;
			glm::vec3 mRightDirection;
			float	  mAngle;			///< In radians
			float	  mOffset;
			float	  mLength;

			SegmentData() : mAngle(0.0f), mOffset(0.0f), mLength(0.0f) {}
		};
		typedef std::vector<SegmentData> SegmentArray;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PathSegmenter();

		/**
		*  @brief
		*    Destructor
		*/
		~PathSegmenter();

		/**
		*  @brief
		*    Return length in units (usually meters)
		*/
		inline float getLength() const;

		/**
		*  @brief
		*    Return vector of segment representations
		*/
		inline const SegmentArray& getSegments() const;

		/**
		*  @brief
		*    Initialize from array of Node instances
		*/
		void buildFromNodeArray(const std::vector<Node>& nodes);

		void cutAwayPathBeforeOffset(float pathOffset);
		//void cutAwayPathAfterOffset(float pathOffset);

		/**
		*  @brief
		*    Evaluate the path at the given path position
		*
		*  @param[in] pathOffset
		*    Position along the path, measured in units; beginning with 0 at the path start, ending with "getLength()" at the path end
		*
		*  @return
		*    3D position on the path corresponding to the given path position
		*
		*  @note
		*	 - The coordinate system of the returned value depends on the nodes used for original initialization, e.g. may be in an entity's local object space
		*/
		glm::vec3 evaluatePositionAt(float pathOffset) const;

		/**
		*  @brief
		*    Evaluate the path at the given path position
		*
		*  @param[in] pathOffset
		*    Position along the path, measured in units; beginning with 0 at the path start, ending with "getLength()" at the path end
		*
		*  @return
		*    3D direction on the path corresponding to the given path position
		*
		*  @note
		*	 - The coordinate system of the returned value depends on the nodes used for original initialization, e.g. may be in an entity's local object space
		*/
		glm::vec3 evaluateDirectionAt(float pathOffset) const;

		float getPathPositionByNodeIndex(float nodeIndex) const;
		float getNodeIndexByPathPosition(float pathPosition) const;

		void createLinearSegmentApproximation(std::vector<glm::vec3>& positions, float detailFactor = 1.0f);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		SegmentData& addSegment(const glm::vec3& startPosition, const glm::vec3& startDirection, const glm::vec3& rightDirection, float angle, float length);
		SegmentData& addLinearSegment(const glm::vec3& startPosition, const glm::vec3& endPosition);
		SegmentData& addCurvedSegment(const glm::vec3& startPosition, const glm::vec3& endPosition, const glm::vec3& startDirection, const glm::vec3& circlePosition, float circleRadius);

		const SegmentData* getSegmentAtPathOffset(float pathOffset, float& localRelativeOffset) const;

		glm::vec3 evaluatePositionInSegment(const SegmentData& segment, float t) const;
		glm::vec3 evaluateDirectionInSegment(const SegmentData& segment, float t) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SegmentArray	   mSegments;
		float			   mLength;
		std::vector<float> mPathPositionByNodeIndex;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/math/path/PathSegmenter-inl.h"
