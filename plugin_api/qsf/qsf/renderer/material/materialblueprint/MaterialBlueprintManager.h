// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/ElementManager.h"
#include "qsf/renderer/material/material/MaterialProperties.h"
#include "qsf/renderer/material/materialblueprint/MaterialBlueprint.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ShaderBlueprintManager;
	class IMaterialBlueprintListener;
	namespace detail
	{
		class OgreHlmsManagerListener;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef std::vector<uint32> OgreHlmsCacheHashes;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Material blueprint manager
	*/
	class MaterialBlueprintManager : public ElementManager<MaterialBlueprintId, MaterialBlueprint, detail::DirectAllocator<MaterialBlueprint>, ConsecutiveIdGenerator<uint32>>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MaterialSystem;					///< The material system is the master of this class
		friend class JsonMaterialBlueprintSerializer;	// TODO(co) Remove this


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the material blueprint listener
		*
		*  @return
		*    The material blueprint listener
		*/
		inline IMaterialBlueprintListener& getMaterialBlueprintListener() const;

		/**
		*  @brief
		*    Set the material blueprint listener
		*
		*  @param[in] materialBlueprintListener
		*    The material blueprint listener to set, can be a null pointer, address must stay valid as long as its referenced, listener is not automatically destroyed
		*/
		void setMaterialBlueprintListener(IMaterialBlueprintListener* materialBlueprintListener);

		/**
		*  @brief
		*    Return the global material properties
		*
		*  @return
		*    The global material properties
		*/
		inline MaterialProperties& getGlobalMaterialProperties();
		inline const MaterialProperties& getGlobalMaterialProperties() const;

		/**
		*  @brief
		*    Reload all material blueprints
		*/
		void reloadAllBlueprints();

		/**
		*  @brief
		*    Create default pipeline state cache instances
		*
		*  @remarks
		*    Ensure that for every material blueprint there's a pipeline state cache for the most basic pipeline state signature
		*    -> With this in place, a fallback pipeline state signature can always be found for pipeline state cache misses
		*    -> Without this, we'll end up with runtime hiccups for pipeline state cache misses
		*
		*   In order to reduce visual artifacts, a material blueprint can define a set of shader properties for which pipeline
		*   state caches must also be created. Inside the JSON material blueprint files, those properties are marked by
		*   "MandatoryStaticCombination" set to true (long name because combinations take long to generate). Good examples for
		*   such shader properties are color map or GPU skinning. It's the responsibility of the material blueprint author to
		*   keep the number of such shader properties to a bare minimum.
		*/
		void createDefaultPipelineStateCaches();

		/**
		*  @brief
		*    Called when a frame has ended
		*/
		void onFrameEnded();

		/**
		*  @brief
		*    Called when the render system has been changed
		*
		*  @param[in] ogreRenderSystem
		*    The new OGRE render system, can be a null pointer
		*/
		void onChangeRenderSystem(Ogre::RenderSystem* ogreRenderSystem);

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get a material blueprint instance by using "Ogre::IdString(<material blueprint name>)"
		*
		*  @param[in] ogreIdString
		*    Result of "Ogre::IdString(<material blueprint name>)"
		*
		*  @return
		*    The material blueprint instance, null pointer on error
		*/
		inline MaterialBlueprint* getMaterialBlueprintByOgreIdString(uint32 ogreIdString) const;

		/**
		*  @brief
		*    Set current pass shader properties
		*
		*  @param[in] passShaderProperties
		*    The pass shader properties to set
		*/
		void setCurrentPassShaderProperties(const ShaderProperties& passShaderProperties);

		/**
		*  @brief
		*    Cleanup dirty cache parts
		*
		*  @param[in] deletedOgreHlmsCacheHashes
		*    The hashes of now invalid OGRE HLMS caches
		*/
		void cleanupDirtyCacheParts(const OgreHlmsCacheHashes& deletedOgreHlmsCacheHashes);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		MaterialBlueprintManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MaterialBlueprintManager();

		/**
		*  @brief
		*    Get all managed material blueprints which have at least one material property with the usage "STATIC_PASS"
		*/
		void getStaticPassPropertyMaterialBlueprints();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::unordered_map<uint32, MaterialBlueprint*> OgreIdStringToMaterialBlueprint;		///< "Ogre::IdString" to "qsf::MaterialBlueprint" mapping
		typedef std::vector<MaterialBlueprint*>				   StaticPassPropertyMaterialBlueprints;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IMaterialBlueprintListener*			 mMaterialBlueprintListener;			///< Material blueprint listener, always valid, do not destroy the instance
		MaterialProperties					 mGlobalMaterialProperties;				///< Global material properties
		OgreIdStringToMaterialBlueprint		 mOgreIdStringToMaterialBlueprint;		// TODO(co) Update in case of material blueprint destruction
		StaticPassPropertyMaterialBlueprints mStaticPassPropertyMaterialBlueprints;	///< List of material blueprints which have at least one material property with the usage "STATIC_PASS"


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/materialblueprint/MaterialBlueprintManager-inl.h"
