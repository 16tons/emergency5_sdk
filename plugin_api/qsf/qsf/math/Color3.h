// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/serialization/binary/BasicTypeSerialization.h"


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
	*    Color class with red, green and blue component
	*/
	class QSF_API_EXPORT Color3
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Main colors
		static const Color3 TRANSPARENT;	///< 0.0, 0.0, 0.0
		static const Color3 BLACK;			///< Black (sRGB="0 0 0", Hex="#000000")
		static const Color3 WHITE;			///< White (sRGB="255 255 255", Hex="#FFFFFF")
		static const Color3 RED;			///< Red (sRGB="255 0 0", Hex="#FF0000")
		static const Color3 YELLOW;			///< Yellow (sRGB="255 255 0", Hex="#FFFF00")
		static const Color3 GREEN;			///< Green (sRGB="0 255 0", Hex="#00FF00")
		static const Color3 CYAN;			///< Cyan (sRGB="0 255 255", Hex="#00FFFF")
		static const Color3 BLUE;			///< Blue (sRGB="0 0 255", Hex="#0000FF")
		static const Color3 PINK;			///< Pink (sRGB="255 0 255", Hex="#FF00FF")
		static const Color3 ORANGE;			///< Orange(sRGB="255 127 0", Hex="#FF7F00")

		// Special colors
		static const Color3 FUCHSIA;		///< Fuchsia, also called full magenta, bright pink or vivid pink (sRGB="255 0 255", Hex="#FF00FF")
		static const Color3 MAROON;			///< Maroon, dark brownish-red color (sRGB="128 0 0", Hex="#800000")
		static const Color3 OLIVE;			///< Olive, dark green/brown color (sRGB="128 128 0", Hex="#808000")
		static const Color3 NAVY;			///< Navy, also called navy blue, very dark shade of the color blue (sRGB="0 0 128", Hex="#000080")
		static const Color3 PURPLE;			///< Purple, color intermediate to red and blue (sRGB="128 0 128", Hex="#800080")
		static const Color3 TEAL;			///< Teal, also called teal blue, greenish dark-blue color (sRGB="0 128 128", Hex="#008080")
		static const Color3 GRAY;			///< Gray (sRGB="128 128 128", Hex="#808080")
		static const Color3 SILVER;			///< Silver (sRGB="192 192 192", Hex="#C0C0C0")
		static const Color3 LIME;			///< Lime, actually corresponds to the green primary of an RGB display (sRGB="0 255 0", Hex="#00FF00")


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		union
		{
			float color[3];
			struct
			{
				float r, g, b;
			};
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return a RGB color by using a given Kelvin value
		*
		*  @param[in] kelvin
		*    Kelvin value to return as RGB color
		*
		*  @return
		*    RGB color from the given Kelvin value
		*
		*  @note
		*    - Basing on "How to Convert Temperature (K) to RGB: Algorithm and Sample Code" ( http://www.tannerhelland.com/4435/convert-temperature-rgb-algorithm-code/ )
		*    - See also "Moving Frostbite to Physically Based Rendering" from DICE, starting page 35 ( http://www.frostbite.com/wp-content/uploads/2014/11/s2014_pbs_frostbite_slides.pdf )
		*/
		static Color3 fromKelvin(float kelvin);

		/**
		*  @brief
		*    Interpolate between two RGB colors
		*
		*  @param[in] color1
		*    First color; this is returned with interpolation factor 0.0f
		*  @param[in] color2
		*    Second color; this is returned with interpolation factor 1.0f
		*  @param[in] factor
		*    Interpolation factor between 0.0f and 1.0f
		*
		*  @return
		*    Interpolated RGB color
		*/
		inline static Color3 interpolate(const Color3& color1, const Color3& color2, float factor);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor (all components are set to 0.0)
		*/
		inline Color3();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] r
		*    Red color component (0.0-1.0)
		*  @param[in] g
		*    Green color component (0.0-1.0)
		*  @param[in] b
		*    Blue color component (0.0-1.0)
		*/
		inline Color3(float r, float g, float b);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] r
		*    Red color component (0-255)
		*  @param[in] g
		*    Green color component (0-255)
		*  @param[in] b
		*    Blue color component (0-255)
		*/
		inline Color3(uint8 r, uint8 g, uint8 b);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rgb
		*    Float array holding the color, must have 3 elements! (0.0-1.0)
		*/
		inline Color3(const float rgb[]);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rgb
		*    uint8 array holding the color, must have 3 elements! (0-255)
		*/
		inline Color3(const uint8 rgb[]);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] luminance
		*    Value for all three color components (0.0-1.0)
		*/
		inline Color3(float luminance);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] luminance
		*    Value for all three color components (0-255)
		*/
		inline Color3(uint8 luminance);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] source
		*    Source color
		*/
		inline explicit Color3(const Color3& source);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~Color3();

		/**
		*  @brief
		*    Maps this RGB floating point channels (0.0f to 1.0f range) to uint32
		*
		*  @return
		*    This color as uint32
		*/
		inline uint32 toUInt32() const;
		inline uint32 toRGBUInt32() const;
		inline uint32 toBGRUInt32() const;

		/**
		*  @brief
		*    Get a hex color string representation (including '#' at the beginning) from a "qsf::Color3"
		*/
		std::string toHexColor(float alpha = 1.0f) const;

		/**
		*  @brief
		*    Maps from a uint32 to RGB floating point channels (0.0f to 1.0f range)
		*
		*  @param[in] color
		*    Color3 as uint32
		*/
		inline void fromUInt32(uint32 encodedColor);

		/**
		*  @brief
		*    Get the color component as float values
		*
		*  @param[out] r
		*    Will receive the red color component (0.0-1.0)
		*  @param[out] g
		*    Will receive the green color component (0.0-1.0)
		*  @param[out] b
		*    Will receive the blue color component (0.0-1.0)
		*/
		inline void getRGB(float& r, float& g, float& b) const;

		/**
		*  @brief
		*    Get the color component as integer values
		*
		*  @param[out] r
		*    Will receive the red color component (0-255)
		*  @param[out] g
		*    Will receive the green color component (0-255)
		*  @param[out] b
		*    Will receive the blue color component (0-255)
		*/
		inline void getRGB(uint8& r, uint8& g, uint8& b) const;

		/**
		*  @brief
		*    Set the color components by using float values
		*
		*  @param[in] r
		*    Red color component (0.0-1.0)
		*  @param[in] g
		*    Green color component (0.0-1.0)
		*  @param[in] b
		*    Blue color component (0.0-1.0)
		*/
		inline void setRGB(float r, float g, float b);

		/**
		*  @brief
		*    Set the color components by using integer values
		*
		*  @param[in] r
		*    Red color component (0-255)
		*  @param[in] g
		*    Green color component (0-255)
		*  @param[in] b
		*    Blue color component (0-255)
		*/
		inline void setRGB(uint8 r, uint8 g, uint8 b);

		/**
		*  @brief
		*    Get the color component as array of float values
		*
		*  @param[out] color
		*    Reference to float array receiving the color values, must have 3 elements! (0.0-1.0)
		*/
		inline void getRGB(float rgb[]) const;

		/**
		*  @brief
		*    Get the color component as array of integer values
		*
		*  @param[out] color
		*    Reference to uint8 array receiving the color values, must have 3 elements! (0-255)
		*/
		inline void getRGB(uint8 rgb[]) const;

		/**
		*  @brief
		*    Set the color components by using a reference to float values
		*
		*  @param[in] color
		*    Reference to float array containing the color value, must have 3 elements! (0.0-1.0)
		*/
		inline void setRGB(const float rgb[]);

		/**
		*  @brief
		*    Set the color components by using a reference to integer values
		*
		*  @param[in] color
		*    Reference to uint8 array containing the color value, must have 3 elements! (0-255)
		*/
		inline void setRGB(const uint8 rgb[]);

		/**
		*  @brief
		*    Set all color component to the same float value
		*
		*  @param[in] value
		*    Value for all three color component (0.0-1.0)
		*/
		inline void setRGB(float value);

		/**
		*  @brief
		*    Set all color component to the same integer value
		*
		*  @param[in] value
		*    Value for all three color component (0-255)
		*/
		inline void setRGB(uint8 value);

		/**
		*  @brief
		*    Clamps the color values between 0.0 and 1.0
		*/
		inline void saturate();

		//[-------------------------------------------------------]
		//[ Assignment operators                                  ]
		//[-------------------------------------------------------]
		inline Color3& operator =(const Color3& otherColor);
		inline Color3& operator =(const float rgb[]);
		inline Color3& operator =(float value);
		inline operator float *();
		inline operator const float *() const;

		//[-------------------------------------------------------]
		//[ Comparison                                            ]
		//[-------------------------------------------------------]
		inline bool operator ==(const Color3& otherColor) const;
		inline bool operator !=(const Color3& otherColor) const;
		inline bool operator ==(float value) const;
		inline bool operator !=(float value) const;

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		inline Color3  operator +(const Color3& otherColor) const;
		inline Color3  operator +(float value) const;
		inline Color3& operator +=(const Color3& otherColor);
		inline Color3& operator +=(float value);
		inline Color3  operator -() const;
		inline Color3  operator -(const Color3& otherColor) const;
		inline Color3  operator -(float value) const;
		inline Color3& operator -=(const Color3& otherColor);
		inline Color3& operator -=(float value);
		inline Color3  operator *(const Color3& otherColor) const;
		inline Color3  operator *(float value) const;
		inline Color3& operator *=(const Color3& otherColor);
		inline Color3& operator *=(float value);
		inline Color3  operator /(const Color3& otherColor) const;
		inline Color3  operator /(float value) const;
		inline Color3& operator /=(const Color3& otherColor);
		inline Color3& operator /=(float value);
		inline float& operator [](int index);


	};


	namespace serialization
	{
		// Serialization specialization for Color3 for use with the QSF binary serialization
		template<>
		struct serializer<Color3>
		{
			inline static void serialize(BinarySerializer& serializer, Color3& value)
			{
				serializer.serialize(value.r);
				serializer.serialize(value.g);
				serializer.serialize(value.b);
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
#include "qsf/math/Color3-inl.h"
