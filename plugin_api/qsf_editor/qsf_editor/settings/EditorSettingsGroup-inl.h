// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline EditorSettingsGroup::EditorSettingsGroup(SettingsGroupManager* settingsGroupManager) :
			SettingsGroup(settingsGroupManager),
			// General
			mStartupMap("qsf/map/default/empty"),
			mFloatPrecision(3),
			mUiStyle("Fusion"),
			mUiStyleSheet("data/qsf/editor/style_sheets/dark/dark_qsf.qss"),
			// Show
			mShowHiddenComponents(false),
			mExpandObjectProperties(false),
			// Selection
			mSelectEntityOnLayer(false),
			mTerrainRenderViewSelection(false),
			mPasteEntitiesInSourceLayer(false),
			// Log
			mShowLogSeverityLevel(LogMessage::ERROR),
			// Street
			mDefaultStreetCrossingNodeRadius(2.8f),
			mDefaultStreetSectionBaseMesh("sample/mesh/default/street_01c"),	// TODO(co) Might be no good idea to use a QSF sample default asset in here
			mDefaultStreetSectionNodeRadius(7.5f),
			mMinimumStreetTier1To2Distance(0.1f),
			// Terrain modeling
			mTerrainFlattenOffset(0.053f),
			// Asset database
			mAssetPreview(false),
			mAssetImportPreviewEnabled(false),
			mGlobalAssetIdGenerationBase(1),
			mMultiAssetImportEnabled(false),
			mMaterialAssetReimportEnabled(false),
			mRelaxTextureAssetImportSecurityChecks(false),
			mRelaxMeshAssetImportSecurityChecks(false),
			mPlacementMapTestTolerance(0.01f)
		{
			// Nothing to do in here
		}

		inline EditorSettingsGroup::~EditorSettingsGroup()
		{
			// Nothing to do in here
		}

		inline const AssetProxy& EditorSettingsGroup::getStartupMap() const
		{
			return mStartupMap;
		}

		inline void EditorSettingsGroup::setStartupMap(const AssetProxy& startupMap)
		{
			assignAndPromoteChange(mStartupMap, startupMap, STARTUP_MAP);
		}

		inline int EditorSettingsGroup::getFloatPrecision() const
		{
			return mFloatPrecision;
		}

		inline void EditorSettingsGroup::setFloatPrecision(int value)
		{
			assignAndPromoteChange(mFloatPrecision, value, FLOAT_PRECISION);
		}

		inline const std::string& EditorSettingsGroup::getUiStyle() const
		{
			return mUiStyle;
		}

		inline const std::string& EditorSettingsGroup::getUiStyleSheet() const
		{
			return mUiStyleSheet;
		}

		inline bool EditorSettingsGroup::getShowHiddenComponents() const
		{
			return mShowHiddenComponents;
		}

		inline void EditorSettingsGroup::setShowHiddenComponents(bool showHiddenComponents)
		{
			assignAndPromoteChange(mShowHiddenComponents, showHiddenComponents, SHOW_HIDDEN_COMPONENTS);
		}

		inline bool EditorSettingsGroup::getExpandObjectProperties() const
		{
			return mExpandObjectProperties;
		}

		inline void EditorSettingsGroup::setExpandObjectProperties(bool expandObjectProperties)
		{
			assignAndPromoteChange(mExpandObjectProperties, expandObjectProperties, EXPAND_OBJECT_PROPERTIES);
		}

		inline bool EditorSettingsGroup::getSelectEntityOnLayer() const
		{
			return mSelectEntityOnLayer;
		}

		inline void EditorSettingsGroup::setSelectEntityOnLayer(bool selectEntityOnLayer)
		{
			assignAndPromoteChange(mSelectEntityOnLayer, selectEntityOnLayer, SELECT_ENTITY_ON_LAYER);
		}

		inline bool EditorSettingsGroup::getTerrainRenderViewSelection() const
		{
			return mTerrainRenderViewSelection;
		}

		inline void EditorSettingsGroup::setTerrainRenderViewSelection(bool enabled)
		{
			assignAndPromoteChange(mTerrainRenderViewSelection, enabled, TERRAIN_RENDER_VIEW_SELECTION);
		}

		inline bool EditorSettingsGroup::getPasteEntitiesInSourceLayer() const
		{
			return mPasteEntitiesInSourceLayer;
		}

		inline void EditorSettingsGroup::setPasteEntitiesInSourceLayer(bool enabled)
		{
			assignAndPromoteChange(mPasteEntitiesInSourceLayer, enabled, PASTE_ENTITIES_IN_SOURCE_LAYER);
		}

		inline LogMessage::SeverityLevel EditorSettingsGroup::getShowLogSeverityLevel() const
		{
			return mShowLogSeverityLevel;
		}

		inline void EditorSettingsGroup::setShowLogSeverityLevel(LogMessage::SeverityLevel showLogSeverityLevel)
		{
			assignAndPromoteChange(mShowLogSeverityLevel, showLogSeverityLevel, SHOW_LOG_SEVERITY_LEVEL);
		}

		inline const std::vector<LogContextFilterSettings>& EditorSettingsGroup::getLogContextFilterSettings() const
		{
			return mLogContextFilterSettings;
		}

		inline std::vector<LogContextFilterSettings>& EditorSettingsGroup::getLogContextFilterSettings()
		{
			return mLogContextFilterSettings;
		}

		inline float EditorSettingsGroup::getDefaultStreetCrossingNodeRadius() const
		{
			return mDefaultStreetCrossingNodeRadius;
		}

		inline void EditorSettingsGroup::setDefaultStreetCrossingNodeRadius(float defaultStreetCrossingNodeRadius)
		{
			assignAndPromoteChange(mDefaultStreetCrossingNodeRadius, defaultStreetCrossingNodeRadius, DEFAULT_STREET_CROSSING_NODE_RADIUS);
		}

		inline const AssetProxy& EditorSettingsGroup::getDefaultStreetSectionBaseMesh() const
		{
			return mDefaultStreetSectionBaseMesh;
		}

		inline void EditorSettingsGroup::setDefaultStreetSectionBaseMesh(const AssetProxy& defaultStreetSectionBaseMesh)
		{
			assignAndPromoteChange(mDefaultStreetSectionBaseMesh, defaultStreetSectionBaseMesh, DEFAULT_STREET_SECTION_BASE_MESH);
		}

		inline float EditorSettingsGroup::getDefaultStreetSectionNodeRadius() const
		{
			return mDefaultStreetSectionNodeRadius;
		}

		inline void EditorSettingsGroup::setDefaultStreetSectionNodeRadius(float defaultStreetSectionNodeRadius)
		{
			assignAndPromoteChange(mDefaultStreetSectionNodeRadius, defaultStreetSectionNodeRadius, DEFAULT_STREET_SECTION_NODE_RADIUS);
		}

		inline float EditorSettingsGroup::getMinimumStreetTier1To2Distance() const
		{
			return mMinimumStreetTier1To2Distance;
		}

		inline void EditorSettingsGroup::setMinimumStreetTier1To2Distance(float minimumStreetTier1To2Distance)
		{
			assignAndPromoteChange(mMinimumStreetTier1To2Distance, minimumStreetTier1To2Distance, MINIMUM_STREET_TIER_1_TO_2_DISTANCE);
		}

		inline float EditorSettingsGroup::getTerrainFlattenOffset() const
		{
			return mTerrainFlattenOffset;
		}

		inline void EditorSettingsGroup::setTerrainFlattenOffset(float terrainFlattenOffset)
		{
			assignAndPromoteChange(mTerrainFlattenOffset, terrainFlattenOffset, TERRAIN_FLATTEN_OFFSET);
		}

		inline bool EditorSettingsGroup::getAssetPreview() const
		{
			return mAssetPreview;
		}

		inline void EditorSettingsGroup::setAssetPreview(bool assetPreview)
		{
			assignAndPromoteChange(mAssetPreview, assetPreview, ASSET_PREVIEW);
		}

		inline bool EditorSettingsGroup::isAssetImportPreviewEnabled() const
		{
			return mAssetImportPreviewEnabled;
		}

		inline void EditorSettingsGroup::setAssetImportPreviewEnabled(bool assetImportPreviewEnabled)
		{
			assignAndPromoteChange(mAssetImportPreviewEnabled, assetImportPreviewEnabled, ASSET_IMPORT_PREVIEW_ENABLED);
		}

		inline uint64 EditorSettingsGroup::getGlobalAssetIdGenerationBase() const
		{
			return mGlobalAssetIdGenerationBase;
		}

		inline void EditorSettingsGroup::setGlobalAssetIdGenerationBase(uint64 globalAssetIdGenerationBase)
		{
			assignAndPromoteChange(mGlobalAssetIdGenerationBase, globalAssetIdGenerationBase, GLOBAL_ASSET_ID_GENERATION_BASE);
		}

		inline bool EditorSettingsGroup::isMultiAssetImportEnabled() const
		{
			return mMultiAssetImportEnabled;
		}

		inline void EditorSettingsGroup::setMultiAssetImportEnabled(bool multiAssetImportEnabled)
		{
			assignAndPromoteChange(mMultiAssetImportEnabled, multiAssetImportEnabled, MULTI_ASSET_IMPORT_ENABLED);
		}

		inline bool EditorSettingsGroup::isMaterialAssetReimportEnabled() const
		{
			return mMaterialAssetReimportEnabled;
		}

		inline void EditorSettingsGroup::setMaterialAssetReimportEnabled(bool materialAssetReimportEnabled)
		{
			assignAndPromoteChange(mMaterialAssetReimportEnabled, materialAssetReimportEnabled, MATERIAL_ASSET_REIMPORT_ENABLED);
		}

		inline const EditorSettingsGroup::LocalAssetDatasources& EditorSettingsGroup::getLocalAssetDatasources() const
		{
			return mLocalAssetDatasources;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
