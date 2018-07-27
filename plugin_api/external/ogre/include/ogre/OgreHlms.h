/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

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
#ifndef _OgreHlms_H_
#define _OgreHlms_H_

#include "OgreStringVector.h"
#include "OgreHlmsCommon.h"
#if !OGRE_NO_JSON
    #include "OgreHlmsJson.h"
#endif
#include "OgreHeaderPrefix.h"

namespace Ogre
{
    class CompositorShadowNode;
    struct QueuedRenderable;

    /** \addtogroup Core
    *  @{
    */
    /** \addtogroup Resources
    *  @{
    */

    /** HLMS stands for "High Level Material System". */
    class _OgreExport Hlms : public PassAlloc
    {
        friend class HlmsManager; // "Ogre::HlmsManager::registerHlms()" needs to be able to call "Ogre::Hlms::enumeratePieceFiles()"
    public:
        enum LightGatheringMode
        {
            LightGatherForward,
            LightGatherForwardPlus,
            LightGatherDeferred,
            LightGatherNone,
        };

        struct DatablockEntry
        {
            HlmsDatablock   *datablock;
            bool            visibleToManager;
            String          name;
            DatablockEntry() : datablock( 0 ), visibleToManager( false ) {}
            DatablockEntry( HlmsDatablock *_datablock, bool _visibleToManager, const String &_name ) :
                datablock( _datablock ), visibleToManager( _visibleToManager ), name( _name ) {}
        };

        typedef std::map<IdString, DatablockEntry> HlmsDatablockMap;

    protected:
        typedef std::map<IdString, String> PiecesMap;

        struct RenderableCache
        {
            HlmsPropertyVec setProperties;
            PiecesMap       pieces[NumShaderTypes];

            RenderableCache( const HlmsPropertyVec &properties,
                             const PiecesMap *_pieces ) :
                setProperties( properties )
            {
                if( _pieces )
                {
                    for( size_t i=0; i<NumShaderTypes; ++i )
                        pieces[i] = _pieces[i];
                }
            }

            bool operator == ( const RenderableCache &_r ) const
            {
                bool piecesEqual = true;
                for( size_t i=0; i<NumShaderTypes; ++i )
                    piecesEqual &= pieces[i] == _r.pieces[i];

                return setProperties == _r.setProperties && piecesEqual;
            }
        };

        typedef vector<HlmsPropertyVec>::type HlmsPropertyVecVec;
        typedef vector<RenderableCache>::type RenderableCacheVec;

        HlmsPropertyVecVec  mPassCache;
        RenderableCacheVec  mRenderableCache;
        HlmsCacheVec        mShaderCache;

        HlmsPropertyVec mSetProperties;
        PiecesMap       mPieces;

    public:
        struct Library
        {
            Archive         *dataFolder;
            StringVector    pieceFiles[NumShaderTypes];
        };

        typedef vector<Library>::type LibraryVec;
    protected:
        LibraryVec      mLibrary;
        Archive         *mDataFolder;
        StringVector    mPieceFiles[NumShaderTypes];
        HlmsManager     *mHlmsManager;

        LightGatheringMode  mLightGatheringMode;
        uint16              mNumLightsLimit;

        /// Listener for adding extensions. @see setListener.
        /// Pointer is [b]never[/b] null.
        HlmsListener    *mListener;
        RenderSystem    *mRenderSystem;

        HlmsDatablockMap mDatablocks;

        typedef vector<IdString>::type IdStringVec;

        String          mShaderProfile; /// "glsl", "glsles", "hlsl"
        IdStringVec     mRsSpecificExtensions;
        String const    *mShaderTargets[NumShaderTypes]; ///[0] = "vs_4_0", etc. Only used by D3D
        String          mShaderFileExt; /// Either glsl or hlsl
        String          mOutputPath;
        bool            mDebugOutput;
        bool            mHighQuality;

        /// The default datablock occupies the name IdString(); which is not the same as IdString("")
        HlmsDatablock   *mDefaultDatablock;

