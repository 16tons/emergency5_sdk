/*
  -----------------------------------------------------------------------------
  This source file is part of OGRE
  (Object-oriented Graphics Rendering Engine)
  For the latest info, see http://www.ogre3d.org

Copyright (c) 2000-2014 Torus Knot Software Ltd

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
  -----------------------------------------------------------------------------
*/

#ifndef __GL3PlusRenderSystem_H__
#define __GL3PlusRenderSystem_H__

#include "OgreNULLPrerequisites.h"

#include "OgreRenderSystem.h"

namespace Ogre
{
    namespace v1
    {
        class HardwareBufferManager;
    }

    /**
       Implementation of NULL as a rendering system.
    */
    class _OgreNULLExport NULLRenderSystem : public RenderSystem
    {
        bool mInitialized;
        v1::HardwareBufferManager *mHardwareBufferManager;

        ConfigOptionMap mOptions;

        vector<RenderTarget*>::type mRenderTargets;

    public:
        NULLRenderSystem();

        virtual void shutdown(void);

        virtual const String& getName(void) const;
        virtual const String& getFriendlyName(void) const;
        virtual ConfigOptionMap& getConfigOptions(void) { return mOptions; }
        virtual void setConfigOption(const String &name, const String &value) {}

        virtual HardwareOcclusionQuery* createHardwareOcclusionQuery(void);

        virtual String validateConfigOptions(void)  { return BLANKSTRING; }

        virtual RenderSystemCapabilities* createRenderSystemCapabilities(void) const;

        virtual void reinitialise(void);

        virtual RenderWindow* _initialise(bool autoCreateWindow, const String& windowTitle = "OGRE Render Window");

        virtual RenderWindow* _createRenderWindow( const String &name,
                                                   unsigned int width, unsigned int height,
                                                   bool fullScreen,
                                                   const NameValuePairList *miscParams = 0);

        virtual MultiRenderTarget* createMultiRenderTarget(const String & name);

        virtual String getErrorDescription(long errorNumber) const;

        virtual void _useLights(const LightList& lights, unsigned short limit);
        virtual void _setWorldMatrix(const Matrix4 &m);
        virtual void _setViewMatrix(const Matrix4 &m);
        virtual void _setProjectionMatrix(const Matrix4 &m);

        virtual void _setSurfaceParams( const ColourValue &ambient,
                                const ColourValue &diffuse, const ColourValue &specular,
                                const ColourValue &emissive, Real shininess,
                                TrackVertexColourType tracking = TVC_NONE );
        virtual void _setPointSpritesEnabled(bool enabled);
        virtual void _setPointParameters(Real size, bool attenuationEnabled,
            Real constant, Real linear, Real quadratic, Real minSize, Real maxSize);

        virtual void queueBindUAV( uint32 slot, TexturePtr texture,
                                           ResourceAccess::ResourceAccess access = ResourceAccess::ReadWrite,
                                           int32 mipmapLevel = 0, int32 textureArrayIndex = 0,
                                           PixelFormat pixelFormat = PF_UNKNOWN );
        virtual void clearUAVs(void);
        virtual void flushUAVs(void);

        virtual void _setTexture(size_t unit, bool enabled,  Texture *texPtr);

        virtual void _setTextureCoordSet(size_t unit, size_t index);
        virtual void _setTextureCoordCalculation(size_t unit, TexCoordCalcMethod m,
                                                 const Frustum* frustum = 0);
        virtual void _setTextureBlendMode(size_t unit, const LayerBlendModeEx& bm);
        virtual void _setTextureMatrix(size_t unit, const Matrix4& xform);

        virtual void _setIndirectBuffer( IndirectBufferPacked *indirectBuffer );

        virtual DepthBuffer* _createDepthBufferFor( RenderTarget *renderTarget,
                                                    bool exactMatchFormat );

        virtual void _beginFrame(void);
        virtual void _endFrame(void);
        virtual void _setViewport(Viewport *vp);

        virtual void _setHlmsMacroblock( const HlmsMacroblock *macroblock );
        virtual void _setHlmsBlendblock( const HlmsBlendblock *blendblock );
        virtual void _setHlmsSamplerblock( uint8 texUnit, const HlmsSamplerblock *Samplerblock );
        virtual void _setProgramsFromHlms( const HlmsCache *hlmsCache );

        virtual VertexElementType getColourVertexElementType(void) const;
        virtual void _convertProjectionMatrix(const Matrix4& matrix,
            Matrix4& dest, bool forGpuProgram = false) {}
        virtual void _makeProjectionMatrix(const Radian& fovy, Real aspect, Real nearPlane, Real farPlane,
            Matrix4& dest, bool forGpuProgram = false) {}

        virtual void _makeProjectionMatrix(Real left, Real right, Real bottom, Real top,
            Real nearPlane, Real farPlane, Matrix4& dest, bool forGpuProgram = false) {}
        virtual void _makeOrthoMatrix(const Radian& fovy, Real aspect, Real nearPlane, Real farPlane,
            Matrix4& dest, bool forGpuProgram = false) {}
        virtual void _applyObliqueDepthProjection(Matrix4& matrix, const Plane& plane,
            bool forGpuProgram) {}
        virtual void setStencilCheckEnabled(bool enabled) {}

        virtual void setVertexDeclaration(v1::VertexDeclaration* decl);
        virtual void setVertexBufferBinding(v1::VertexBufferBinding* binding);
        virtual void _setVertexArrayObject( const VertexArrayObject *vao );

        virtual void _render( const CbDrawCallIndexed *cmd );
        virtual void _render( const CbDrawCallStrip *cmd );
        virtual void _renderEmulated( const CbDrawCallIndexed *cmd );
        virtual void _renderEmulated( const CbDrawCallStrip *cmd );

        virtual void _setRenderOperation( const v1::CbRenderOp *cmd );
        virtual void _render( const v1::CbDrawCallIndexed *cmd );
        virtual void _render( const v1::CbDrawCallStrip *cmd );

        virtual void bindGpuProgramParameters(GpuProgramType gptype,
            GpuProgramParametersSharedPtr params, uint16 variabilityMask);
        virtual void bindGpuProgramPassIterationParameters(GpuProgramType gptype);

        virtual void clearFrameBuffer(unsigned int buffers,
            const ColourValue& colour = ColourValue::Black,
            Real depth = 1.0f, unsigned short stencil = 0);
        virtual void discardFrameBuffer( unsigned int buffers );

        virtual Real getHorizontalTexelOffset(void);
        virtual Real getVerticalTexelOffset(void);
        virtual Real getMinimumDepthInputValue(void);
        virtual Real getMaximumDepthInputValue(void);

        virtual void _setRenderTarget(RenderTarget *target, bool colourWrite);
        virtual void preExtraThreadsStarted();
        virtual void postExtraThreadsStarted();
        virtual void registerThread();
        virtual void unregisterThread();
        virtual unsigned int getDisplayMonitorCount() const     { return 1; }

        virtual void beginProfileEvent( const String &eventName );
        virtual void endProfileEvent( void );
        virtual void markProfileEvent( const String &event );

        virtual bool hasAnisotropicMipMapFilter() const         { return true; }

        virtual void setClipPlanesImpl(const PlaneList& clipPlanes);
        virtual void initialiseFromRenderSystemCapabilities(RenderSystemCapabilities* caps, RenderTarget* primary);
    };
}

#endif
