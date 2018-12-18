// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/material/MaterialProperty.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	struct HlmsMacroblock;
	struct HlmsBlendblock;
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
	*    Material properties
	*/
	class QSF_API_EXPORT MaterialProperties
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<MaterialProperty> SortedPropertyVector;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline MaterialProperties();

		/**
		*  @brief
		*    Destructor
		*/
		inline ~MaterialProperties();

		/**
		*  @brief
		*    Return the properties
		*
		*  @return
		*    The properties
		*/
		inline const SortedPropertyVector& getSortedPropertyVector() const;

		/**
		*  @brief
		*    Return a material property by its ID
		*
		*  @param[in] materialPropertyId
		*    ID of the material property to return
		*
		*  @return
		*    The requested material property, null pointer on error, don't destroy the returned instance
		*/
		const MaterialProperty* getPropertyById(MaterialPropertyId materialPropertyId) const;

		/**
		*  @brief
		*    Remove all material properties
		*/
		inline void removeAllProperties();

		/**
		*  @brief
		*    Set a material property value by its ID
		*
		*  @param[in] materialPropertyId
		*    ID of the material property to set the value from
		*  @param[in] materialPropertyValue
		*    The material property value to set
		*  @param[in] usage
		*    The material property usage
		*  @param[in] changeOverwrittenState
		*    Change overwritten state?
		*
		*  @return
		*    Pointer to the added or changed property, null pointer if no material property change has been detected, don't destroy the returned instance
		*/
		MaterialProperty* setPropertyById(MaterialPropertyId materialPropertyId, const MaterialPropertyValue& materialPropertyValue, MaterialProperty::Usage usage = MaterialProperty::Usage::DYNAMIC, bool changeOverwrittenState = false);

		/**
		*  @brief
		*    Apply material properties to the given OGRE HLMS macroblock
		*
		*  @param[in] ogreHlmsMacroblock
		*    OGRE HLMS macroblock to apply the material properties to
		*/
		void applyToOgreHlmsMacroblock(Ogre::HlmsMacroblock& ogreHlmsMacroblock);

		/**
		*  @brief
		*    Apply material properties to the given OGRE HLMS blendblock
		*
		*  @param[in] ogreHlmsBlendblock
		*    OGRE HLMS blendblock to apply the material properties to
		*/
		void applyToOgreHlmsBlendblock(Ogre::HlmsBlendblock& ogreHlmsBlendblock);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SortedPropertyVector mSortedPropertyVector;


	};

	namespace detail
	{
		struct OrderByMaterialPropertyId
		{
			inline bool operator()(const MaterialProperty& left, MaterialPropertyId right) const
			{
				return (left.getMaterialPropertyId() < right);
			}

			inline bool operator()(MaterialPropertyId left, const MaterialProperty& right) const
			{
				return (left < right.getMaterialPropertyId());
			}
		};
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/material/MaterialProperties-inl.h"