        HlmsTypes       mType;
        IdString        mTypeName;
        String          mTypeNameStr;

        /** Inserts common properties about the current Renderable,
            such as hlms_skeleton hlms_uv_count, etc
        */
        void setCommonProperties();

        /** Populates all mPieceFiles with all files in mDataFolder with suffix ending in
                piece_vs    - Vertex Shader
                piece_ps    - Pixel Shader
                piece_gs    - Geometry Shader
                piece_hs    - Hull Shader
                piece_ds    - Domain Shader
            Case insensitive.
        */
        virtual void enumeratePieceFiles(void);
        /// Populates pieceFiles, returns true if found at least one piece file.
        static bool enumeratePieceFiles( Archive *dataFolder, StringVector *pieceFiles );

        void setProperty( IdString key, int32 value );
        int32 getProperty( IdString key, int32 defaultVal=0 ) const;

        enum ExpressionType
        {
            EXPR_OPERATOR_OR,        //||
            EXPR_OPERATOR_AND,       //&&
            EXPR_OBJECT,             //(...)
            EXPR_VAR
        };

        struct Expression
        {
            bool                    result;
            bool                    negated;
            ExpressionType          type;
            std::vector<Expression> children;
            String                  value;

            Expression() : result( false ), negated( false ), type( EXPR_VAR ) {}
        };

        typedef std::vector<Expression> ExpressionVec;

        static void copy( String &outBuffer, const SubStringRef &inSubString, size_t length );
        static void repeat( String &outBuffer, const SubStringRef &inSubString, size_t length,
                            size_t passNum, const String &counterVar );
        bool parseMath( const String &inBuffer, String &outBuffer );
        bool parseForEach( const String &inBuffer, String &outBuffer ) const;
        bool parseProperties( String &inBuffer, String &outBuffer ) const;
        bool collectPieces( const String &inBuffer, String &outBuffer );
        bool insertPieces( String &inBuffer, String &outBuffer ) const;
        bool parseCounter( const String &inBuffer, String &outBuffer );
        bool parse( const String &inBuffer, String &outBuffer ) const;

        /** Goes through 'buffer', starting from startPos (inclusive) looking for the given
            character while skipping whitespace. If any character other than whitespace or
            EOLs if found returns String::npos
        @return
            String::npos if not found or wasn't the next character. If found, the position
            in the buffer, from start
        */
        static size_t findNextCharacter( const String &buffer, size_t startPos, char character );

        static void findBlockEnd( SubStringRef &outSubString , bool &syntaxError );

        bool evaluateExpression( SubStringRef &outSubString, bool &outSyntaxError ) const;
        bool evaluateExpressionRecursive( ExpressionVec &expression, bool &outSyntaxError ) const;
        static size_t evaluateExpressionEnd( const SubStringRef &outSubString );

        static void evaluateParamArgs( SubStringRef &outSubString, StringVector &outArgs,
                                       bool &outSyntaxError );

        static size_t calculateLineCount(const String &buffer, size_t idx );
        static size_t calculateLineCount( const SubStringRef &subString );

        /** Caches a set of properties (i.e. key-value pairs) & snippets of shaders. If an
            exact entry exists in the cache, its index is returned. Otherwise a new entry
            will be created.
        @param renderableSetProperties
            A vector containing key-value pairs of data
        @param pieces
            Shader snippets for each type of shader. Can be null. When not null, must hold
            NumShaderTypes entries, i.e. String val = pieces[NumShaderTypes-1][IdString]
        @return
            The index to the cache entry.
        */
        size_t addRenderableCache( const HlmsPropertyVec &renderableSetProperties,
                                   const PiecesMap *pieces );

        /// Retrieves a cache entry using the returned value from @addRenderableCache
        const RenderableCache& getRenderableCache( uint32 hash ) const;

