// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/materialblueprint/cache/PipelineStateCompiler.h"
#include "qsf/job/JobProxy.h"

#include <OGRE/OgreGpuProgramParams.h>
#include <OGRE/OgreGpuProgramManager.h>

#include <deque>
#include <mutex>
#include <thread>
#include <vector>
#include <condition_variable>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class GL3PlusContext;
}
namespace qsf
{
	class MaterialBlueprint;
	class ShaderSourceCodeCache;
	namespace Renderer
	{
		struct RootSignature;
	}
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
	*    OpenGL pipeline state compiler class for separate programs (recommended approach)
	*/
	class PipelineStateCompilerSeparateOpenGL : public PipelineStateCompiler
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MaterialSystem;	///< Manages the pipeline state compiler instance


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct CompilerRequestOpenGL : public CompilerRequest
		{
			MaterialBlueprint*				   materialBlueprint;
			const ShaderSourceCodeCache*	   shaderSourceCodeCache[Ogre::NumShaderTypes];
			uint32							   openGLShaderPrograms[Ogre::NumShaderTypes];	///< "GLuint"
			Ogre::GpuProgramManager::Microcode ogreGpuProgramManagerMicrocode;

			CompilerRequestOpenGL(PipelineStateCache& _pipelineStateCache) :
				CompilerRequest(_pipelineStateCache),
				materialBlueprint(nullptr)
			{
				memset(&shaderSourceCodeCache[0], 0, sizeof(ShaderSourceCodeCache*) * Ogre::NumShaderTypes);
				memset(&openGLShaderPrograms[0], 0, sizeof(uint32) * Ogre::NumShaderTypes);
			}
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void setGlslProgramBindingPointsFromRootSignature(uint32 glProgramHandle, const qsf::Renderer::RootSignature& rootSignature);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::PipelineStateCompiler methods     ]
	//[-------------------------------------------------------]
	public:
		virtual uint32 getNumberOfCompilerThreads() const override;
		virtual void setNumberOfCompilerThreads(uint32 numberOfCompilerThreads) override;
		virtual void setCompilationEnabled(bool compilationEnabled) override;
		virtual void setAsynchronousCompilationEnabled(bool enabled) override;
		virtual void flushBuilderQueue() override;
		virtual void flushCompilerQueue() override;
		virtual void flushAllQueues() override;
		virtual void clearAllQueues() override;
		virtual void addCompilerRequest(PipelineStateCache& pipelineStateCache) override;
		virtual void instantCompilerRequest(PipelineStateCache& pipelineStateCache) override;
		virtual bool isWorking() override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PipelineStateCompilerSeparateOpenGL();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PipelineStateCompilerSeparateOpenGL();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<std::thread>		  CompilerThreads;
		typedef std::deque<CompilerRequestOpenGL> CompilerRequests;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void builderThreadWorker();
		void compilerThreadWorker();
		void dispatchJob(const JobArguments& jobArguments);
		void clearCompilerRequests(CompilerRequests& compilerRequests);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32				  mNumberOfCompilerThreads;
		Ogre::GL3PlusContext* mGl3PlusContext;

		// Asynchronous building (moderate cost)
		std::atomic<bool>		mShutdownBuilderThread;
		std::mutex				mBuilderMutex;
		std::condition_variable	mBuilderConditionVariable;
		CompilerRequests		mBuilderQueue;
		std::thread				mBuilderThread;

		// Asynchronous compilation (nuts cost)
		std::atomic<bool>		mShutdownCompilerThread;
		std::mutex				mCompilerMutex;
		std::condition_variable	mCompilerConditionVariable;
		CompilerRequests		mCompilerQueue;
		CompilerThreads			mCompilerThreads;

		// Synchronous dispatch
		std::mutex		 mDispatchMutex;
		CompilerRequests mDispatchQueue;
		JobProxy		 mDispatchJobProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
