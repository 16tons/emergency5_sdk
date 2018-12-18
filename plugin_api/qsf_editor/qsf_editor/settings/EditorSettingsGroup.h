// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/log/LogSystem.h>
#include <qsf/base/UniqueInstance.h>
#include <qsf/base/PropertyHelper.h>
#include <qsf/base/GetUninitialized.h>
#include <qsf/settings/SettingsGroup.h>
#include <qsf/reflection/type/CampQsfAssetProxy.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		// TODO(mk) cleanup this class
		class LogContextFilterSettings
		{
		public:
			LogContextFilterSettings() :
				contextId(getUninitialized<uint32>()),
				active(true),
				error(true),
				warning(true),
				info(true),
				debug(true),
				trace(true)
			{}

			LogContextFilterSettings(uint32 ContextId) :
				contextId(ContextId),
				active(true),
				error(true),
				warning(true),
				info(true),
				debug(true),
				trace(true)
			{}

			virtual ~LogContextFilterSettings() {}

			uint32 contextId;
			std::string plainText;
			bool active;
			bool error;
			bool warning;
			bool info;
			bool debug;
			bool trace;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    QSF editor settings group implementation
		*
		*  @todo
		*    - TODO(co) For now, we only have a single camera mode for a single render view, later on there might be multiple render views, each with its own camera mode
		*/
		class QSF_EDITOR_API_EXPORT EditorSettingsGroup : public SettingsGroup, public qsf::UniqueInstance<EditorSettingsGroup>
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class GeneralMetaclasses;	// For the declaration of the CAMP metaclass, the plugin needs direct access to "qsf::editor::EditorSettingsGroup::mLogContextFilterSettings" in order to connect it as CAMP array


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;							///< "qsf::editor::RenderingSettingsGroup" unique pluginable settings group ID
			// General
			static const uint32 STARTUP_MAP;							///< "StartupMap" unique class property ID inside the class
			static const uint32 FLOAT_PRECISION;						///< "FloatPrecision" unique class property ID inside the class
			// UI
			static const uint32 UI_STYLE;								///< "UiStyle" unique class property ID inside the class
			static const uint32 UI_STYLE_SHEET;							///< "UiStyleSheet" unique class property ID inside the class
			// Show
			static const uint32 SHOW_HIDDEN_COMPONENTS;					///< "ShowHiddenComponents" unique class property ID inside the class
			static const uint32 EXPAND_OBJECT_PROPERTIES;				///< "ExpandObjectProperties" unique class property ID inside the class
			// Log
			static const uint32 SHOW_LOG_SEVERITY_LEVEL;				///< "ShowLogSeverityLevel" unique class property ID inside the class
			// Selection
			static const uint32 SELECT_ENTITY_ON_LAYER;					///< "SelectEntityOnLayer" unique class property ID inside the class
			static const uint32 TERRAIN_RENDER_VIEW_SELECTION;			///< "TerrainRenderViewSelection" unique class property ID inside the class
			static const uint32 PASTE_ENTITIES_IN_SOURCE_LAYER;			///< "PasteEntitiesInSourceLayer" unique class property ID inside the class
			// Street
			static const uint32 DEFAULT_STREET_CROSSING_NODE_RADIUS;	///< "DefaultStreetCrossingNodeRadius" unique class property ID inside the class
			static const uint32 DEFAULT_STREET_SECTION_BASE_MESH;		///< "DefaultStreetSectionBaseMesh" unique class property ID inside the class
			static const uint32 DEFAULT_STREET_SECTION_NODE_RADIUS;		///< "DefaultStreetSectionNodeRadius" unique class property ID inside the class
			static const uint32 MINIMUM_STREET_TIER_1_TO_2_DISTANCE;	///< "MinimumStreetTier1To2Distance" unique class property ID inside the class
			// Terrain modeling
			static const uint32 TERRAIN_FLATTEN_OFFSET;					///< "TerrainFlattenOffset" unique class property ID inside the class
			// Asset database
			static const uint32 ASSET_PREVIEW;							///< "AssetPreview" unique class property ID inside the class
			static const uint32 ASSET_IMPORT_PREVIEW_ENABLED;			///< "AssetImportPreviewEnabled" unique class property ID inside the class
			static const uint32 GLOBAL_ASSET_ID_GENERATION_BASE;		///< "GlobalAssetIdGenerationBase" unique class property ID inside the class
			static const uint32 MULTI_ASSET_IMPORT_ENABLED;				///< "MultiAssetImportEnabled" unique class property ID inside the class
			static const uint32 MATERIAL_ASSET_REIMPORT_ENABLED;		///< "MaterialAssetReimportEnabled" unique class property ID inside the class
			static const uint32 LOCAL_ASSET_DATASOURCES;				///< "LocalAssetDatasources" unique class property ID inside the class
			// Tests
			static const uint32 PLACEMENT_MAP_TEST_TOLERANCE;	///< "PlacementMapTestTolerance" unique class property ID inside the class
			// Static presets of the special (automatically preconfigured) datasource for materials. "qsf::editor::OgreMaterialImport" automatically copies all material source assets into this datasource.
			static const std::string MATERIAL_DATASOURCE_IDENTIFIER;		///< Datasource identifier of the material datasource
			static const std::string MATERIAL_DATASOURCE_INTERNAL_PATH;		///< Path relative to the base directory - Begins and ends with a path separator (e.g. "/source/material/")

			// Static presets of the special (automatically preconfigured) datasource for terrains. "qsf::TerrainDefinitions" automatically assumes all terrain source assets to be inside this datasource.
			static const std::string TERRAIN_DATASOURCE_IDENTIFIER;			///< Datasource identifier of the material datasource
			static const std::string TERRAIN_DATASOURCE_INTERNAL_PATH;		///< Path relative to the base directory - Begins and ends with a path separator (e.g. "/source/terrain_data_map/")

			typedef boost::container::flat_map<std::string, std::string> LocalAssetDatasources;	///< Datasource identifier string as key and the local datasource path as value


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] settingsGroupManager
			*    Optional pointer to the settings group manager this settings group should be registered to, can be a null pointer
			*/
			inline explicit EditorSettingsGroup(SettingsGroupManager* settingsGroupManager);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~EditorSettingsGroup();

			//[-------------------------------------------------------]
			//[ General                                               ]
			//[-------------------------------------------------------]
			/** Get the map asset to load on startup by default in case the editor is not connected to a server */
			inline const AssetProxy& getStartupMap() const;

			/** Set the map asset to load on startup by default in case the editor is not connected to a server */
			inline void setStartupMap(const AssetProxy& startupMap);

			/** Return the number of digits displayed after the decimal point */
			inline int getFloatPrecision() const;

			/** Set the number of digits displayed after the decimal point */
			inline void setFloatPrecision(int precision);

			//[-------------------------------------------------------]
			//[ UI                                                    ]
			//[-------------------------------------------------------]
			/** Return the name of the used UI style */
			inline const std::string& getUiStyle() const;

			/** Set the name of the used UI style */
			void setUiStyle(const std::string& uiStyle);

			/** Return the name of the used UI style sheet */
			inline const std::string& getUiStyleSheet() const;

			/** Set the name of the used UI style sheet */
			void setUiStyleSheet(const std::string& uiStyleSheet);

			/** Tell Qt about the style sheet to use */
			void setQtStyleSheet();

			/** Tell Qt about the style to use */
			void setQtStyle();

			//[-------------------------------------------------------]
			//[ Show                                                  ]
			//[-------------------------------------------------------]
			/** Return if the user can see hidden system components and values */
			inline bool getShowHiddenComponents() const;

			/** Set if the user can see hidden system components and values */
			inline void setShowHiddenComponents(bool showHiddenComponents);

			// TODO(co) Comment those
			inline bool getExpandObjectProperties() const;
			inline void setExpandObjectProperties(bool expandObjectProperties);

			//[-------------------------------------------------------]
			//[ Selection                                             ]
			//[-------------------------------------------------------]
			/** Return if the user can select entities only on the selected layer or sublayer */
			inline bool getSelectEntityOnLayer() const;

			/** Set if the user can select entities only on the selected layer or sublayer */
			inline void setSelectEntityOnLayer(bool selectEntityOnLayer);

			/** Return whether or not is possible to select terrain components inside the render view */
			inline bool getTerrainRenderViewSelection() const;

			/** Set whether or not is possible to select terrain components inside the render view */
			inline void setTerrainRenderViewSelection(bool enabled);

			/** Return whether a new entity should be pasted into the same layer the copied entity resides in or into the selected layer */
			inline bool getPasteEntitiesInSourceLayer() const;

			/** Set whether a new entity should be pasted into the same layer the copied entity resides in or into the selected layer */
			inline void setPasteEntitiesInSourceLayer(bool enabled);

			//[-------------------------------------------------------]
			//[ Log                                                   ]
			//[-------------------------------------------------------]
			// TODO(co) Comment those
			inline LogMessage::SeverityLevel getShowLogSeverityLevel() const;
			inline void setShowLogSeverityLevel(LogMessage::SeverityLevel severityLevel);

			// TODO(co) What is this method about? Please comment methods.
			inline const std::vector<LogContextFilterSettings>& getLogContextFilterSettings() const;
			inline std::vector<LogContextFilterSettings>& getLogContextFilterSettings(); // TODO(co) Remove this

			/**
			*  @brief
			*    Check if context is existing and add this to mLogContextFilterSettings
			*/
			void addNewLogContext(uint32 newContext, const std::string& name = "");

			//[-------------------------------------------------------]
			//[ Street                                                ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Get the default street crossing node radius
			*
			*  @return
			*    The default street crossing node radius
			*/
			inline float getDefaultStreetCrossingNodeRadius() const;

			/**
			*  @brief
			*    Set default street crossing node radius
			*
			*  @param[in] defaultStreetCrossingNodeRadius
			*    Default street crossing node radius
			*/
			inline void setDefaultStreetCrossingNodeRadius(float defaultStreetCrossingNodeRadius);

			/**
			*  @brief
			*    Get the default street section base mesh asset
			*
			*  @return
			*    The default street section base mesh asset
			*/
			inline const AssetProxy& getDefaultStreetSectionBaseMesh() const;

			/**
			*  @brief
			*    Set default street section base mesh asset
			*
			*  @param[in] defaultStreetSectionBaseMesh
			*    Default street section base mesh asset to set
			*/
			inline void setDefaultStreetSectionBaseMesh(const AssetProxy& defaultStreetSectionBaseMesh);

			/**
			*  @brief
			*    Get the default street section node radius
			*
			*  @return
			*    The default street section node radius
			*/
			inline float getDefaultStreetSectionNodeRadius() const;

			/**
			*  @brief
			*    Set default street section node radius
			*
			*  @param[in] defaultStreetSectionNodeRadius
			*    Default street section node radius
			*/
			inline void setDefaultStreetSectionNodeRadius(float defaultStreetSectionNodeRadius);

			/**
			*  @brief
			*    Get the minimum street tier 1 to tier 2 distance
			*
			*  @return
			*    The minimum street tier 1 to tier 2 distance
			*/
			inline float getMinimumStreetTier1To2Distance() const;

			/**
			*  @brief
			*    Set the minimum street tier 1 to tier 2 distance
			*
			*  @param[in] minimumStreetTier1To2Distance
			*    Minimum street tier 1 to tier 2 distance
			*/
			inline void setMinimumStreetTier1To2Distance(float minimumStreetTier1To2Distance);

			//[-------------------------------------------------------]
			//[ Terrain modeling                                      ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Get the terrain flatten offset
			*
			*  @return
			*    The terrain flatten offset
			*/
			inline float getTerrainFlattenOffset() const;

			/**
			*  @brief
			*    Set terrain flatten offset
			*
			*  @param[in] terrainFlattenOffset
			*    Terrain flatten offset
			*/
			inline void setTerrainFlattenOffset(float terrainFlattenOffset);

			//[-------------------------------------------------------]
			//[ Asset database                                        ]
			//[-------------------------------------------------------]
			/** Return whether or not there's an asset preview */
			inline bool getAssetPreview() const;

			/** Set whether or not there's an asset preview */
			inline void setAssetPreview(bool assetPreview);

			/**
			*  @brief
			*    Return whether or not the asset import preview when using drag'n'drop-import is enabled
			*
			*  @return
			*    "true" if the asset import preview when using drag'n'drop-import is enabled, else "false"
			*/
			inline bool isAssetImportPreviewEnabled() const;

			/**
			*  @brief
			*    Set whether or not the asset import preview when using drag'n'drop-import is enabled
			*
			*  @param[in] assetImportPreviewEnabled
			*    "true" if the asset import preview when using drag'n'drop-import is enabled, else "false"
			*/
			inline void setAssetImportPreviewEnabled(bool assetImportPreviewEnabled);

			/** Return the minimal generated global asset ID in offline mode */
			inline uint64 getGlobalAssetIdGenerationBase() const;

			/** Set the minimal generated global asset ID in offline mode */
			inline void setGlobalAssetIdGenerationBase(uint64 globalAssetIdGenerationBase);

			/**
			*  @brief
			*    Return whether or not the import of multiple assets at one and the same is allowed
			*
			*  @return
			*    "true" if the import of multiple assets at one and the same is allowed, else "false"
			*/
			inline bool isMultiAssetImportEnabled() const;

			/**
			*  @brief
			*    Set whether or not the import of multiple assets at one and the same is allowed
			*
			*  @param[in] multiAssetImportEnabled
			*    "true" if the import of multiple assets at one and the same is allowed, else "false"
			*/
			inline void setMultiAssetImportEnabled(bool multiAssetImportEnabled);

			/**
			*  @brief
			*    Return whether or not the reimport of material assets is enabled
			*
			*  @return
			*    "true" if the reimport of material assets is enabled, else "false"
			*/
			inline bool isMaterialAssetReimportEnabled() const;

			/**
			*  @brief
			*    Set whether or not the reimport of material assets is enabled
			*
			*  @param[in] materialAssetReimportEnabled
			*    "true" if the reimport of material assets is enabled, else "false"
			*/
			inline void setMaterialAssetReimportEnabled(bool materialAssetReimportEnabled);

			/**
			*  @brief
			*    Get the local asset datasources as key-value mappings with the datasource identifier string as key and the local datasource path as value
			*
			*  @return
			*    The local asset datasources
			*/
			inline const LocalAssetDatasources& getLocalAssetDatasources() const;

			/**
			*  @brief
			*    Set the local asset datasources as key-value mappings with the datasource identifier string as key and the local datasource path as value
			*
			*  @param[in] localAssetDatasources
			*    The local asset datasources
			*/
			void setLocalAssetDatasources(const LocalAssetDatasources& localAssetDatasources);

			std::string getLocalAssetDatasourcesAsString() const;
			void setLocalAssetDatasourcesFromString(const std::string& localAssetDatasourcesAsString);

			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(RelaxTextureAssetImportSecurityChecks, bool, mRelaxTextureAssetImportSecurityChecks)
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(RelaxMeshAssetImportSecurityChecks,    bool, mRelaxMeshAssetImportSecurityChecks)

			//[-------------------------------------------------------]
			//[ Tests                                                 ]
			//[-------------------------------------------------------]
			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PlacementMapTestTolerance, float, mPlacementMapTestTolerance)


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Connected to the CAMP reflection system
			// General
			AssetProxy mStartupMap;		///< The map asset to load on startup by default in case the editor is not connected to a server (e.g. "sample/map/sample/sample")
			int		   mFloatPrecision;	///< The number of digits displayed after the decimal point for floating point values in the editor
			// UI
			std::string mUiStyle;		///< The name of the used UI style
			std::string mUiStyleSheet;	///< The name of the used UI style sheet
			// Show
			bool mShowHiddenComponents;		///< Allow the user to show system components and values in object browser
			bool mExpandObjectProperties;	///< Do automatically expand the object properties?
			// Selection
			bool mSelectEntityOnLayer;			///< Allow entity selection only on selected layer or sublayer
			bool mTerrainRenderViewSelection;	///< Is it possible to select terrain components inside the render view?
			bool mPasteEntitiesInSourceLayer;	///< Paste new entities into the source entity layer or the selected layer?
			// Log
			LogMessage::SeverityLevel			  mShowLogSeverityLevel;		///< Severity level at which to automatically show the debug log view
			std::vector<LogContextFilterSettings> mLogContextFilterSettings;
			// Street
			float	   mDefaultStreetCrossingNodeRadius;	///< Default street crossing node radius
			AssetProxy mDefaultStreetSectionBaseMesh;		///< Default street section base mesh
			float	   mDefaultStreetSectionNodeRadius;		///< Default street section node radius
			float	   mMinimumStreetTier1To2Distance;		///< Minimum street tier 1 to tier 2 distance
			// Terrain modeling
			float mTerrainFlattenOffset;	///< Terrain flatten offset
			// Asset database
			bool				  mAssetPreview;							///< Asset preview?
			bool				  mAssetImportPreviewEnabled;				///< "true" if the asset import preview when using drag'n'drop-import is enabled, else "false"
			uint64				  mGlobalAssetIdGenerationBase;				///< The minimal generated global asset ID when creating assets while working offline
			bool				  mMultiAssetImportEnabled;					///< "true" if the import of multiple assets at one and the same is allowed, else "false"
			bool				  mMaterialAssetReimportEnabled;			///< "true" to allow material reimport, else "false"
			LocalAssetDatasources mLocalAssetDatasources;					///< List of local asset datasources (since CAMP currently doesn't support maps we use a semicolon-separated string instead)
			bool				  mRelaxTextureAssetImportSecurityChecks;	///< Relax texture asset imports security checks (not recommended, mind the kids)
			bool				  mRelaxMeshAssetImportSecurityChecks;		///< Relax mesh asset imports security checks (not recommended, mind the kids)
			// Tests
			float mPlacementMapTestTolerance;	///< Placement map test tolerance


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/settings/EditorSettingsGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::EditorSettingsGroup)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::LogContextFilterSettings)
