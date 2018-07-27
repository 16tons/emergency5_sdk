// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/em4Router/em2012/terrain.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/CollisionBox.h"

#include <glm/glm.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{
		class EGenericObject;
		class ERouterObjectState;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		enum ERouterTargetType
		{
			ERT_NONE,
			ERT_POINT_TARGET,
			ERT_POINT_DISTANCE_TARGET,
			ERT_MULTI_POINT_TARGET,
			ERT_OBJECT_TARGET,
			ERT_TRACEABLE_OBJECT_TARGET,
			_ERT_NUM_TARGET_TYPES
		};

		enum ERouterTurnMode
		{
			ETM_NONE,
			ETM_TURNTO,
			ETM_TURNFROM
		};


		/*!
		 * \brief Base class for all router targets.
		 */
		class ERouterTarget
		{
		public:
			ERouterTarget(ERouterTargetType type_) : mType(type_) {}
			virtual ~ERouterTarget();

			// TODO(co) No (save games) serialization required
			// virtual bool SerializeTo(ESerializer *stream_) const;
			// virtual bool SerializeFrom(ESerializer *stream_);

			ERouterTargetType GetType() const { return mType; }

			virtual ERouterTarget* Clone() const = 0;

			/*!
			* Checks if position_ is a valid target-position.
			* This may be because position_ is exactly the target, position_ is in a given distance around
			* the target or due to some other criteria.
			*/
			virtual bool Reached(EGenericObject* obj_, const ERouterObjectState& state_) const = 0;

			/*!
			 * Computes the minimum distance of state_ from the target, including turns.
			 */
			virtual float ComputeDistance(const ERouterObjectState& state_) const = 0;

			/*!
			 * Computes the minimum distance of position_ from the target, without taking turns into account.
			 */
			virtual float ComputeDistance(const glm::vec3& position_) const = 0;

			/*!
			 * Return the number of individual target, this target object consists of.
			 */
			virtual int GetTargetNum() const = 0;

			/*!
			 * Return the position of the i-th target.
			 */
			virtual const glm::vec3& GetTargetPosition(int i) const = 0;

			/*!
			 * Sets the position of the i-th target.
			 */
			virtual void SetTargetPosition(int i, glm::vec3& position_) {};

			/*!
			 * Returns true if the i-th target has a target-direction.
			 */
			virtual bool HasTargetDirection(int i) const = 0;

			/*!
			 * Return the direction of the i-th target.
			 */
			virtual float GetTargetDirection(int i) const = 0;

			/*!
			 * Return the turnmode of the i-th target.
			 */
			virtual int GetTurnMode(int i) const = 0;

			virtual void SetTolerance(float tolerance_) {};
			virtual void SetTolerance(int i, float tolerance_) {};

			static ERouterTarget* Create(ERouterTargetType type_);

		protected:
			void SetType(ERouterTargetType type_) { mType = type_; }

		private:
			ERouterTargetType mType;
		};


		// in the following distance_ specifies the distance around the target where all positions are valid

		// ----------------------------------------------------------------------------
		/*!
		 * \brief Class for a target at a given point, with or without target-direction.
		 */
		class EPointTarget : public ERouterTarget
		{
		public:
			EPointTarget();
			EPointTarget(const glm::vec3& position_);
			EPointTarget(const glm::vec3& position_, float direction_);

			// TODO(co) No (save games) serialization required
			// virtual bool SerializeTo(ESerializer *stream_) const;
			// virtual bool SerializeFrom(ESerializer *stream_);

			virtual ERouterTarget* Clone() const;

			virtual void SetTolerance(float tolerance_) { mTolerance = tolerance_; }
			virtual void SetTolerance(int i, float tolerance_) { mTolerance = tolerance_; }

			virtual bool Reached(EGenericObject* obj_, const ERouterObjectState& state_) const;
			virtual float ComputeDistance(const ERouterObjectState& state_) const;
			virtual float ComputeDistance(const glm::vec3& position_) const;

			virtual int GetTargetNum() const { return 1; }
			virtual const glm::vec3& GetTargetPosition(int) const { return mPosition; }
			virtual void SetTargetPosition(int, glm::vec3& position_) { mPosition = position_; };
			virtual bool HasTargetDirection(int) const { return mHasDirection; }
			virtual float GetTargetDirection(int) const { return mDirection; }
			virtual int GetTurnMode(int) const { return ETM_NONE; }

		private:
			glm::vec3 mPosition;
			bool mHasDirection;
			float mDirection;
			float mTolerance;
		};

		// ----------------------------------------------------------------------------
		/*!
		 * \brief Class for a target at a given point in a given distance
		 */
		class EPointDistanceTarget : public ERouterTarget
		{
		public:
			EPointDistanceTarget();
			EPointDistanceTarget(const glm::vec3& position_, float distance_);

			// TODO(co) No (save games) serialization required
			// virtual bool SerializeTo(ESerializer *stream_) const;
			// virtual bool SerializeFrom(ESerializer *stream_);

			virtual ERouterTarget* Clone() const;

			glm::vec3 ComputeTargetPos(const glm::vec3& pos_, float offset_) const;

			virtual bool Reached(EGenericObject* obj_, const ERouterObjectState& state_) const;
			virtual float ComputeDistance(const ERouterObjectState& state_) const;
			virtual float ComputeDistance(const glm::vec3& position_) const;

			virtual int GetTargetNum() const { return 1; }
			virtual const glm::vec3& GetTargetPosition(int) const { return mPosition; }
			virtual void SetTargetPosition(int, glm::vec3& position_) { mPosition = position_; };
			virtual bool HasTargetDirection(int) const { return false; }
			virtual float GetTargetDirection(int) const { return 0.0f; }
			virtual int GetTurnMode(int) const { return ETM_NONE; }

		private:
			glm::vec3 mPosition;
			float mDistance;
		};

		// ----------------------------------------------------------------------------
		/*!
		 * \brief Class for a target which consist of multiple points. Reaching one of these points is desired.
		 */
		class EMultiPointTarget : public ERouterTarget
		{
		public:
			typedef std::vector<glm::vec3> PositionArray;
			typedef std::vector<float> DirectionArray;
			typedef std::vector<bool> HasDirectionArray;
			typedef std::vector<float> ToleranceArray;
			typedef std::vector<float> DistanceArray;

			EMultiPointTarget();
			EMultiPointTarget(const PositionArray& positions_);
			EMultiPointTarget(const PositionArray& positions_, const DirectionArray &directions_, const HasDirectionArray &hasDirections_, const DistanceArray &distances, const ToleranceArray &tolerances);

			// TODO(co) No (save games) serialization required
			// virtual bool SerializeTo(ESerializer *stream_) const;
			// virtual bool SerializeFrom(ESerializer *stream_);

			virtual ERouterTarget* Clone() const override;

			glm::vec3 ComputeTargetPos(size_t i, const glm::vec3& pos_, float offset_) const;

			virtual void SetTolerance(float tolerance_) override;
			virtual void SetTolerance(int i, float tolerance_) override { mTolerance[i] = tolerance_; }

			virtual bool Reached(EGenericObject* obj_, const ERouterObjectState& state_) const override;
			virtual float ComputeDistance(const ERouterObjectState& state_) const override;
			virtual float ComputeDistance(const glm::vec3& position_) const override;

			virtual int GetTargetNum() const override { return static_cast<int>(mPositions.size()); }
			virtual const glm::vec3& GetTargetPosition(int i) const override { return mPositions[i]; }
			virtual void SetTargetPosition(int i, glm::vec3& position_) override { mPositions[i] = position_; };
			virtual bool HasTargetDirection(int i) const override { return (mPositions.size() == mDirections.size() && mHasDirections[i]); }
			virtual float GetTargetDirection(int i) const override { return mDirections[i]; }
			virtual int GetTurnMode(int) const override { return ETM_NONE; }

		private:
			PositionArray mPositions;
			DirectionArray mDirections;
			HasDirectionArray mHasDirections;
			ToleranceArray mTolerance;
			DistanceArray mDistances;
		};

		// ----------------------------------------------------------------------------
		/*!
		 * \brief Class for a target that is an object.
		 * \note when using an object-target store the target-geometry (including position & rotation) in the target
		 * to keep the same target over multiple frames even for moving objects
		 */
		class EObjectTarget : public ERouterTarget
		{
		public:
			EObjectTarget();
			EObjectTarget(EGenericObject* obj_, float distance_, float computeTargetPosDistanceScalingFactor_ = 0.8f);
			EObjectTarget(EGenericObject* obj_, float distance_, int turnMode_);

			// TODO(co) No (save games) serialization required
			// virtual bool SerializeTo(ESerializer *stream_) const;
			// virtual bool SerializeFrom(ESerializer *stream_);

			virtual ERouterTarget* Clone() const;

			float GetDistance() const { return mDistance; }

			glm::vec3 ComputeTargetPos(const glm::vec3& pos_, float offset_) const;

			virtual bool Reached(EGenericObject* obj_, const ERouterObjectState& state_) const;
			virtual float ComputeDistance(const ERouterObjectState& state_) const;
			virtual float ComputeDistance(const glm::vec3& position_) const;

			virtual int GetTargetNum() const { return 1; }
			virtual const glm::vec3& GetTargetPosition(int) const { return mPosition; }
			virtual void SetTargetPosition(int, glm::vec3& position_) { mPosition = position_; };
			virtual bool HasTargetDirection(int) const { return false; }
			virtual float GetTargetDirection(int) const { return 0.0f; }
			virtual int GetTurnMode(int) const { return mTurnMode; }
			EGenericObject* GetTargetObject() const { return mObject; }
			int	GetTargetObjectID()	const { return mObjectId; }

		private:
			typedef std::vector<ECollisionBox>	CollisionBoxList;

			int mObjectId;
			EGenericObject* mObject;
			CollisionBoxList mBoxes;
			glm::vec3 mPosition;
			float mDistance;
			int mTurnMode;
			float mComputeTargetPosDistanceScalingFactor;
		};

		// ----------------------------------------------------------------------------
		/*!
		 * \brief like EObjectTarget, but additionally a traceline checks if the way to the target is free
		 */
		class ETraceableObjectTarget : public EObjectTarget
		{
		public:
			ETraceableObjectTarget();
			ETraceableObjectTarget(EGenericObject* obj_, EObjectTerrainClass traceTerrainClass_, float distance_);
			ETraceableObjectTarget(EGenericObject* obj_, EObjectTerrainClass traceTerrainClass_, float distance_, int turnMode_);

			virtual ERouterTarget* Clone() const;

			// TODO(co) No (save games) serialization required
			// virtual bool SerializeTo(ESerializer *stream_) const;
			// virtual bool SerializeFrom(ESerializer *stream_);

			virtual bool Reached(EGenericObject* obj_, const ERouterObjectState& state_) const;
			glm::vec3 ComputeTargetPos(EGenericObject* obj_, const glm::vec3& pos_, float offset_) const;
		private:
			EObjectTerrainClass mTraceTerrainClass;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
