// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PipelineStateCompiler::Optimization PipelineStateCompiler::getOptimization() const
	{
		return mOptimization;
	}

	inline void PipelineStateCompiler::setOptimization(Optimization optimization)
	{
		mOptimization = optimization;
	}

	inline bool PipelineStateCompiler::isCompilationEnabled() const
	{
		return mCompilationEnabled;
	}

	inline bool PipelineStateCompiler::isAsynchronousCompilationEnabled() const
	{
		return mAsynchronousCompilationEnabled;
	}

	inline uint32 PipelineStateCompiler::getNumberOfActiveCompilerRequests() const
	{
		return mNumberOfActiveCompilerRequests;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline PipelineStateCompiler::PipelineStateCompiler() :
		mOptimization(Optimization::Level2),
		mCompilationEnabled(true),
		mAsynchronousCompilationEnabled(false),
		mNumberOfActiveCompilerRequests(0)
	{
		// Nothing to do in here
	}

	inline PipelineStateCompiler::~PipelineStateCompiler()
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
