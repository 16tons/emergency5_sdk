// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterObjectState.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterPath.h"
#include "qsf_ai/navigation/em4Router/em2012/terrain.h"

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class FastDebugDraw;

	namespace ai
	{
		class ERouterTarget;
		class EGenericObject;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		enum ERouterSearchType
		{
			ER_NO_SEARCH,
			ER_LOCAL_SEARCH
		//	ER_STREET_SEARCH	// TODO(co) We don't need the street handling in here
		//	ER_HELI_SEARCH		// TODO(co) We don't need the helicopter handling in here
		};

		enum ERouterSearchResult
		{
			ERSR_FAILED,			// no way to the given target could be found
			ERSR_FAILED_CANCELED,	// search canceled
		//	ERSR_FAILED_NOFLIGHT,	// only helisearch: no way to the given target could be found, "noflight"-object involved	TODO(co) We don't need the helicopter handling in here
			ERSR_SUCCESS,			// way to target found (and returned)
			ERSR_SUCCESS_WITHLIMIT, // way found, but not completed, because one of the memory-limits
			ERSR_COMPUTING			// still searching for path to target
		};


		/*!
		 * \brief Base class of all searches of the router.
		 */
		class ERouterSearch
		{
		public:
			ERouterSearch(ERouterSearchType type_);
			ERouterSearch(ERouterSearchType type_, EGenericObject* obj_, const ERouterObjectState &state_, const ERouterTarget *target_, int flags_);
			virtual ~ERouterSearch();

			ERouterSearchType GetType() const { return mType; }
			EGenericObject* GetObject() const { return mObject; }

			// TODO(co) No (save games) serialization required
			// virtual bool SerializeTo(ESerializer *stream_) const;
			// virtual bool SerializeFrom(ESerializer *stream_);

			virtual void Compute(double timeslice_) = 0;
			virtual ERouterSearchResult GetResult(ERouterPath &path_) = 0;
			ERouterSearchResult InspectResult() const { return mResult; }

			virtual void DebugDraw(FastDebugDraw& fastDebugDraw) const {}

			// debugging methods
			void Step(int n_ = 1);

			void SetPriority(int prio_)	{ mPriority = prio_; }
			int GetPriority() const		{ return mPriority; }

		#ifdef DEBUG
			double mTotalTime;
		#endif

		protected:
			void ComputePriority();

			ERouterSearchType mType;
			EGenericObject* mObject;
			WeakPtr<Entity> mEntity;	// The entity wrapped by mObject; this variable is used only to verify the entity still exists
			EObjectTerrainClass mObjectTerrainClass;
			ERouterObjectState mStartState;
			ERouterTarget* mTarget;
			int mFlags;
			int mPriority;

			ERouterSearchResult mResult;		// the result of the search
			ERouterPath mPath;					// the resulting path (if the search was successful)

			// Debugging
			//  -> Actually only for internal builds (additional check to find erroneous behavior), but we need to make sure end-user SDK is the same with or without ENDUSER define
			int mCurrentStep;
			int mMaxStep;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
