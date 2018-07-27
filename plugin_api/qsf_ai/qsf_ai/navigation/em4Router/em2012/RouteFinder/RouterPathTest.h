// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


// TODO(co) We don't need the route finder test at the moment


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		class EPathTest
		{
		public:
			EPathTest();
			~EPathTest();

			void Recalculate();
			void ShowResults() const;
			void ShowTest() const;
			void SetStart(const ERouterObjectState &state_);
			bool HasStart() const { return mHasStartState; }
			void AddPointTarget(glm::vec3 targetPos_);
			void ClearTargets();
			void ClearResults();
			int GetNrTargets() const;
			const std::string & GetName() const { return mName; }
			void SetName(const std::string &name_) { mName = name_; }
			bool SetSelectedPoint(glm::vec3 s_);	// set point close to given vector as selected
			bool HasSelectedPoint() const { return mSelectedPoint > -2 ? true : false; }
			void DeleteSelectedPoint();
			void MoveSelectedPoint(glm::vec3 pos_);

			void SetSelected(bool isSelected_);
			bool IsSelected() const { return mIsSelected; }

			// dist_ is max dist and returns dist to nearest point
			// returns -2 for no point found, -1 for startpoint and >= 0 for index to targetpoint
			int GetNearestPoint(glm::vec3 pos_, float &dist_) const;

			bool Save(BLFile *file_);
			bool Load(BLFile *file_);

		private:
			std::string					mName;
			bool						mHasStartState;
			ERouterObjectState			mStartState;
			BLDynArray<ERouterTarget*>	mTargets;
			BLDynArray<ERouterPath>		mResultPaths;
			int							mSelectedPoint;	// < -2 for none, -1 for start, >= 0 for targets
			bool						mIsSelected;

			static EGenericObject		*mDefaultObj;
			static int					mDefaultObjRefCount;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
