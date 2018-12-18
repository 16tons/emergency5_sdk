// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline AxisAlignedBoundingBox::AxisAlignedBoundingBox()
	{
		// Nothing do to in here
	}

	inline AxisAlignedBoundingBox::AxisAlignedBoundingBox(const AxisAlignedBoundingBox& source) :
		minimum(source.minimum),
		maximum(source.maximum)
	{
		// Nothing do to in here
	}

	inline AxisAlignedBoundingBox::AxisAlignedBoundingBox(const glm::vec3& minimumMaximum) :
		minimum(minimumMaximum),
		maximum(minimumMaximum)
	{
		// Nothing do to in here
	}

	inline AxisAlignedBoundingBox::AxisAlignedBoundingBox(const glm::vec3& minimumT, const glm::vec3& maximumT) :
		minimum(minimumT),
		maximum(maximumT)
	{
		// Nothing do to in here
	}

	inline AxisAlignedBoundingBox::AxisAlignedBoundingBox(float minimumX, float minimumY, float minimumZ, float maximumX, float maximumY, float maximumZ) :
		minimum(minimumX, minimumY, minimumZ),
		maximum(maximumX, maximumY, maximumZ)
	{
		// Nothing do to in here
	}

	inline AxisAlignedBoundingBox::~AxisAlignedBoundingBox()
	{
		// Nothing do to in here
	}

	inline void AxisAlignedBoundingBox::setToNumericLimit()
	{
		const float float_max = std::numeric_limits<float>().max();
		minimum = glm::vec3(-float_max, -float_max, -float_max);
		maximum = glm::vec3(+float_max, +float_max, +float_max);
	}

	inline glm::vec3 AxisAlignedBoundingBox::getCenter() const
	{
		return glm::vec3((maximum.x + minimum.x) * 0.5f, (maximum.y + minimum.y) * 0.5f, (maximum.z + minimum.z) * 0.5f);
	}

	inline float AxisAlignedBoundingBox::getSizeX() const
	{
		return maximum.x - minimum.x;
	}

	inline float AxisAlignedBoundingBox::getSizeY() const
	{
		return maximum.y - minimum.y;
	}

	inline float AxisAlignedBoundingBox::getSizeZ() const
	{
		return maximum.z - minimum.z;
	}

	inline AxisAlignedBoundingBox::Axis AxisAlignedBoundingBox::getLongestAxis() const
	{
		// Get differences
		const float differenceX = maximum.x - minimum.x;
		const float differenceY = maximum.y - minimum.y;
		const float differenceZ = maximum.z - minimum.z;

		// Return the longest axis
		if (differenceX > differenceY && differenceX > differenceZ)
			return X;
		else if (differenceY > differenceZ)
			return Y;
		else
			return Z;
	}

	inline float AxisAlignedBoundingBox::getLongestAxisLength() const
	{
		// Get differences
		const float differenceX = maximum.x - minimum.x;
		const float differenceY = maximum.y - minimum.y;
		const float differenceZ = maximum.z - minimum.z;

		// Return the longest axis
		if (differenceX > differenceY && differenceX > differenceZ)
			return differenceX;
		else if (differenceY > differenceZ)
			return differenceY;
		else
			return differenceZ;
	}

	inline float AxisAlignedBoundingBox::getEnclosingRadius() const
	{
		// Get the minimum/maximum squared length
		const float minimumSquaredLength = glm::dot(minimum, minimum);
		const float maximumSquaredLength = glm::dot(maximum, maximum);

		// The greater one has to be used for the radius
		return (maximumSquaredLength > minimumSquaredLength) ? sqrt(maximumSquaredLength) : sqrt(minimumSquaredLength);
	}

	inline float AxisAlignedBoundingBox::getInsideRadius() const
	{
		// Get differences
		const float differenceX = maximum.x - minimum.x;
		const float differenceY = maximum.y - minimum.y;
		const float differenceZ = maximum.z - minimum.z;

		// Return the longest axis
		if (differenceX > differenceY && differenceX > differenceZ)
			return differenceX * 0.5f;
		else if (differenceY > differenceZ)
			return differenceY * 0.5f;
		else
			return differenceZ * 0.5f;
	}

	inline float AxisAlignedBoundingBox::calculateSurface() const
	{
		// Get differences
		const float differenceX = maximum.x - minimum.x;
		const float differenceY = maximum.y - minimum.y;
		const float differenceZ = maximum.z - minimum.z;

		// Calculate the surface
		return (differenceX * differenceY + differenceX * differenceZ + differenceY * differenceZ) * 2;
	}

	inline float AxisAlignedBoundingBox::calculateVolume() const
	{
		return (maximum.x - minimum.x) *
			   (maximum.y - minimum.y) *
			   (maximum.z - minimum.z);
	}

	inline void AxisAlignedBoundingBox::clipByAABox(const AxisAlignedBoundingBox& enclosedAxisAlignedBoundingBox)
	{
		// x
		if (minimum.x > enclosedAxisAlignedBoundingBox.maximum.x)
		{
			minimum.x = maximum.x = enclosedAxisAlignedBoundingBox.maximum.x;
		}
		else
		{
			if (maximum.x < enclosedAxisAlignedBoundingBox.minimum.x)
			{
				minimum.x = maximum.x = enclosedAxisAlignedBoundingBox.minimum.x;
			}
			else
			{
				if (minimum.x < enclosedAxisAlignedBoundingBox.minimum.x)
					minimum.x = enclosedAxisAlignedBoundingBox.minimum.x;
				if (maximum.x > enclosedAxisAlignedBoundingBox.maximum.x)
					maximum.x = enclosedAxisAlignedBoundingBox.maximum.x;
			}
		}

		// y
		if (minimum.y > enclosedAxisAlignedBoundingBox.maximum.y)
		{
			minimum.y = maximum.y = enclosedAxisAlignedBoundingBox.maximum.y;
		}
		else
		{
			if (maximum.y < enclosedAxisAlignedBoundingBox.minimum.y)
			{
				minimum.y = maximum.y = enclosedAxisAlignedBoundingBox.minimum.y;
			}
			else
			{
				if (minimum.y < enclosedAxisAlignedBoundingBox.minimum.y)
					minimum.y = enclosedAxisAlignedBoundingBox.minimum.y;
				if (maximum.y > enclosedAxisAlignedBoundingBox.maximum.y)
					maximum.y = enclosedAxisAlignedBoundingBox.maximum.y;
			}
		}

		// z
		if (minimum.z > enclosedAxisAlignedBoundingBox.maximum.z)
		{
			minimum.z = maximum.z = enclosedAxisAlignedBoundingBox.maximum.z;
		}
		else
		{
			if (maximum.z < enclosedAxisAlignedBoundingBox.minimum.z)
			{
				minimum.z = maximum.z = enclosedAxisAlignedBoundingBox.minimum.z;
			}
			else
			{
				if (minimum.z < enclosedAxisAlignedBoundingBox.minimum.z)
					minimum.z = enclosedAxisAlignedBoundingBox.minimum.z;
				if (maximum.z > enclosedAxisAlignedBoundingBox.maximum.z)
					maximum.z = enclosedAxisAlignedBoundingBox.maximum.z;
			}
		}
	}

	inline void AxisAlignedBoundingBox::appendToCubicHull(const glm::vec3& vertex)
	{
		if (vertex.x < minimum.x)
			minimum.x = vertex.x;
		if (vertex.y < minimum.y)
			minimum.y = vertex.y;
		if (vertex.z < minimum.z)
			minimum.z = vertex.z;
		if (vertex.x > maximum.x)
			maximum.x = vertex.x;
		if (vertex.y > maximum.y)
			maximum.y = vertex.y;
		if (vertex.z > maximum.z)
			maximum.z = vertex.z;
	}

	inline void AxisAlignedBoundingBox::combineAABoxes(const AxisAlignedBoundingBox& axisAlignedBoundingBox)
	{
		appendToCubicHull(axisAlignedBoundingBox.minimum);
		appendToCubicHull(axisAlignedBoundingBox.maximum);
	}

	inline float AxisAlignedBoundingBox::getMinimumDistance(const glm::vec3& point) const
	{
		glm::vec3 distance(0.0f, 0.0f, 0.0f);

		if (point.x < minimum.x)
			distance.x = minimum.x - point.x;
		if (point.y < minimum.y)
			distance.y = minimum.y - point.y;
		if (point.z < minimum.z)
			distance.z = minimum.z - point.z;
		if (point.x > maximum.x)
			distance.x = maximum.x - point.x;
		if (point.y > maximum.y)
			distance.y = maximum.y - point.y;
		if (point.z > maximum.z)
			distance.z = maximum.z - point.z;

		return glm::length(distance);
	}

	inline AxisAlignedBoundingBox AxisAlignedBoundingBox::operator+ (const glm::vec3& vertex) const
	{
		return AxisAlignedBoundingBox(minimum+vertex, maximum+vertex);
	}

	inline AxisAlignedBoundingBox AxisAlignedBoundingBox::operator+ (float scalar) const
	{
		return AxisAlignedBoundingBox(minimum + scalar, maximum + scalar);
	}

	inline AxisAlignedBoundingBox& AxisAlignedBoundingBox::operator+= (const glm::vec3& vertex)
	{
		minimum += vertex;
		maximum += vertex;
		return *this;
	}

	inline AxisAlignedBoundingBox& AxisAlignedBoundingBox::operator+= (float scalar)
	{
		minimum += scalar;
		maximum += scalar;
		return *this;
	}

	inline AxisAlignedBoundingBox AxisAlignedBoundingBox::operator- (const glm::vec3& vertex) const
	{
		return AxisAlignedBoundingBox(minimum - vertex, maximum - vertex);
	}

	inline AxisAlignedBoundingBox AxisAlignedBoundingBox::operator- (float scalar) const
	{
		return AxisAlignedBoundingBox(minimum - scalar, maximum - scalar);
	}

	inline AxisAlignedBoundingBox& AxisAlignedBoundingBox::operator-= (const glm::vec3& vertex)
	{
		minimum -= vertex;
		maximum -= vertex;
		return *this;
	}

	inline AxisAlignedBoundingBox& AxisAlignedBoundingBox::operator-= (float scalar)
	{
		minimum -= scalar;
		maximum -= scalar;
		return *this;
	}

	inline AxisAlignedBoundingBox AxisAlignedBoundingBox::operator* (const glm::vec3& vertex) const
	{
		return AxisAlignedBoundingBox(minimum * vertex, maximum * vertex);
	}

	inline AxisAlignedBoundingBox AxisAlignedBoundingBox::operator* (float scalar) const
	{
		return AxisAlignedBoundingBox(minimum * scalar, maximum * scalar);
	}

	inline AxisAlignedBoundingBox& AxisAlignedBoundingBox::operator*= (const glm::vec3& vertex)
	{
		minimum *= vertex;
		maximum *= vertex;
		return *this;
	}

	inline AxisAlignedBoundingBox& AxisAlignedBoundingBox::operator*= (float scalar)
	{
		minimum *= scalar;
		maximum *= scalar;
		return *this;
	}

	inline AxisAlignedBoundingBox AxisAlignedBoundingBox::operator/ (const glm::vec3& vertex) const
	{
		return AxisAlignedBoundingBox(minimum / vertex, maximum / vertex);
	}

	inline AxisAlignedBoundingBox AxisAlignedBoundingBox::operator/ (float scalar) const
	{
		return AxisAlignedBoundingBox(minimum / scalar, maximum / scalar);
	}

	inline AxisAlignedBoundingBox& AxisAlignedBoundingBox::operator/= (const glm::vec3& vertex)
	{
		minimum /= vertex;
		maximum /= vertex;
		return *this;
	}

	inline AxisAlignedBoundingBox& AxisAlignedBoundingBox::operator/= (float scalar)
	{
		minimum /= scalar;
		maximum /= scalar;
		return *this;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
