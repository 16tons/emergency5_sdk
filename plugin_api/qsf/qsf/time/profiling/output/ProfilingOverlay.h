// Copyright (C) 2012-2018 Promotion Software GmbH
#ifdef QSF_PROFILING


	//[-------------------------------------------------------]
	//[ Header guard                                          ]
	//[-------------------------------------------------------]
	#pragma once


	//[-------------------------------------------------------]
	//[ Includes                                              ]
	//[-------------------------------------------------------]
	#include "qsf/debug/DebugDrawProxy.h"
	#include "qsf/job/JobProxy.h"
	#include "qsf/time/profiling/ProfilingElement.h"
	#include "qsf/base/NamedIdentifier.h"

	#include <vector>
	#include <unordered_map>


	//[-------------------------------------------------------]
	//[ Forward declarations                                  ]
	//[-------------------------------------------------------]
	namespace qsf
	{
		class DebugTextParameters;
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
		*    Draws information about each job manager on the screen
		*/
		class QSF_API_EXPORT ProfilingOverlay
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			ProfilingOverlay();

			/**
			*  @brief
			*    Destructor
			*/
			~ProfilingOverlay();

			bool isVisible() const;
			void setVisible(bool visible);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			struct ProfilingLine
			{
				NamedIdentifier				mName;
				ProfilingElement			mData;
				bool						mHasData;
				bool						mHasParent;
				std::vector<ProfilingLine*> mChildren;
			};

			typedef std::unordered_map<uint32, ProfilingLine> ProfilingLineMap;
			typedef std::vector<ProfilingLine*>				  ProfilingLineArray;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void updateJob(const JobArguments& jobArguments);
			void processProfilingLineRecursively(const ProfilingLine& profilingLine, glm::vec2& position, DebugTextParameters& textParameters, int level);
			ProfilingLine& getOrCreateProfilingLine(uint32 id, const std::string& name);
			void addText(const std::string& text, const glm::vec2& position, const DebugTextParameters& textParameters);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			DebugDrawProxy	   mDebugDrawProxy;
			JobProxy		   mUpdateJob;
			bool			   mVisible;

			Time			   mAccumulatedTime;
			ProfilingLineMap   mProfilingLines;


		};


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	} // qsf

#endif
