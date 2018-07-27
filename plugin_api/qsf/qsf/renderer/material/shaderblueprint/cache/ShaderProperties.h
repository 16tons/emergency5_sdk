// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/base/StringHash.h"

#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringHash ShaderPropertyId;	///< Shader property identifier, internally just a POD "uint32", result of hashing the property name


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Shader properties
	*/
	class QSF_API_EXPORT ShaderProperties
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct Property
		{
			ShaderPropertyId shaderPropertyId;
			int32			 value;

			Property() :
				shaderPropertyId(getUninitialized<ShaderPropertyId>()),
				value(getUninitialized<int32>())
			{
				// Nothing here
			}

			Property(StringHash _shaderPropertyId, int32 _value) :
				shaderPropertyId(_shaderPropertyId),
				value(_value)
			{
				// Nothing here
			}

			bool operator ==(const Property& property) const
			{
				return (shaderPropertyId == property.shaderPropertyId && value == property.value);
			}
		};

		typedef std::vector<Property> SortedPropertyVector;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline ShaderProperties();

		/**
		*  @brief
		*    Destructor
		*/
		inline ~ShaderProperties();

		/**
		*  @brief
		*    Return the properties
		*
		*  @return
		*    The properties
		*/
		inline const SortedPropertyVector& getSortedPropertyVector() const;
		inline SortedPropertyVector& getSortedPropertyVector();

		/**
		*  @brief
		*    Remove all shader properties
		*/
		inline void clear();

		/**
		*  @brief
		*    Return the value of a property
		*
		*  @param[in] shaderPropertyId
		*    ID of the shader property to return the value from
		*  @param[out] value
		*    Receives the property value
		*  @param[in] defaultValue
		*    Default value in case the shader property doesn't exist
		*
		*  @return
		*    "true" if the requested shader property exists, else "false" if the requested shader property doesn't exist and the default value was returned instead
		*/
		bool getPropertyValue(ShaderPropertyId shaderPropertyId, int32& value, int32 defaultValue = 0) const;

		/**
		*  @brief
		*    Return the value of a property
		*
		*  @param[in] shaderPropertyId
		*    ID of the shader property to return the value from
		*  @param[in] defaultValue
		*    Default value in case the shader property doesn't exist
		*
		*  @return
		*    The property value or the default value
		*
		*  @note
		*    - Unsafe because one can't figure out by the return value whether or not the property exists or if it's just the default value
		*/
		int32 getPropertyValueUnsafe(ShaderPropertyId shaderPropertyId, int32 defaultValue = 0) const;

		/**
		*  @brief
		*    Clear the value of a property
		*
		*  @param[in] shaderPropertyId
		*    ID of the shader property to clear the value from
		*/
		void clearPropertyValue(ShaderPropertyId shaderPropertyId);

		/**
		*  @brief
		*    Set the value of a property
		*
		*  @param[in] shaderPropertyId
		*    ID of the shader property to set the value of
		*  @param[in] value
		*    The shader property value to set
		*/
		void setPropertyValue(ShaderPropertyId shaderPropertyId, int32 value);

		/**
		*  @brief
		*    Set property values by using a given shader properties instance
		*
		*  @param[in] shaderProperties
		*    Shader properties instance to set the shader properties from
		*/
		void setPropertyValues(const ShaderProperties& shaderProperties);

		bool operator ==(const ShaderProperties& shaderProperties) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SortedPropertyVector mSortedPropertyVector;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/shaderblueprint/cache/ShaderProperties.inl"
