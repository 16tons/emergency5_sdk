// Copyright (C) 2012-2018 Promotion Software GmbH
#ifdef QSF_PROFILING


	//[-------------------------------------------------------]
	//[ Header guard                                          ]
	//[-------------------------------------------------------]
	#pragma once


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	namespace qsf
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline ScopedProfilingZone::ScopedProfilingZone(ProfilingElement& element) :
			mElement(element)
		{
			// Nothing here
		}

		inline ScopedProfilingZone::~ScopedProfilingZone()
		{
			mElement.registerUpdate(mStopwatch.getElapsed(), mScopedMemoryZone.getConsumed());
		}


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	} // qsf
#endif
