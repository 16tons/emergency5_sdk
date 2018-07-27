// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/noncopyable.hpp>

#include <atomic>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class PipelineStateCache;
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
	*    Abstract pipeline state compiler class
	*
	*  @note
	*    - Takes care of asynchronous pipeline state compilation
	*/
	class PipelineStateCompiler : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MaterialSystem;	///< Manages the pipeline state compiler instance


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum class Optimization
		{
			Skip = 0,	///< No optimization, usually only used for debugging
			Level0,		///< Low optimization
			Level1,		///< Medium optimization
			Level2,		///< High optimization
			Level3		///< Ultra optimization
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline Optimization getOptimization() const;
		inline void setOptimization(Optimization optimization);
		inline bool isCompilationEnabled() const;
		inline bool isAsynchronousCompilationEnabled() const;
		inline uint32 getNumberOfActiveCompilerRequests() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::PipelineStateCompiler methods     ]
	//[-------------------------------------------------------]
	public:
		virtual uint32 getNumberOfCompilerThreads() const = 0;
		virtual void setNumberOfCompilerThreads(uint32 numberOfCompilerThreads) = 0;
		virtual void setCompilationEnabled(bool compilationEnabled) = 0;
		virtual void setAsynchronousCompilationEnabled(bool enabled) = 0;
		virtual void flushBuilderQueue() = 0;
		virtual void flushCompilerQueue() = 0;
		virtual void flushAllQueues() = 0;
		virtual void clearAllQueues() = 0;	// Brutally cancels all compiler requests
		virtual void addCompilerRequest(PipelineStateCache& pipelineStateCache) = 0;		// Asynchronous
		virtual void instantCompilerRequest(PipelineStateCache& pipelineStateCache) = 0;	// Synchronous
		virtual bool isWorking() = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		inline PipelineStateCompiler();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PipelineStateCompiler();


	//[-------------------------------------------------------]
	//[ Protected definitions                                 ]
	//[-------------------------------------------------------]
	protected:
		struct CompilerRequest
		{
			PipelineStateCache& pipelineStateCache;

			CompilerRequest(PipelineStateCache& _pipelineStateCache) :
				pipelineStateCache(_pipelineStateCache)
			{
				// Nothing here
			}
		};


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		std::atomic<Optimization> mOptimization;
		std::atomic<bool>		  mCompilationEnabled;
		bool					  mAsynchronousCompilationEnabled;
		uint32					  mNumberOfActiveCompilerRequests;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/materialblueprint/cache/PipelineStateCompiler-inl.h"
