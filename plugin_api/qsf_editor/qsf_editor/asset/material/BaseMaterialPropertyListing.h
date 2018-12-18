// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/base/StringHash.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		struct BaseMaterialProperty
		{
			enum Type
			{
				TYPE_BOOLEAN,
				TYPE_FLOAT,
				TYPE_FLOAT2,
				TYPE_FLOAT3,
				TYPE_FLOAT4,
				TYPE_COLOR3,
				TYPE_COLOR4,
				TYPE_TEXTURE,
				TYPE_TEXTURE_LUT,
				TYPE_TEXTURE_CUBE,
				TYPE_ENUM,
				TYPE_TEXT,
				TYPE_UNKNOWN
			};

			Type					 type;
			std::string				 name;
			std::string				 description;
			std::string				 defaultValue;
			bool					 requiresRecompilation;
			std::string				 rangeMin;
			std::string				 rangeMax;
			std::string				 rangeStep;
			std::vector<std::string> enumValues;

			BaseMaterialProperty() :
				type(TYPE_UNKNOWN),
				requiresRecompilation(false)
			{}

		};


		//[-------------------------------------------------------]
		//[ Class                                                 ]
		//[-------------------------------------------------------]
		class BaseMaterialPropertyListing
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef std::map<unsigned int, BaseMaterialProperty> PropertyMap;
			typedef std::vector<std::string>					 CompiledTexturePropertyList;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			BaseMaterialPropertyListing();
			const std::string& getName() const;
			bool isCompilationNeeded() const;
			const BaseMaterialProperty* getProperty(const StringHash& propertyKey) const;
			const PropertyMap& getPropertyMap() const;
			const CompiledTexturePropertyList& getCompiledTexturePropertyList() const;
			bool loadFromFilename(const std::string& absoluteFilename);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::string					mName;
			bool						mIsCompilationNeeded;
			PropertyMap					mPropertyMap;
			CompiledTexturePropertyList	mCompiledTexturePropertyList;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
