// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/asset/AssetSystemTypes.h"

#include <OGRE/OgreMaterial.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ParameterGroup;
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
	*    Light material generator
	*
	*  @remarks
	*    Caching, on-the-fly material generator. This is a class that automatically
	*    generates and stores different combinations of a material, and its shaders.
	*    It can be used if you have a material that has lots of slightly different
	*    variations, like whether to use a specular light, skinning, normal mapping
	*    and other options. Writing all these out is a tedious job. Of course it is
	*    possible to always use the material with all features, but that might result
	*    in large, slow shader programs. This class provides an efficient solution
	*    to that.
	*
	*  @note
	*    - Based on the OGRE deferred shading sample
	*/
	class LightMaterialGenerator : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Combination of light materials
		*/
		enum MaterialID
		{
			MI_POINT				= 1<<0,	///< Is a point light
			MI_SPOTLIGHT			= 1<<1,	///< Is a spot light
			MI_DIRECTIONAL			= 1<<2,	///< Is a directional light
			MI_ATTENUATED			= 1<<3,	///< Rendered attenuated
			MI_SHADOW_CASTER		= 1<<4,	///< Will cast shadows
			MI_PROJECTED_TEXTURE	= 1<<5	///< Will project texture
		};

		typedef uint32 Combination;	///< Bitfield used to signify a material combinations


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		LightMaterialGenerator();
		~LightMaterialGenerator();
		std::string getMaterialName(Combination combination, GlobalAssetId globalAssetId);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called as soon as there's a property change inside the settings group manager
		*
		*  @param[in] parameterGroup
		*    Parameter group instance the changed property is in
		*  @param[in] propertyId
		*    Unique class property ID the component property which values was changed
		*/
		void onSettingsGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef Ogre::map<Combination, std::string>::type CombinationMaterialNames;	// TODO(co) Use a more efficient data structure


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		CombinationMaterialNames mCombinationMaterialNames;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