        const HlmsCache* addShaderCache( uint32 hash, GpuProgramPtr &vertexShader,
                                         GpuProgramPtr &geometryShader,
                                         GpuProgramPtr &tesselationHullShader,
                                         GpuProgramPtr &tesselationDomainShader,
                                         GpuProgramPtr &pixelShader );
        const HlmsCache* getShaderCache( uint32 hash ) const;
        void clearShaderCache(void);

        void processPieces( Archive *archive, const StringVector &pieceFiles );

        /** Creates a shader based on input parameters. Caller is responsible for ensuring
            this shader hasn't already been created.
            Shader template files will be processed and then compiled.
        @param renderableHash
            The hash calculated in from @calculateHashFor that lives in @Renderable
        @param passCache
            The return value of @preparePassHash
        @param finalHash
            A hash calculated on the pass' & renderable' hash. Must be unique. Caller is
            responsible for ensuring this hash stays unique.
        @param queuedRenderable
            The renderable who owns the renderableHash. Not used by the base class, but
            derived implementations may overload this function and take advantage of
            some of the direct access it provides.
        @return
            The newly created shader.
        */
        virtual const HlmsCache* createShaderCacheEntry( uint32 renderableHash,
                                                         const HlmsCache &passCache,
                                                         uint32 finalHash,
                                                         const QueuedRenderable &queuedRenderable );

        virtual HlmsDatablock* createDatablockImpl( IdString datablockName,
                                                    const HlmsMacroblock *macroblock,
                                                    const HlmsBlendblock *blendblock,
                                                    const HlmsParamVec &paramVec );

        virtual HlmsDatablock* createDefaultDatablock(void);
        void _destroyAllDatablocks(void);

        inline void calculateHashForSemantic( VertexElementSemantic semantic, VertexElementType type,
                                              uint16 index, uint &inOutNumTexCoords );
        uint16 calculateHashForV1( Renderable *renderable );
        uint16 calculateHashForV2( Renderable *renderable );

        virtual void calculateHashForPreCreate( Renderable *renderable, PiecesMap *inOutPieces ) {}
        virtual void calculateHashForPreCaster( Renderable *renderable, PiecesMap *inOutPieces ) {}

        HlmsCache preparePassHashBase( const Ogre::CompositorShadowNode *shadowNode,
                                       bool casterPass, bool dualParaboloid,
                                       SceneManager *sceneManager );

    public:
        /**
        @param libraryFolders
            Path to folders to be processed first for collecting pieces. Will be processed in order.
            Pointer can be null.
        */
        Hlms(HlmsTypes type, const String &typeName, Archive *dataFolder,
              ArchiveVec *libraryFolders );
        virtual ~Hlms();

        HlmsTypes getType(void) const                       { return mType; }
        IdString getTypeName(void) const                    { return mTypeName; }
        const String& getTypeNameStr(void) const            { return mTypeNameStr; }
        void _notifyManager( HlmsManager *manager )         { mHlmsManager = manager; }
        HlmsManager* getHlmsManager(void) const             { return mHlmsManager; }

        /** Sets the quality of the Hlms. This function is most relevant for mobile and
            almost or completely ignored by Desktop.
            The default value is false.
        @par
            On mobile, high quality will use "highp" quality precision qualifier for
            all its variables and functions.
            When not in HQ, mobile users may see aliasing artifacts, gradients; but
            the performance impact can be quite high. Some GPU drivers might even
            refuse to execute the shader as they cannot handle it.
        @par
            Unless you absolutely require high quality rendering on Mobile devices
            and/or to get it to look as closely as possible as it looks in a Desktop
            device, the recommended option is to have this off.
        */
        void setHighQuality( bool highQuality );
        bool getHighQuality(void) const                     { return mHighQuality; }

#if !OGRE_NO_JSON
        /** Loads datablock values from a JSON value. @see HlmsJson.
        @param jsonValue
            JSON Object containing the definition of this datablock.
        @param blocks
            All the loaded Macro-, Blend- & Samplerblocks the JSON has
            defined and may be referenced by the datablock declaration.
        @param datablock
            Datablock to fill the values.
        */
        virtual void _loadJson( const rapidjson::Value &jsonValue, const HlmsJson::NamedBlocks &blocks,
                                HlmsDatablock *datablock ) const {}
        virtual void _saveJson( const HlmsDatablock *datablock, String &outString ) const {}

