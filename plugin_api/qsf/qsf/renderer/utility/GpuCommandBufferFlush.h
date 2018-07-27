// From http://www.ogre3d.org/tikiwiki/tiki-index.php?page=FlushGPUBuffer&highlight=GpuCommandBufferFlush

#ifndef __GPUCOMMANDBUFFERFLUSH_H__
#define __GPUCOMMANDBUFFERFLUSH_H__

#include <OGRE/OgrePrerequisites.h>
#include <OGRE/OgreFrameListener.h>

namespace Ogre
{

	/**
	* @brief
	*   Helper class which can assist you in making sure the -GPU command buffer is regularly flushed, so in cases where the -CPU is outpacing the
	*   GPU we do not hit a situation where the -CPU suddenly has to stall to wait for more space in the buffer
	*
	*  @see
	*  - http://www.ogre3d.org/tikiwiki/FlushGPUBuffer
	*  - http://www.ogre3d.org/forums/viewtopic.php?f=5&t=50486
    */
    class GpuCommandBufferFlush : public FrameListener, public Ogre::RenderSystem::Listener
    {
    protected:
        bool mUseOcclusionQuery;
        typedef std::vector<HardwareOcclusionQuery*> HOQList;
        HOQList mHOQList;
        size_t mMaxQueuedFrames;
        size_t mCurrentFrame;
        bool mStartPull;
        bool mStarted;
		bool mDeviceLostSinceFrameStarted;

    public:
        GpuCommandBufferFlush();
        virtual ~GpuCommandBufferFlush();

        void start(size_t maxQueuedFrames = 2);
        void stop();
        bool frameStarted(const FrameEvent& evt) override;
        bool frameEnded(const FrameEvent& evt) override;

	// Public virtual "Ogre::RenderSystem::Listener" methods
	public:
		virtual void eventOccurred(const Ogre::String& eventName, const Ogre::NameValuePairList* parameters = nullptr) override;

    };

}

#endif
