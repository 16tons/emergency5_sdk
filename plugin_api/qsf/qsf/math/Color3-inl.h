// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Special operators                                     ]
	//[-------------------------------------------------------]
	inline Color3 operator *(float value, const Color3& otherColor)
	{
		return Color3(otherColor.r * value, otherColor.g * value, otherColor.b * value);
	}


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	inline Color3 Color3::interpolate(const Color3& color1, const Color3& color2, float factor)
	{
		return Color3(color1 + (color2 - color1) * factor);
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Color3::Color3() :
		r(0.0f),
		g(0.0f),
		b(0.0f)
	{
		// Nothing to do in here
	}

	inline Color3::Color3(float _r, float _g, float _b) :
		r(_r),
		g(_g),
		b(_b)
	{
		// Nothing to do in here
	}

	inline Color3::Color3(uint8 _r, uint8 _g, uint8 _b) :
		r(static_cast<float>(_r) / 255.0f),
		g(static_cast<float>(_g) / 255.0f),
		b(static_cast<float>(_b) / 255.0f)
	{
		// Nothing to do in here
	}

	inline Color3::Color3(const float rgb[]) :
		r(rgb[0]),
		g(rgb[1]),
		b(rgb[2])
	{
		// Nothing to do in here
	}

	inline Color3::Color3(const uint8 rgb[]) :
		r(rgb[0] / 255.0f),
		g(rgb[1] / 255.0f),
		b(rgb[2] / 255.0f)
	{
		// Nothing to do in here
	}

	inline Color3::Color3(float luminance) :
		r(luminance),
		g(luminance),
		b(luminance)
	{
		// Nothing to do in here
	}

	inline Color3::Color3(uint8 luminance) :
		r(static_cast<float>(luminance) / 255.0f),
		g(static_cast<float>(luminance) / 255.0f),
		b(static_cast<float>(luminance) / 255.0f)
	{
		// Nothing to do in here
	}

	inline Color3::Color3(const Color3& source) :
		r(source.r),
		g(source.g),
		b(source.b)
	{
		// Nothing to do in here
	}

	inline Color3::~Color3()
	{
		// Nothing to do in here
	}

	inline uint32 Color3::toUInt32() const
	{
		// Use RGB as default output
		return toRGBUInt32();
	}

	inline uint32 Color3::toRGBUInt32() const
	{
		return static_cast<uint32>(
			((static_cast<uint8>(r * 255.0f) & 0xff) << 16) |
			((static_cast<uint8>(g * 255.0f) & 0xff) << 8) |
			(static_cast<uint8>(b * 255.0f) & 0xff)
			);
	}

	inline uint32 Color3::toBGRUInt32() const
	{
		return static_cast<uint32>(
			((static_cast<uint8>(b * 255.0f) & 0xff) << 16) |
			((static_cast<uint8>(g * 255.0f) & 0xff) << 8) |
			(static_cast<uint8>(r * 255.0f) & 0xff)
			);
	}

	inline void Color3::fromUInt32(uint32 encodedColor)
	{
		r = (static_cast<uint8>((encodedColor >> 16) & 0xff)) / 255.0f;
		g = (static_cast<uint8>((encodedColor >> 8) & 0xff)) / 255.0f;
		b = (static_cast<uint8>(encodedColor & 0xff)) / 255.0f;
	}

	inline void Color3::getRGB(float& _r, float& _g, float& _b) const
	{
		_r = r;
		_g = g;
		_b = b;
	}

	inline void Color3::getRGB(uint8& _r, uint8& _g, uint8& _b) const
	{
		_r = static_cast<uint8>(r * 255);
		_g = static_cast<uint8>(g * 255);
		_b = static_cast<uint8>(b * 255);
	}

	inline void Color3::setRGB(float _r, float _g, float _b)
	{
		r = _r;
		g = _g;
		b = _b;
	}

	inline void Color3::setRGB(uint8 _r, uint8 _g, uint8 _b)
	{
		r = static_cast<float>(_r) / 255.0f;
		g = static_cast<float>(_g) / 255.0f;
		b = static_cast<float>(_b) / 255.0f;
	}

	inline void Color3::getRGB(float rgb[]) const
	{
		rgb[0] = r;
		rgb[1] = g;
		rgb[2] = b;
	}

	inline void Color3::getRGB(uint8 rgb[]) const
	{
		rgb[0] = static_cast<uint8>(r * 255);
		rgb[1] = static_cast<uint8>(g * 255);
		rgb[2] = static_cast<uint8>(b * 255);
	}

	inline void Color3::setRGB(const float rgb[])
	{
		r = rgb[0];
		g = rgb[1];
		b = rgb[2];
	}

	inline void Color3::setRGB(const uint8 rgb[])
	{
		r = static_cast<float>(rgb[0])/  255.0f;
		g = static_cast<float>(rgb[1]) / 255.0f;
		b = static_cast<float>(rgb[2]) / 255.0f;
	}

	inline void Color3::setRGB(float value)
	{
		r = value;
		g = value;
		b = value;
	}

	inline void Color3::setRGB(uint8 value)
	{
		r = static_cast<float>(value) / 255.0f;
		g = static_cast<float>(value) / 255.0f;
		b = static_cast<float>(value) / 255.0f;
	}

	inline void Color3::saturate()
	{
		if (r < 0.0f)
			r = 0.0f;
		if (r > 1.0f)
			r = 1.0f;
		if (g < 0.0f)
			g = 0.0f;
		if (g > 1.0f)
			g = 1.0f;
		if (b < 0.0f)
			b = 0.0f;
		if (b > 1.0f)
			b = 1.0f;
	}


	//[-------------------------------------------------------]
	//[ Assignment operators                                  ]
	//[-------------------------------------------------------]
	inline Color3& Color3::operator =(const Color3& otherColor)
	{
		r = otherColor.r;
		g = otherColor.g;
		b = otherColor.b;

		return *this;
	}

	inline Color3& Color3::operator =(const float otherColor[])
	{
		r = otherColor[0];
		g = otherColor[1];
		b = otherColor[2];

		return *this;
	}

	inline Color3& Color3::operator =(float value)
	{
		r = g = b = value;

		return *this;
	}

	inline Color3::operator float *()
	{
		return &r;
	}

	inline Color3::operator const float *() const
	{
		return &r;
	}


	//[-------------------------------------------------------]
	//[ Comparison                                            ]
	//[-------------------------------------------------------]
	inline bool Color3::operator ==(const Color3& otherColor) const
	{
		return (r == otherColor.r && g == otherColor.g && b == otherColor.b);
	}

	inline bool Color3::operator !=(const Color3& otherColor) const
	{
		return (r != otherColor.r || g != otherColor.g || b != otherColor.b);
	}

	inline bool Color3::operator ==(float value) const
	{
		return (r == value && g == value && b == value);
	}

	inline bool Color3::operator !=(float value) const
	{
		return (r != value || g != value || b != value);
	}


	//[-------------------------------------------------------]
	//[ Misc                                                  ]
	//[-------------------------------------------------------]
	inline Color3 Color3::operator +(const Color3& otherColor) const
	{
		return Color3(r + otherColor.r, g + otherColor.g, b + otherColor.b);
	}

	inline Color3 Color3::operator +(float value) const
	{
		return Color3(r + value, g + value, b + value);
	}

	inline Color3& Color3::operator +=(const Color3& otherColor)
	{
		r += otherColor.r;
		g += otherColor.g;
		b += otherColor.b;

		return *this;
	}

	inline Color3& Color3::operator +=(float value)
	{
		r += value;
		g += value;
		b += value;

		return *this;
	}

	inline Color3 Color3::operator -() const
	{
		return Color3(-r, -g, -b);
	}

	inline Color3 Color3::operator -(const Color3& otherColor) const
	{
		return Color3(r - otherColor.r, g - otherColor.g, b - otherColor.b);
	}

	inline Color3 Color3::operator -(float value) const
	{
		return Color3(r - value, g - value, b - value);
	}

	inline Color3& Color3::operator -=(const Color3& otherColor)
	{
		r -= otherColor.r;
		g -= otherColor.g;
		b -= otherColor.b;

		return *this;
	}

	inline Color3& Color3::operator -=(float value)
	{
		r -= value;
		g -= value;
		b -= value;

		return *this;
	}

	inline Color3 Color3::operator *(const Color3& otherColor) const
	{
		return Color3(r * otherColor.r, g * otherColor.g, b * otherColor.b);
	}

	inline Color3 Color3::operator *(float value) const
	{
		return Color3(r * value, g * value, b * value);
	}

	inline Color3& Color3::operator *=(const Color3& otherColor)
	{
		r *= otherColor.r;
		g *= otherColor.g;
		b *= otherColor.b;

		return *this;
	}

	inline Color3& Color3::operator *=(float value)
	{
		r *= value;
		g *= value;
		b *= value;

		return *this;
	}

	inline Color3 Color3::operator /(const Color3& otherColor) const
	{
		return Color3(r / otherColor.r, g / otherColor.g, b / otherColor.b);
	}

	inline Color3 Color3::operator /(float value) const
	{
		return Color3(r / value, g / value, b / value);
	}

	inline Color3& Color3::operator /=(const Color3& otherColor)
	{
		r /= otherColor.r;
		g /= otherColor.g;
		b /= otherColor.b;

		return *this;
	}

	inline Color3& Color3::operator /=(float value)
	{
		r /= value;
		g /= value;
		b /= value;

		return *this;
	}

	inline float& Color3::operator [](int index)
	{
		return color[index];
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