        virtual void _collectSamplerblocks( set<const HlmsSamplerblock*>::type &outSamplerblocks,
                                            const HlmsDatablock *datablock ) const {}
#endif

        /** Destroys all the cached shaders and in the next opportunity will recreate them
            from the new location. This is very useful for fast iteration and real-time
            editing of Hlms shader templates.
        @remarks
            Calling with null pointer is possible and will only invalidate existing shaders
            but you should provide a valid pointer before we start generating the first
            shader (or else crash).
        @par
            Existing datablock materials won't be reloaded from files, so their properties
            won't change (i.e. changed from blue to red), but the shaders will.
        @param libraryFolders
            When null pointer, the library folders paths won't be changed at all
            (but still will be reloaded).
            When non-null pointer, the library folders will be overwriten.
            Pass an empty container if you want to stop using libraries.
        */
        virtual void reloadFrom( Archive *newDataFolder, ArchiveVec *libraryFolders=0 );

        Archive* getDataFolder(void)                        { return mDataFolder; }
        const LibraryVec& getPiecesLibrary(void) const      { return mLibrary; }

        /** Creates a unique datablock that can be shared by multiple renderables.
        @remarks
            The name of the datablock must be in paramVec["name"] and must be unique
            Throws if a datablock with the same name paramVec["name"] already exists
        @param name
            Name of the Datablock, must be unique within all Hlms types, not just this one.
            99% you want this to be IdString( refName ); however this is not enforced.
        @param refName
            Name of the Datablock. The engine doesn't use this value at all. It is only
            useful for UI editors which want to enumerate all existing datablocks and
            display its name to the user.
        @param macroblockRef
            @See HlmsManager::getMacroblock
        @param blendblockRef
            @See HlmsManager::getBlendblock
        @param paramVec
            Key - String Value list of paramters. MUST BE SORTED.
        @param visibleToManager
            When false, HlmsManager::getDatablock won't find this datablock. True by default
        @return
            Pointer to created Datablock
        */
        HlmsDatablock* createDatablock( IdString name, const String &refName,
                                        const HlmsMacroblock &macroblockRef,
                                        const HlmsBlendblock &blendblockRef,
                                        const HlmsParamVec &paramVec,
                                        bool visibleToManager=true );

        /** Finds an existing datablock based on its name (@see createDatablock)
        @return
            The datablock associated with that name. Null pointer if not found. Doesn't throw.
        */
        HlmsDatablock* getDatablock( IdString name ) const;

        /// Returns the string name associated with its hashed name (this was
        /// passed as refName in @createDatablock). Returns null ptr if
        /// not found.
        /// The reason this String doesn't live in HlmsDatablock is to prevent
        /// cache trashing (datablocks are hot iterated every frame, and the
        /// full name is rarely ever used)
        const String* getFullNameString( IdString name ) const;

        /** Destroys a datablocks given its name. Caller is responsible for ensuring
            those pointers aren't still in use (i.e. dangling pointers)
        @remarks
            Throws if no datablock with the given name exists.
        */
        void destroyDatablock( IdString name );

        /// Destroys all datablocks created with @createDatablock. Caller is responsible
        /// for ensuring those pointers aren't still in use (i.e. dangling pointers)
        /// The default datablock will be recreated.
        void destroyAllDatablocks(void);

        /// @copydoc HlmsManager::getDefaultDatablock
        HlmsDatablock* getDefaultDatablock(void) const;

        /// Returns all datablocks owned by this Hlms, including the default one.
        const HlmsDatablockMap& getDatablockMap(void) const { return mDatablocks; }

