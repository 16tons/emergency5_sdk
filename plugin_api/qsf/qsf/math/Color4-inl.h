// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Special operators                                     ]
	//[-------------------------------------------------------]
	inline Color4 operator *(float value, const Color4& rgba)
	{
		return Color4(rgba.r * value, rgba.g * value, rgba.b * value, rgba.a * value);
	}


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	inline Color4 Color4::interpolate(const Color4& color1, const Color4& color2, float factor)
	{
		return Color4(color1 + (color2 - color1) * factor);
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Color4::Color4() :
		r(0.0f),
		g(0.0f),
		b(0.0f),
		a(0.0f)
	{
		// Nothing to do in here
	}

	inline Color4::Color4(float _r, float _g, float _b, float _a) :
		r(_r),
		g(_g),
		b(_b),
		a(_a)
	{
		// Nothing to do in here
	}

	inline Color4::Color4(uint8 _r, uint8 _g, uint8 _b, uint8 _a) :
		r(static_cast<float>(_r) / 255.0f),
		g(static_cast<float>(_g) / 255.0f),
		b(static_cast<float>(_b) / 255.0f),
		a(static_cast<float>(_a) / 255.0f)
	{
		// Nothing to do in here
	}

	inline Color4::Color4(const float rgba[]) :
		r(rgba[0]),
		g(rgba[1]),
		b(rgba[2]),
		a(rgba[3])
	{
		// Nothing to do in here
	}

	inline Color4::Color4(const uint8 rgba[]) :
		r(rgba[0] / 255.0f),
		g(rgba[1] / 255.0f),
		b(rgba[2] / 255.0f),
		a(rgba[3] / 255.0f)
	{
		// Nothing to do in here
	}

	inline Color4::Color4(float luminance) :
		r(luminance),
		g(luminance),
		b(luminance),
		a(luminance)
	{
		// Nothing to do in here
	}

	inline Color4::Color4(uint8 luminance) :
		r(static_cast<float>(luminance) / 255.0f),
		g(static_cast<float>(luminance) / 255.0f),
		b(static_cast<float>(luminance) / 255.0f),
		a(static_cast<float>(luminance) / 255.0f)
	{
		// Nothing to do in here
	}

	inline Color4::Color4(const Color4& source) :
		r(source.r),
		g(source.g),
		b(source.b),
		a(source.a)
	{
		// Nothing to do in here
	}

	inline Color4::~Color4()
	{
		// Nothing to do in here
	}

	inline uint32 Color4::toUInt32() const
	{
		return toARGBUInt32();
	}

	inline uint32 Color4::toARGBUInt32() const
	{
		return static_cast<uint32>(((static_cast<uint8>(a * 255.0f) & 0xff) << 24) |
			((static_cast<uint8>(r * 255.0f) & 0xff) << 16) |
			((static_cast<uint8>(g * 255.0f) & 0xff) << 8) |
			(static_cast<uint8>(b * 255.0f) & 0xff));
	}

	inline uint32 Color4::toRGBAUInt32() const
	{
		return static_cast<uint32>(((static_cast<uint8>(r * 255.0f) & 0xff) << 24) |
			((static_cast<uint8>(g * 255.0f) & 0xff) << 16) |
			((static_cast<uint8>(b * 255.0f) & 0xff) << 8) |
			(static_cast<uint8>(a * 255.0f) & 0xff));
	}

	inline uint32 Color4::toRGBUInt32() const
	{
		return static_cast<uint32>(
			((static_cast<uint8>(r * 255.0f) & 0xff) << 16) |
			((static_cast<uint8>(g * 255.0f) & 0xff) << 8) |
			(static_cast<uint8>(b * 255.0f) & 0xff)
			);
	}

	inline void Color4::fromUInt32(uint32 packedColor)
	{
		r = (static_cast<uint8>(packedColor & 0xff)) / 255.0f;
		g = (static_cast<uint8>((packedColor >> 8) & 0xff)) / 255.0f;
		b = (static_cast<uint8>((packedColor >> 16) & 0xff)) / 255.0f;
		a = (static_cast<uint8>((packedColor >> 24) & 0xff)) / 255.0f;
	}

	inline void Color4::getRGBA(float& _r, float& _g, float& _b, float& _a) const
	{
		_r = r;
		_g = g;
		_b = b;
		_a = a;
	}

	inline void Color4::getRGBA(uint8& _r, uint8& _g, uint8& _b, uint8& _a) const
	{
		_r = static_cast<uint8>(r * 255);
		_g = static_cast<uint8>(g * 255);
		_b = static_cast<uint8>(b * 255);
		_a = static_cast<uint8>(a * 255);
	}

	inline void Color4::setRGBA(float _r, float _g, float _b, float _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	inline void Color4::setRGBA(uint8 _r, uint8 _g, uint8 _b, uint8 _a)
	{
		r = static_cast<float>(_r) / 255.0f;
		g = static_cast<float>(_g) / 255.0f;
		b = static_cast<float>(_b) / 255.0f;
		a = static_cast<float>(_a) / 255.0f;
	}

	inline void Color4::getRGBA(float rgba[]) const
	{
		rgba[0] = r;
		rgba[1] = g;
		rgba[2] = b;
		rgba[3] = a;
	}

	inline void Color4::getRGBA(uint8 rgba[]) const
	{
		rgba[0] = static_cast<uint8>(r * 255);
		rgba[1] = static_cast<uint8>(g * 255);
		rgba[2] = static_cast<uint8>(b * 255);
		rgba[3] = static_cast<uint8>(a * 255);
	}

	inline void Color4::setRGBA(const float rgba[])
	{
		r = rgba[0];
		g = rgba[1];
		b = rgba[2];
		a = rgba[3];
	}

	inline void Color4::setRGBA(const uint8 rgba[])
	{
		r = static_cast<float>(rgba[0])/  255.0f;
		g = static_cast<float>(rgba[1]) / 255.0f;
		b = static_cast<float>(rgba[2]) / 255.0f;
		a = static_cast<float>(rgba[3]) / 255.0f;
	}

	inline void Color4::setRGBA(float value)
	{
		r = value;
		g = value;
		b = value;
		a = value;
	}

	inline void Color4::setRGBA(uint8 value)
	{
		r = static_cast<float>(value) / 255.0f;
		g = static_cast<float>(value) / 255.0f;
		b = static_cast<float>(value) / 255.0f;
		a = static_cast<float>(value) / 255.0f;
	}

	inline void Color4::saturate()
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
		if (a < 0.0f)
			a = 0.0f;
		if (a > 1.0f)
			a = 1.0f;
	}


	//[-------------------------------------------------------]
	//[ Assignment operators                                  ]
	//[-------------------------------------------------------]
	inline Color4& Color4::operator =(const Color4& otherColor)
	{
		r = otherColor.r;
		g = otherColor.g;
		b = otherColor.b;
		a = otherColor.a;

		return *this;
	}

	inline Color4& Color4::operator =(const float rgba[])
	{
		r = rgba[0];
		g = rgba[1];
		b = rgba[2];
		a = rgba[3];

		return *this;
	}

	inline Color4& Color4::operator =(float value)
	{
		r = g = b = a = value;

		return *this;
	}

	inline Color4::operator float *()
	{
		return &r;
	}

	inline Color4::operator const float *() const
	{
		return &r;
	}


	//[-------------------------------------------------------]
	//[ Comparison                                            ]
	//[-------------------------------------------------------]
	inline bool Color4::operator ==(const Color4& otherColor) const
	{
		return (r == otherColor.r && g == otherColor.g && b == otherColor.b && a == otherColor.a);
	}

	inline bool Color4::operator !=(const Color4& otherColor) const
	{
		return (r != otherColor.r || g != otherColor.g || b != otherColor.b || a != otherColor.a);
	}

	inline bool Color4::operator ==(float value) const
	{
		return (r == value && g == value && b == value && a == value);
	}

	inline bool Color4::operator !=(float value) const
	{
		return (r != value || g != value || b != value || a != value);
	}


	//[-------------------------------------------------------]
	//[ Misc                                                  ]
	//[-------------------------------------------------------]
	inline Color4 Color4::operator +(const Color4& otherColor) const
	{
		return Color4(r + otherColor.r, g + otherColor.g, b + otherColor.b, a + otherColor.a);
	}

	inline Color4 Color4::operator +(float value) const
	{
		return Color4(r + value, g + value, b + value, a + value);
	}

	inline Color4& Color4::operator +=(const Color4& otherColor)
	{
		r += otherColor.r;
		g += otherColor.g;
		b += otherColor.b;
		a += otherColor.a;

		return *this;
	}

	inline Color4& Color4::operator +=(float value)
	{
		r += value;
		g += value;
		b += value;
		a += value;

		return *this;
	}

	inline Color4 Color4::operator -() const
	{
		return Color4(-r, -g, -b, -a);
	}

	inline Color4 Color4::operator -(const Color4& otherColor) const
	{
		return Color4(r - otherColor.r, g - otherColor.g, b - otherColor.b, a - otherColor.a);
	}

	inline Color4 Color4::operator -(float value) const
	{
		return Color4(r - value, g - value, b - value, a - value);
	}

	inline Color4& Color4::operator -=(const Color4& otherColor)
	{
		r -= otherColor.r;
		g -= otherColor.g;
		b -= otherColor.b;
		a -= otherColor.a;

		return *this;
	}

	inline Color4& Color4::operator -=(float value)
	{
		r -= value;
		g -= value;
		b -= value;
		a -= value;

		return *this;
	}

	inline Color4 Color4::operator *(const Color4& otherColor) const
	{
		return Color4(r * otherColor.r, g * otherColor.g, b * otherColor.b, a * otherColor.a);
	}

	inline Color4 Color4::operator *(float value) const
	{
		return Color4(r * value, g * value, b * value, a * value);
	}

	inline Color4& Color4::operator *=(const Color4& otherColor)
	{
		r *= otherColor.r;
		g *= otherColor.g;
		b *= otherColor.b;
		a *= otherColor.a;

		return *this;
	}

	inline Color4& Color4::operator *=(float value)
	{
		r *= value;
		g *= value;
		b *= value;
		a *= value;

		return *this;
	}

	inline Color4 Color4::operator /(const Color4& otherColor) const
	{
		return Color4(r / otherColor.r, g / otherColor.g, b / otherColor.b, a / otherColor.a);
	}

	inline Color4 Color4::operator /(float value) const
	{
		return Color4(r / value, g / value, b / value, a / value);
	}

	inline Color4& Color4::operator /=(const Color4& rgba)
	{
		r /= rgba.r;
		g /= rgba.g;
		b /= rgba.b;
		a /= rgba.a;

		return *this;
	}

	inline Color4& Color4::operator /=(float value)
	{
		r /= value;
		g /= value;
		b /= value;
		a /= value;

		return *this;
	}

	inline float& Color4::operator [](int index)
	{
		return color[index];
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
