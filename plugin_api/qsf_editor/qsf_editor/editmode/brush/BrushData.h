// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/EditMode.h"

#include <QtCore/qpoint.h>

#include <glm/glm.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Brush data class
		*/
		class BrushData
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:

			/**
			*  @brief
			*    Influence shape type for editing
			*/
			enum ShapeType
			{
				SHAPE_DOME		= 0,	///< Dome shape
				SHAPE_CONE		= 1,	///< Cone shape
				SHAPE_CIRCLE	= 2,	///< Flat circle
				SHAPE_SQUARE	= 3		///< Flat square
			};


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			BrushData();

			/**
			*  @brief
			*    Destructor
			*/
			~BrushData();

			/**
			*  @brief
			*    Return the brush name
			*
			*  @return
			*    The brush name
			*/
			const std::string& getBrushName() const;

			/**
			*  @brief
			*    Set the brush name
			*
			*  @param[in] brushName
			*    The brush name
			*/
			void setBrushName(const std::string& brushName);

			/**
			*  @brief
			*    Return the brush size
			*
			*  @return
			*    The brush size
			*/
			float getBrushSize() const;

			/**
			*  @brief
			*    Set the brush size
			*
			*  @param[in] brushSize
			*    The brush size
			*/
			void setBrushSize(float brushSize);

			/**
			*  @brief
			*    Return the brush intensity
			*
			*  @return
			*    The brush intensity
			*/
			float getBrushIntensity() const;

			/**
			*  @brief
			*    Set the brush intensity
			*
			*  @param[in] brushIntensity
			*    The brush intensity
			*/
			void setBrushIntensity(float brushIntensity);

			/**
			*  @brief
			*    Return the brush randomness
			*
			*  @return
			*    The brush randomness [0..1]
			*/
			float getBrushRandomness() const;

			/**
			*  @brief
			*    Set the brush randomness
			*
			*  @param[in] brushRandomness
			*    The brush randomness [0..1]
			*/
			void setBrushRandomness(float brushRandomness);

			/**
			*  @brief
			*    Return the brush type
			*
			*  @return
			*    The brush type
			*/
			ShapeType getBrushType() const;

			/**
			*  @brief
			*    Set the brush type
			*
			*  @param[in] type
			*    The brush type
			*/
			void setBrushType(ShapeType type);

			/**
			*  @brief
			*    Read settings for brush data
			*
			*  @param[in] qSettings
			*    QSettings to read from
			*/
			void readBrushSettings(QSettings& qSettings);

			/**
			*  @brief
			*    Write settings from brush data
			*
			*  @param[in] qSettings
			*    QSettings to write to
			*/
			void writeBrushSettings(QSettings& qSettings);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::string	mName;				///< Brush data name
			float		mBrushSize;			///< Brush world space size
			float		mBrushIntensity;	///< Brush intensity
			float		mBrushRandomness;	///< Factor [0..1] for randomness
			ShapeType	mBrushType;			///< Brush type


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
