// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/activity/step/ActivityStep.h"

#include <qsf/asset/AssetProxy.h>
#include <qsf/forward/BoostPTree.h>
#include <qsf/base/NamedIdentifier.h>

#include <boost/noncopyable.hpp>
#include <boost/container/flat_map.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Activity class
	*/
	class Activity : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct State
		{
			qsf::NamedIdentifier	   mName;
			std::vector<ActivityStep*> mSteps;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline explicit Activity(uint32 id);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~Activity();

		void clear();

		void loadFromBoostPTree(const boost::property_tree::ptree& pTree);

		inline const qsf::AssetProxy& getSourceAssetProxy() const;
		void setSourceAssetProxy(const qsf::AssetProxy& assetProxy);

		inline uint32 getId() const;

		inline uint32 getInitialStateId() const;
		inline const State* getStateById(uint32 id) const;

		inline uint32 getLoadVersion() const;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef boost::container::flat_map<uint32, State> StateMap;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32				 mId;				///< ID usually the local asset ID of the file this activity was loaded from
		qsf::AssetProxy		 mSourceAssetProxy;	///< Source asset proxy, where this activity was loaded from
		uint32				 mLoadVersion;		///< Number of loads so far (needed for debug reloading)

		qsf::NamedIdentifier mInitialState;
		StateMap			 mStates;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/activity/Activity-inl.h"
