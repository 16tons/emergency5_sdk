// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/em4Router/em2012/Bezier.h"

#include <glm/glm.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class FastDebugDraw;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		/*!
		 * Type definitions for different kinds of movement.
		 */
		enum EMoveTypes
		{
			EMT_NONE			= 0,
			EMT_FORWARD			= 0x01,
			EMT_BACKWARD		= 0x02,
			EMT_STRAIGHTSHUNTING= 0x04
		};

		/*!
		 * Type definitions for computed paths of the route-finder.
		 */
		enum ERouterPathType
		{
			RPT_NONE,
			RPT_NORMAL,
		//	RPT_STREET,			// TODO(co) We don't need the street handling in here
		//	RPT_STREET_BEZIER,	// TODO(co) We don't need the street handling in here
			RPT_BEZIER,
		};

		/*!
		 * \brief Class to represent a single node of the path.
		 */
		class ERouterPathNode
		{
		public:
			ERouterPathNode() :
				direction(0.0f),
				distance(0.0f),
				movement(0)
			{}

			ERouterPathNode(const glm::vec3& pos_, float dir_, float dist_, char movement_)
				: position(pos_), direction(dir_), distance(dist_), movement(movement_)
			{ }

			bool operator== (const ERouterPathNode &node_) const;
			bool operator!= (const ERouterPathNode &node_) const;

			// TODO(co) No (save games) serialization required
			// bool SerializeTo(ESerializer *stream_) const;
			// bool SerializeFrom(ESerializer *stream_);

			glm::vec3 position;
			float direction;
			float distance;
			char movement;	// EMoveTypes
		};

		/*!
		 * \brief Class to represent a path computed by the route-finder.
		 */
		class ERouterPath
		{
		public:
			typedef std::vector<ERouterPathNode> NodeArray;
			typedef std::vector<Bezier> CurveArray;

			ERouterPath();
			ERouterPath(ERouterPathType type_);
			~ERouterPath();

			// TODO(co) No (save games) serialization required
			/*
			bool SerializeTo(ESerializer *stream_) const;
			bool SerializePartTo(ESerializer *stream_, int start_, int end_) const;	// including start_, excluding end_
			bool SerializeFrom(ESerializer *stream_);
			*/

			ERouterPath& operator= (const ERouterPath &path_);

			void Reserve(int n_);
			void Append(const ERouterPath & path_, bool connectNodes_);
			void AppendPart(const ERouterPath & path_, int startNode_, bool connectNodes_);
			void RemoveFrom(size_t node_);	// remove elements from node to end
			void Add(const glm::vec3& pos_, float dir_, char movement_);
			void Add(const glm::vec3& pos, const Bezier &curve_, char movement_);
			// Shortcut the path by exchanging all nodes exclusive fromNode_, but inclusive toNode_, by the new node
			bool Shortcut(size_t fromNode_, size_t toNode_, const Bezier &curve_, char movement_, bool onlyShorter_ = true);
			void Clear();
			int GetFirstUnequalElement(const ERouterPath & path_);	// return -1 if paths are equal
			void exchangeNodeAndCurve(size_t oldNode_, const ERouterPathNode& node, const Bezier& curve);

			void SetType(ERouterPathType type_) { mType = type_; }
			ERouterPathType GetType() const { return mType; }

			float GetLength() const;
			float GetLengthToNode(size_t node_) const;

			float GetNextDist(float dist_, float delta_) const;

			char GetMovement(float dist_) const;
			glm::vec3 GetPosition(float dist_) const;
			float GetDirection(float dist_) const;
			void GetPositionAndDirection(float dist_, glm::vec3& pos_, float &dir_) const;
			size_t GetNodeId(float dist_) const;

			size_t GetSize() const { return mNodes.size(); }
			const NodeArray & GetNodes() const { return mNodes; }
			const ERouterPathNode & GetFront() const { return mNodes.front(); }
			const ERouterPathNode & GetBack() const { return mNodes.back(); }
			const CurveArray & GetCurves() const { return mCurves; }

			void ChangeNodeDirection(size_t node_, float direction_);

			void Draw(FastDebugDraw& fastDebugDraw, unsigned char r_, unsigned char g_, unsigned char b_, bool relative_ = false, bool showPositions_ = false, bool showNodeIndex_ = false, bool projectToFloor_ = true, bool showDirection_ = false) const;
			// void WriteToLog() const;	// TODO(co) Port logger?

			// check and rest
			inline bool HasChangedMP() const { return mChangedMP; }
			inline void ResetChangedMP() { mChangedMP = false; }

		private:
			ERouterPathType mType;
			NodeArray mNodes;
			CurveArray mCurves;

			// was path changed since last StateStreamSendMP
			// no need to serialize it
			bool mChangedMP;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