        /** Finds the parameter with key 'key' in the given 'paramVec'. If found, outputs
            the value to 'inOut', otherwise leaves 'inOut' as is.
        @return
            True if the key was found (inOut was modified), false otherwise
        @remarks
            Assumes paramVec is sorted by key.
        */
        static bool findParamInVec( const HlmsParamVec &paramVec, IdString key, String &inOut );

        /** Called by the renderable when either it changes the material,
            or its properties change (e.g. the mesh' uvs are stripped)
        @param renderable
            The renderable the material will be used on.
        @param movableObject
            The MovableObject the material will be used on (usually the parent of renderable)
        @return
            A hash. This hash references property parameters that are already cached.
        */
        virtual void calculateHashFor( Renderable *renderable, uint32 &outHash, uint32 &outCasterHash );

        /** Called every frame by the Render Queue to cache the properties needed by this
            pass. i.e. Number of PSSM splits, number of shadow casting lights, etc
        @param shadowNode
            The shadow node currently in effect. Can be null.
        @return
            A hash and cached property parameters. Unlike @calculateHashFor, the cache
            must be kept by the caller and not by us (because it may change every frame
            and is one for the whole pass, but Mesh' properties usually stay consistent
            through its lifetime but may differ per mesh)
        */
        virtual HlmsCache preparePassHash( const Ogre::CompositorShadowNode *shadowNode,
                                           bool casterPass, bool dualParaboloid,
                                           SceneManager *sceneManager );

        /** Retrieves an HlmsCache filled with the GPU programs to be used by the given
            renderable. If the shaders have already been created (i.e. whether for this
            renderable, or another one) it gets them from a cache. Otherwise we create it.
            It assumes that renderable->setHlms( this, parameters ) has already called.
        @param lastReturnedValue
            The last value returned by getMaterial.
        @param passCache
            The cache returned by @preparePassHash.
        @param renderable
            The renderable the caller wants us to give the shaders.
        @param movableObject
            The MovableObject owner of the renderable (we need it to know if renderable
            should cast shadows)
        @param casterPass
            True if this pass is the shadow mapping caster pass, false otherwise
        @return
            Structure containing all necessary shaders
        */
        virtual const HlmsCache* getMaterial( HlmsCache const *lastReturnedValue, const HlmsCache &passCache,
                                              const QueuedRenderable &queuedRenderable, bool casterPass );

        /** Fills the constant buffers. Gets executed right before drawing the mesh.
        @param cache
            Current cache of Shaders to be used.
        @param queuedRenderable
            The Renderable-MovableObject pair about to be rendered.
        @param casterPass
            Whether this is a shadow mapping caster pass.
        @param lastCacheHash
            The hash of the cache of shaders that was the used by the previous renderable.
        @param lastTextureHash
            Last Texture Hash, used to let the Hlms know whether the textures should be changed again
        @return
            New Texture hash (may be equal or different to lastTextureHash).
        */
        virtual uint32 fillBuffersFor( const HlmsCache *cache, const QueuedRenderable &queuedRenderable,
                                       bool casterPass, uint32 lastCacheHash,
                                       uint32 lastTextureHash ) = 0;

        virtual uint32 fillBuffersForV1( const HlmsCache *cache,
                                         const QueuedRenderable &queuedRenderable,
                                         bool casterPass, uint32 lastCacheHash,
                                         CommandBuffer *commandBuffer ) = 0;

        virtual uint32 fillBuffersForV2( const HlmsCache *cache,
                                         const QueuedRenderable &queuedRenderable,
                                         bool casterPass, uint32 lastCacheHash,
                                         CommandBuffer *commandBuffer ) = 0;

        /// This gets called right before executing the command buffer.
        virtual void preCommandBufferExecution( CommandBuffer *commandBuffer ) {}
        /// This gets called after executing the command buffer.
        virtual void postCommandBufferExecution( CommandBuffer *commandBuffer ) {}

        /// Called when the frame has fully ended (ALL passes have been executed to all RTTs)
        virtual void frameEnded(void) {}

