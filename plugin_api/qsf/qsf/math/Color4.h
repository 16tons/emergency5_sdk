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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Color3;	// TODO(fw): Make this an include instead? We could inline the "Color4(const Color3&)" constructor then
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
	*    Color class with red, green, blue and alpha component
	*/
	class QSF_API_EXPORT Color4
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Main colors
		static const Color4 TRANSPARENT;	///< 0.0, 0.0, 0.0, 0.0
		static const Color4 BLACK;			///< Black (sRGB="0 0 0", Hex="#000000")
		static const Color4 WHITE;			///< White (sRGB="255 255 255", Hex="#FFFFFF")
		static const Color4 RED;			///< Red (sRGB="255 0 0", Hex="#FF0000")
		static const Color4 YELLOW;			///< Yellow (sRGB="255 255 0", Hex="#FFFF00")
		static const Color4 GREEN;			///< Green (sRGB="0 255 0", Hex="#00FF00")
		static const Color4 CYAN;			///< Cyan (sRGB="0 255 255", Hex="#00FFFF")
		static const Color4 BLUE;			///< Blue (sRGB="0 0 255", Hex="#0000FF")
		static const Color4 PINK;			///< Pink (sRGB="255 0 255", Hex="#FF00FF")
		static const Color4 ORANGE;			///< Orange(sRGB="255 127 0", Hex="#FF7F00")

		// Special colors
		static const Color4 FUCHSIA;		///< Fuchsia, also called full magenta, bright pink or vivid pink (sRGB="255 0 255", Hex="#FF00FF")
		static const Color4 MAROON;			///< Maroon, dark brownish-red color (sRGB="128 0 0", Hex="#800000")
		static const Color4 OLIVE;			///< Olive, dark green/brown color (sRGB="128 128 0", Hex="#808000")
		static const Color4 NAVY;			///< Navy, also called navy blue, very dark shade of the color blue (sRGB="0 0 128", Hex="#000080")
		static const Color4 PURPLE;			///< Purple, color intermediate to red and blue (sRGB="128 0 128", Hex="#800080")
		static const Color4 TEAL;			///< Teal, also called teal blue, greenish dark-blue color (sRGB="0 128 128", Hex="#008080")
		static const Color4 GRAY;			///< Gray (sRGB="128 128 128", Hex="#808080")
		static const Color4 SILVER;			///< Silver (sRGB="192 192 192", Hex="#C0C0C0")
		static const Color4 LIME;			///< Lime, actually corresponds to the green primary of an RGB display (sRGB="0 255 0", Hex="#00FF00")


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Interpolate between two RGBA colors
		*
		*  @param[in] color1
		*    First color; this is returned with interpolation factor 0.0f
		*  @param[in] color2
		*    Second color; this is returned with interpolation factor 1.0f
		*  @param[in] factor
		*    Interpolation factor between 0.0f and 1.0f
		*
		*  @return
		*    Interpolated RGBA color
		*
		*  @note
		*    - This is a component-wise interpolation without special handling for the alpha channel
		*/
		inline static Color4 interpolate(const Color4& color1, const Color4& color2, float factor);


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		union
		{
			float color[4];
			struct
			{
				float r, g, b, a;
			};
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor (all components are set to 0.0)
		*/
		inline Color4();

		/**
		*  @brief
		*    Constructor from RGB color, adding alpha value of 1.0f
		*
		*  @note
		*    - Not explicit by intent
		*/
		Color4(const Color3& rgb, float alpha = 1.0f);

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
		*  @param[in] a
		*    Alpha color component (0.0-1.0)
		*/
		inline Color4(float r, float g, float b, float a = 1.0f);

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
		*  @param[in] a
		*    Alpha color component (0-255)
		*/
		inline Color4(uint8 r, uint8 g, uint8 b, uint8 a = 255);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rgba
		*    Float array holding the color, must have 4 elements! (0.0-1.0)
		*/
		inline Color4(const float rgba[]);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rgba
		*    uint8 array holding the color, must have 4 elements! (0-255)
		*/
		inline Color4(const uint8 rgba[]);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] luminance
		*    Value for all four color components (0.0-1.0)
		*/
		inline Color4(float luminance);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] luminance
		*    Value for all four color components (0-255)
		*/
		inline Color4(uint8 luminance);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] source
		*    Source color
		*/
		inline explicit Color4(const Color4& source);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~Color4();

		/**
		*  @brief
		*    Maps this RGBA floating point channels (0.0f to 1.0f range) to uint32
		*
		*  @return
		*    This color as uint32 (ARGB)
		*/
		inline uint32 toUInt32() const;
		inline uint32 toRGBAUInt32() const;
		inline uint32 toARGBUInt32() const;
		inline uint32 toRGBUInt32() const;

		/**
		*  @brief
		*    Get a hex color string representation (including '#' at the beginning) from a "qsf::Color4"
		*/
		std::string toHexColor() const;

		/**
		*  @brief
		*    Maps from a uint32 to RGBA floating point channels (0.0f to 1.0f range)
		*
		*  @param[in] packedColor
		*    Color4 as uint32
		*/
		inline void fromUInt32(uint32 packedColor);

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
		*  @param[out] a
		*    Will receive the alpha color component (0.0-1.0)
		*/
		inline void getRGBA(float& r, float& g, float& b, float& a) const;

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
		*  @param[out] a
		*    Will receive the alpha color component (0-255)
		*/
		inline void getRGBA(uint8& r, uint8& g, uint8& b, uint8& a) const;

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
		*  @param[in] a
		*    Alpha color component (0.0-1.0)
		*/
		inline void setRGBA(float r, float g, float b, float a);

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
		*  @param[in] a
		*    Alpha color component (0-255)
		*/
		inline void setRGBA(uint8 r, uint8 g, uint8 b, uint8 a);

		/**
		*  @brief
		*    Get the color component as array of float values
		*
		*  @param[out] rgba
		*    Reference to float array receiving the color values, must have 4 elements! (0.0-1.0)
		*/
		inline void getRGBA(float rgba[]) const;

		/**
		*  @brief
		*    Get the color component as array of integer values
		*
		*  @param[out] rgba
		*    Reference to uint8 array receiving the color values, must have 4 elements! (0-255)
		*/
		inline void getRGBA(uint8 rgba[]) const;

		/**
		*  @brief
		*    Set the color components by using a reference to float values
		*
		*  @param[in] rgba
		*    Reference to float array containing the color value, must have 4 elements! (0.0-1.0)
		*/
		inline void setRGBA(const float rgba[]);

		/**
		*  @brief
		*    Set the color components by using a reference to integer values
		*
		*  @param[in] rgba
		*    Reference to uint8 array containing the color value, must have 4 elements! (0-255)
		*/
		inline void setRGBA(const uint8 rgba[]);

		/**
		*  @brief
		*    Set all color component to the same float value
		*
		*  @param[in] value
		*    Value for all four color component (0.0-1.0)
		*/
		inline void setRGBA(float value);

		/**
		*  @brief
		*    Set all color component to the same integer value
		*
		*  @param[in] value
		*    Value for all four color component (0-255)
		*/
		inline void setRGBA(uint8 value);

		/**
		*  @brief
		*    Clamps the color values between 0.0 and 1.0
		*/
		inline void saturate();

		//[-------------------------------------------------------]
		//[ Assignment operators                                  ]
		//[-------------------------------------------------------]
		inline Color4& operator =(const Color4& otherColor);
		inline Color4& operator =(const float rgba[]);
		inline Color4& operator =(float value);
		inline operator float *();
		inline operator const float *() const;

		//[-------------------------------------------------------]
		//[ Comparison                                            ]
		//[-------------------------------------------------------]
		inline bool operator ==(const Color4& otherColor) const;
		inline bool operator !=(const Color4& otherColor) const;
		inline bool operator ==(float value) const;
		inline bool operator !=(float value) const;

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		inline Color4  operator +(const Color4& otherColor) const;
		inline Color4  operator +(float value) const;
		inline Color4& operator +=(const Color4& otherColor);
		inline Color4& operator +=(float value);
		inline Color4  operator -() const;
		inline Color4  operator -(const Color4& otherColor) const;
		inline Color4  operator -(float value) const;
		inline Color4& operator -=(const Color4& otherColor);
		inline Color4& operator -=(float value);
		inline Color4  operator *(const Color4& otherColor) const;
		inline Color4  operator *(float value) const;
		inline Color4& operator *=(const Color4& otherColor);
		inline Color4& operator *=(float value);
		inline Color4  operator /(const Color4& otherColor) const;
		inline Color4  operator /(float value) const;
		inline Color4& operator /=(const Color4& otherColor);
		inline Color4& operator /=(float value);
		inline float& operator [](int index);


	};


	namespace serialization
	{
		// Serialization specialization for Color4 for use with the QSF binary serialization
		template<>
		struct serializer<Color4>
		{
			inline static void serialize(BinarySerializer& serializer, Color4& value)
			{
				serializer.serialize(value.r);
				serializer.serialize(value.g);
				serializer.serialize(value.b);
				serializer.serialize(value.a);
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
#include "qsf/math/Color4-inl.h"
