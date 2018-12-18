// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		enum EActionFindPathState
		{
			EFP_INITIAL,
			EFP_SEARCH_PATH,
			EFP_SEARCH_FRONT_PATH,
			EFP_SEARCH_BACK_PATH,
			EFP_MOVE,
			EFP_ABORT,
			EFP_DONE
		};

		enum EActionFindPathFlags
		{
			EFPF_NONE					= 0x0000,
			EFPF_IGNORE_HYDRANTDISTANCE = 0x0001,
			EFPF_NOCURVES				= 0x0002
		};

		// #define BLINKER_ONLY_ON_STREETS	// TODO(co) We don't need the street handling in here

		//----------------------------------------------------------------------------

		/*!
		 * Action that uses the router to find the path to a given target.
		 */
		class EActionFindPath : public EAction
		{
			DECLARE_ACTION(EActionFindPath);

		public:
			EActionFindPath();
			~EActionFindPath();

			bool Init(EGenericObject* owner_, float targetX_, float targetY_, float targetZ_, EActionFindPathFlags flags_ = EFPF_NONE);
			bool Init(EGenericObject* owner_, float targetX_, float targetY_, float targetZ_, float targetDir_);
			bool Init(EGenericObject* owner_, EActor* target_, ETargetPoint point_, float distance_ = 0.0f, float followTargetNear_ = 50.f, float followTargetHalt_ = 200.f);
			bool Init(EGenericObject* owner_, ERouterTarget* target_);
			virtual void DeInit(bool aborting_ = false);

			// TODO(co) No (save games) serialization required
			// virtual bool SerializeTo(ESerializer *stream_);
			// virtual bool SerializeFrom(ESerializer *stream_);

			void GetCallbackInfo(EActionCallback &Info_);

			virtual EActionResult Update();

			// does sum the distance + the lenghts of all paths before the current path
			float GetDistanceMP()	const;

		protected:
			void InternalInit();

			float GetSpeed(bool withoutlimit_, bool testTerrainAtPosition);

			EActionResult ProcessNormal();
			// EActionResult ProcessStreet();	// TODO(co) We don't need the street handling in here

			int HasGangsterCarCollision(const ERouterObjectState &routerObjState_);
			int CheckCollision(const ERouterObjectState &routerObjState_, int stepsAhead_, bool ignoreUnmoved);

			void OnGangsterCarRoadblockCollision();
			void OnGangsterCarPersonCollision();

			void StartObject();
			void StopObject(bool stopMoving = true);
			bool HaltObject(bool stopMoving = true);

			bool ComputeTarget();
			glm::vec3 ComputeGroupTarget(const glm::vec3& target_, float safetyBorder_, int fapFlags_, bool forceComputing_ = false);
			// glm::vec3 ComputeStreetGroupTarget(const glm::vec3& target_, const glm::vec3& lastStreetPos_);	// TODO(co) We don't need the street handling in here
			void ComputeMultiSelectionOffset();

			// bool FindAdditionalStreetPaths();	// TODO(co) We don't need the street handling in here
			glm::vec3 NearestPoint(const glm::vec3& point_, const glm::vec3& p0_, const glm::vec3& p1_) const;
			// int FindNearestStreetPoint(const glm::vec3& point_, const glm::vec3& lastStreetPos_, glm::vec3& nearest_) const;	// TODO(co) We don't need the street handling in here

			void InitSmoothing();
			void DoSmoothing();
			bool TryFindShortcut(int pathId_, int sourceNode_, int targetNode_, Bezier &curve_, char &movement_);

			// Blinker
			void InitBlinker();
			void SetBlinker(EBlinkerMode mode_);
			void StopBlinker();
			void CheckBlinker(float dist_, int pathState_);
			void UpdateBlinker(float dist_);

			void UpdateDust(const glm::vec3& pos_);

			void SetState(int state_);
			void SetRouterPathMP();

		private:
			glm::vec3 mTargetPos;
			int mTargetActor;
			bool mHasTargetDir;
			float mTargetDir;
			ETargetPoint mTargetPoint;
			float mTargetDistance;

			bool mTargetComputed;

			// TODO(co) We don't need the street handling in here
			// bool mGroupStreetTargetComputed;
			// bool mHasGroupStreetTarget;

			bool mHasMultiSelection;
			float mGroupTargetOffsetX, mGroupTargetOffsetY;
			glm::vec3 mSelectionCenter;
			bool mOwnerPlaySirenInMultiselection;

			float mTargetNear;
			float mTargetHalt;

			ERouterTarget* mTarget;

			// TODO(co) We don't need the street handling in here
			/*
			ERouterTarget* mGroupStreetTarget;
			ERouterTarget* mStreetTarget;
			int mStreetStartNode[3];
			int mRealStreetStartNode;
			int mNumStreetStartNodes;
			*/

			float mSpeedLimit;
			float mCurrentSpeed;
			float mDistance;

			int mState;

			// TODO(co) We don't need the street handling in here
			/*
			bool mUseStreet;
			bool mForceNoStreet;
			bool mForceStreet;
			*/

			ERouterPath mPaths[3];
			int mPathState;
			// EStreetType mStreetType;	// TODO(co) We don't need the street handling in here

			bool mAccelerate;
			char mMovementType;

			bool mHalted;
			float mHaltTime;

			bool mUseUnitCarLogic;
			bool mIsRedirectedCar;
			int mPathFlags;
			EActionFindPathFlags mFlags;

			int mNumTries;
			long mStartTime;
			bool mRunningStopped;

			bool mRerouteOnTarget; // bei Suchtiefenlimit

			int	mSmoothFrom;
			int mSmoothTo;
			// int mSmoothStreetPath;	// TODO(co) We don't need the street handling in here

			bool mFollowOnDifferentHouses;	// owner and target are not in the same house while in ETP_FOLLOW mode

			float mBlinkerOffDistance;
			EBlinkerMode mBlinkerMode;
			ParticleEffect* mDustEffect;
			BLString mDustEffectFile;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