        /** Call to output the automatically generated shaders (which are usually made from templates)
            on the given folder for inspection, analyzing, debugging, etc.
        @remarks
            The shader will be dumped when it is generated, not when this function gets called.
            You should call this function at start up
        @param enableDebugOutput
            Whether to enable or disable dumping the shaders into a folder
        @param path
            Path location on where to dump it. Should end with slash for proper concatenation
            (i.e. C:/path/ instead of C:/path; or /home/user/ instead of /home/user)
        */
        void setDebugOutputPath( bool enableDebugOutput, const String &path = BLANKSTRING );

        /** Sets a listener to extend an existing Hlms implementation's with custom code,
            without having to rewrite it or modify the source code directly.
        @remarks
            Other alternatives for extending an existing implementation is to derive
            from the class and override particular virtual functions.
            For performance reasons, listeners are never called on a per-object basis.
            Consult the section "Customizing an existing implementation" from the
            manual in the Docs/2.0 folder.
        @param listener
            Listener pointer. Use null to disable.
        */
        void setListener( HlmsListener *listener );

        /** Returns the current listener. @see setListener();
        @remarks
            If the default listener is being used (that does nothing) then null is returned.
        */
        HlmsListener* getListener(void) const;

        /// For debugging stuff. I.e. the Command line uses it for testing manually set properties
        void _setProperty( IdString key, int32 value )      { setProperty( key, value ); }

        /// Utility helper, mostly useful to HlmsListener implementations.
        static int32 getProperty( const HlmsPropertyVec &properties,
                                  IdString key, int32 defaultVal=0 );

        /// Internal use. @see HlmsManager::setShadowMappingUseBackFaces
        void _notifyShadowMappingBackFaceSetting(void);

        virtual void _changeRenderSystem( RenderSystem *newRs );

        RenderSystem* getRenderSystem(void) const           { return mRenderSystem; }
    };

    /// These are "default" or "Base" properties common to many implementations and thus defined here.
    /// Most of them start with the suffix hlms_
    struct _OgreExport HlmsBaseProp
    {
        static const IdString Skeleton;
        static const IdString BonesPerVertex;
        static const IdString Pose;

        static const IdString Normal;
        static const IdString QTangent;
        static const IdString Tangent;

        static const IdString Colour;

        static const IdString IdentityWorld;
        static const IdString IdentityViewProj;
        /// When this is set, the value of IdentityViewProj is meaningless.
        static const IdString IdentityViewProjDynamic;

        static const IdString UvCount;
        static const IdString UvCount0;
        static const IdString UvCount1;
        static const IdString UvCount2;
        static const IdString UvCount3;
        static const IdString UvCount4;
        static const IdString UvCount5;
        static const IdString UvCount6;
        static const IdString UvCount7;

        //Change per frame (grouped together with scene pass)
        static const IdString LightsDirectional;
        static const IdString LightsDirNonCaster;
        static const IdString LightsPoint;
        static const IdString LightsSpot;
        static const IdString LightsAttenuation;
        static const IdString LightsSpotParams;

        //Change per scene pass
        static const IdString DualParaboloidMapping;
        static const IdString NumShadowMaps;
        static const IdString PssmSplits;
        static const IdString ShadowCaster;
        static const IdString ShadowUsesDepthTexture;
        static const IdString Forward3D;
        static const IdString Forward3DFlipY;
        static const IdString Forward3DDebug;
        static const IdString Forward3DFadeAttenRange;
        static const IdString VPos;

        //Change per material (hash can be cached on the renderable)
        static const IdString AlphaTest;
        static const IdString AlphaBlend;

        static const IdString GL3Plus;
        static const IdString HighQuality;
        static const IdString TexGather;
        static const IdString DisableStage;

        //Useful GL Extensions
        static const IdString GlAmdTrinaryMinMax;

        static const IdString *UvCountPtrs[8];
    };

    struct _OgreExport HlmsBasePieces
    {
        static const IdString AlphaTestCmpFunc;
    };

    /** @} */
    /** @} */

}

#include "OgreHeaderSuffix.h"

#endif
