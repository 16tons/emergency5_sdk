// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/component/Component.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Helper class to access a material property with a component
		*
		*  @todo
		*    - TODO(co) A component misused to access material properties? This looks odd and overkill like.
		*/
		class QSF_GAME_API_EXPORT MaterialAccessComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::game::MaterialAccessComponent" unique component ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] prototype
			*    The prototype this component is in, no null pointer allowed
			*/
			inline explicit MaterialAccessComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~MaterialAccessComponent();

			/**
			*  @brief
			*    Return the material property name
			*/
			inline const std::string& getPropertyName() const;

			/**
			*  @brief
			*    Set the material name
			*/
			void setPropertyName(const std::string& propertyName);

			/**
			*  @brief
			*    Return the material property name
			*/
			inline const std::string& getPropertyValue() const;

			/**
			*  @brief
			*    Set the material name
			*/
			void setPropertyValue(const std::string& propertyValue);


		//[-------------------------------------------------------]
		//[ Protected qsf::Component methods                      ]
		//[-------------------------------------------------------]
		protected:
			virtual void onSetActive(bool active) override;
			virtual bool onStartup() override;
			virtual void onShutdown() override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Access to the material, load the material-property-value to the material
			*
			*    TODO(tl): We don't want error messages right ?
			*/
			void setMaterial();

			/**
			*  @brief
			*    Access to the material, reset the material
			*
			*    TODO(tl): We don't want error messages right ?
			*/
			void resetMaterial();

			/**
			*  @brief
			*    Access to the material, get the material-property-value to the material, TODO(tl) Not implemented
			*/
			void getMaterial();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::string mPropertyName;
			std::string mPropertyValue;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/component/material/MaterialAccessComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::MaterialAccessComponent)
