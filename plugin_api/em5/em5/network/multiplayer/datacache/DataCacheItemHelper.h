// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_game/network/BitStream.h>

#include <qsf/base/error/ErrorHandling.h>
#include <qsf/math/EulerAngles.h>

#include <glm/gtc/quaternion.hpp>


//[-------------------------------------------------------]
//[ Local defines                                         ]
//[-------------------------------------------------------]
// This define is used to enable the cache debug logging (per cache entry logging how many bits where written/read to/from BitStream )
// This debug logging are used to find out what went wrong on the client side. Mostly in conjunction with game recorder/player data
//#define ENABLE_MULTIPLAYER_CACHE_DEBUG 1


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{
		namespace detail
		{


			//[-------------------------------------------------------]
			//[ Methods                                               ]
			//[-------------------------------------------------------]
			template<typename T>
			static void writeChanges(const bool valueChanged, const T& newValue, qsf::game::BitStream& bitStream)
			{
				// Write if data has changed
				bitStream.write(valueChanged);
				if (valueChanged)
				{
					// Write changed value
					bitStream.write(newValue);
				}
			}

			template<typename T>
			static void writeChanges(const bool valueChanged, const T& newValue, size_t bitCount, qsf::game::BitStream& bitStream)
			{
				// Write if data has changed
				bitStream.write(valueChanged);
				if (valueChanged)
				{
					// Write changed value
					bitStream.write(newValue, bitCount);
				}
			}

			// TODO(ca) Got a warning here: "explicit specialization cannot have storage class"... this looks like a specialized function template... have you tried overloading instead?
			// TODO(ca) This causes a 'Unused function' warning due to static storage class - please find a better solution
			template<>
			static void writeChanges(const bool valueChanged, const glm::vec2& newValue, qsf::game::BitStream& bitStream)
			{
				// Write if data has changed
				bitStream.write(valueChanged);
				if (valueChanged)
				{
					// Write changed value
					bitStream.write(newValue.x);
					bitStream.write(newValue.y);
				}
			}

			// TODO(ca) Got a warning here: "explicit specialization cannot have storage class"... this looks like a specialized function template... have you tried overloading instead?
			// TODO(ca) This causes a 'Unused function' warning due to static storage class - please find a better solution
			template<>
			static void writeChanges(const bool valueChanged, const glm::vec3& newValue, qsf::game::BitStream& bitStream)
			{
				// Write if data has changed
				bitStream.write(valueChanged);
				if (valueChanged)
				{
					// Write changed value
					bitStream.write(newValue.x);
					bitStream.write(newValue.y);
					bitStream.write(newValue.z);
				}
			}

			// TODO(ca) Got a warning here: "explicit specialization cannot have storage class"... this looks like a specialized function template... have you tried overloading instead?
			// TODO(ca) This causes a 'Unused function' warning due to static storage class - please find a better solution
			template<>
			static void writeChanges(const bool valueChanged, const glm::quat& newValue, qsf::game::BitStream& bitStream)
			{
				// Write if data has changed
				bitStream.write(valueChanged);
				if (valueChanged)
				{
					// Write changed value
					bitStream.write(newValue.x);
					bitStream.write(newValue.y);
					bitStream.write(newValue.z);
					bitStream.write(newValue.w);
				}
			}

			template<typename T>
			static bool checkForChanges(const T& newValue, T& currentValue)
			{
				bool valueChanged = newValue != currentValue;
				if (valueChanged)
				{
					// Save changed data
					currentValue = newValue;
				}

				return valueChanged;
			}

			template<typename T>
			static bool checkChangedAndWrite(const T& newValue, T& currentValue, qsf::game::BitStream& bitStream, bool force)
			{
				bool valueChanged = newValue != currentValue || force;
				// Write if data has changed
				bitStream.write(valueChanged);
				if (valueChanged)
				{
					// Save changed data
					currentValue = newValue;

					// Write changed value
					bitStream.write(newValue);
				}

				return valueChanged;
			}

			// TODO(ca) Got a warning here: "explicit specialization cannot have storage class"... this looks like a specialized function template... have you tried overloading instead?
			// TODO(ca) This causes a 'Unused function' warning due to static storage class - please find a better solution
			template<>
			static bool checkChangedAndWrite(const glm::vec2& newValue, glm::vec2& currentValue, qsf::game::BitStream& bitStream, bool force)
			{
				bool valueChanged = newValue != currentValue || force;
				// Write if data has changed
				bitStream.write(valueChanged);
				if (valueChanged)
				{
					// Save changed data
					currentValue = newValue;

					// Write changed value
					bitStream.write(newValue.x);
					bitStream.write(newValue.y);
				}

				return valueChanged;
			}

			// TODO(ca) Got a warning here: "explicit specialization cannot have storage class"... this looks like a specialized function template... have you tried overloading instead?
			// TODO(ca) This causes a 'Unused function' warning due to static storage class - please find a better solution
			template<>
			static bool checkChangedAndWrite(const glm::vec3& newValue, glm::vec3& currentValue, qsf::game::BitStream& bitStream, bool force)
			{
				bool valueChanged = newValue != currentValue || force;
				// Write if data has changed
				bitStream.write(valueChanged);
				if (valueChanged)
				{
					// Save changed data
					currentValue = newValue;

					// Write changed value
					bitStream.write(newValue.x);
					bitStream.write(newValue.y);
					bitStream.write(newValue.z);
				}

				return valueChanged;
			}

			// TODO(ca) Got a warning here: "explicit specialization cannot have storage class"... this looks like a specialized function template... have you tried overloading instead?
			// TODO(ca) This causes a 'Unused function' warning due to static storage class - please find a better solution
			template<>
			static bool checkChangedAndWrite(const glm::quat& newValue, glm::quat& currentValue, qsf::game::BitStream& bitStream, bool force)
			{
				bool valueChanged = newValue != currentValue || force;
				// Write if data has changed
				bitStream.write(valueChanged);
				if (valueChanged)
				{
					// Save changed data
					currentValue = newValue;

					// Write changed value
					bitStream.write(newValue.x);
					bitStream.write(newValue.y);
					bitStream.write(newValue.z);
					bitStream.write(newValue.w);
				}

				return valueChanged;
			}

			// TODO(ca) This causes a 'Unused function' warning due to static storage class - please find a better solution
			static bool checkChanged(const qsf::game::BitStream& bitStream)
			{
				// Read if data has changed
				bool haveData = false;
				if (!bitStream.read(haveData))
				{
					QSF_ERROR("DataCacheItem: could not read changed state from stream", QSF_REACT_THROW);
				}

				return haveData;
			}

			template<typename T>
			static bool checkChangedAndRead(const qsf::game::BitStream& bitStream, T& currentValue)
			{
				// Read if data has changed
				bool haveData = false;
				bitStream.read(haveData);

				if (haveData)
				{
					// Try read the value and save it on success
					T newValue;
					if (bitStream.read(newValue))
					{
						currentValue = newValue;
					}
					else
					{
						QSF_ERROR("DataCacheItem: could not read value from stream", QSF_REACT_THROW);
					}
				}

				// Return if data has changed
				return haveData;
			}

			template<typename T>
			static bool checkChangedAndRead(const qsf::game::BitStream& bitStream, size_t bitCount, T& currentValue)
			{
				// Read if data has changed
				bool haveData = false;
				bitStream.read(haveData);

				if (haveData)
				{
					// Try read the value and save it on success
					T newValue;
					if (bitStream.read(newValue, bitCount))
					{
						currentValue = newValue;
					}
					else
					{
						QSF_ERROR("DataCacheItem: could not read value from stream", QSF_REACT_THROW);
					}
				}

				// Return if data has changed
				return haveData;
			}

			// TODO(ca) Got a warning here: "explicit specialization cannot have storage class"... this looks like a specialized function template... have you tried overloading instead?
			template<>
			static bool checkChangedAndRead(const qsf::game::BitStream& bitStream, glm::vec2& currentValue)
			{
				// Read if data has changed
				bool haveData = false;
				bitStream.read(haveData);

				if (haveData)
				{
					// Try read the value and save it on success
					glm::vec2 newValue;
					if (!bitStream.read(newValue.x))
					{
						QSF_ERROR("DataCacheItem: could not read value from stream", QSF_REACT_THROW);
					}

					if (!bitStream.read(newValue.y))
					{
						QSF_ERROR("DataCacheItem: could not read value from stream", QSF_REACT_THROW);
					}

					currentValue = newValue;
				}

				// Return if data has changed
				return haveData;
			}

			// TODO(ca) Got a warning here: "explicit specialization cannot have storage class"... this looks like a specialized function template... have you tried overloading instead?
			template<>
			static bool checkChangedAndRead(const qsf::game::BitStream& bitStream, glm::vec3& currentValue)
			{
				// Read if data has changed
				bool haveData = false;
				bitStream.read(haveData);

				if (haveData)
				{
					// Try read the value and save it on success
					glm::vec3 newValue;
					if (!bitStream.read(newValue.x))
					{
						QSF_ERROR("DataCacheItem: could not read value from stream", QSF_REACT_THROW);
					}

					if (!bitStream.read(newValue.y))
					{
						QSF_ERROR("DataCacheItem: could not read value from stream", QSF_REACT_THROW);
					}

					if (!bitStream.read(newValue.z))
					{
						QSF_ERROR("DataCacheItem: could not read value from stream", QSF_REACT_THROW);
					}

					currentValue = newValue;
				}

				// Return if data has changed
				return haveData;
			}

			// TODO(ca) Got a warning here: "explicit specialization cannot have storage class"... this looks like a specialized function template... have you tried overloading instead?
			template<>
			static bool checkChangedAndRead(const qsf::game::BitStream& bitStream, glm::quat& currentValue)
			{
				// Read if data has changed
				bool haveData = false;
				bitStream.read(haveData);

				if (haveData)
				{
					// Try read the value and save it on success
					glm::quat newValue;
					if (!bitStream.read(newValue.x))
					{
						QSF_ERROR("DataCacheItem: could not read value from stream", QSF_REACT_THROW);
					}

					if (!bitStream.read(newValue.y))
					{
						QSF_ERROR("DataCacheItem: could not read value from stream", QSF_REACT_THROW);
					}

					if (!bitStream.read(newValue.z))
					{
						QSF_ERROR("DataCacheItem: could not read value from stream", QSF_REACT_THROW);
					}

					if (!bitStream.read(newValue.w))
					{
						QSF_ERROR("DataCacheItem: could not read value from stream", QSF_REACT_THROW);
					}

					currentValue = newValue;
				}

				// Return if data has changed
				return haveData;
			}

			// TODO(ca) Unused local function
			/*
			static bool checkChangedAndWritePosition(const glm::vec3& newValue, glm::vec3& currentValue, qsf::game::BitStream& bitStream, bool force)
			{
				// Positions are stored as 16bit signed values (0.1 meters precision)

				int16 newValueX = static_cast<int16>(newValue.x*10);
				int16 newValueY = static_cast<int16>(newValue.y*10);
				int16 newValueZ = static_cast<int16>(newValue.z*10);

				int16 currentValueX = static_cast<int16>(currentValue.x*10);
				int16 currentValueY = static_cast<int16>(currentValue.y*10);
				int16 currentValueZ = static_cast<int16>(currentValue.z*10);

				bool xValueChanged = newValueX != currentValueX || force;
				bool yValueChanged = newValueY != currentValueY || force;
				bool zValueChanged = newValueZ != currentValueZ || force;

				if (xValueChanged || yValueChanged || zValueChanged)
				{
					// Save changed data
					currentValue = newValue;
				}

				bitStream.write(xValueChanged);
				if (xValueChanged)
				{
					// Write changed value
					bitStream.write(newValueX);
				}

				bitStream.write(yValueChanged);
				if (yValueChanged)
				{
					// Write changed value
					bitStream.write(newValueY);
				}

				bitStream.write(zValueChanged);
				if (zValueChanged)
				{
					// Write changed value
					bitStream.write(newValueZ);
				}

				return xValueChanged || yValueChanged || zValueChanged;
			}
			*/

			// TODO(ca) Unused local function
			/*
			static bool checkChangedAndReadPosition(const qsf::game::BitStream& bitStream, glm::vec3& currentValue)
			{
				// Positions are stored as 16bit signed values (0.1 meters precision)
				bool result = false;
				// Check and read x value
				int16 xValue;
				if (checkChangedAndRead(bitStream, xValue))
				{
					result = true;
					currentValue.x = static_cast<float>(xValue/10.0f);
				}

				// Check and read y value
				int16 yValue;
				if (checkChangedAndRead(bitStream, yValue))
				{
					result = true;
					currentValue.y = static_cast<float>(yValue/10.0f);
				}

				// Check and read y value
				int16 zValue;
				if (checkChangedAndRead(bitStream, zValue))
				{
					result = true;
					currentValue.z = static_cast<float>(zValue/10.0f);
				}

				return result;
			}
			*/

			// TODO(ca) This causes a 'Unused function' warning due to static storage class - please find a better solution
			static float unwrapEulerAngle(int8 wrappedAngle)
			{
				return static_cast<float>(wrappedAngle * 360.0f / 256.0f);
			}

			// TODO(ca) This causes a 'Unused function' warning due to static storage class - please find a better solution
			static int8 wrapEulerAngle(float angle)
			{
				return static_cast<int8>(angle * 256 / 360);
			}

			// TODO(ca) Unused local function
			/*
			static bool checkChangedAndWriteRotation(const glm::quat& newValue, glm::quat& currentValue, qsf::game::BitStream& bitStream, bool force)
			{
				// Rotation are stored as 8bit signed values (as euler angles, -180° - 180°)
				glm::vec3 newEulerAngles = glm::degrees(qsf::EulerAngles::quaternionToEuler(newValue));
				glm::vec3 currentEulerAngles = glm::degrees(qsf::EulerAngles::quaternionToEuler(currentValue));

				int8 newValueX = wrapEulerAngle(newEulerAngles.x);
				int8 newValueY = wrapEulerAngle(newEulerAngles.y);
				int8 newValueZ = wrapEulerAngle(newEulerAngles.z);

				int8 currentValueX = wrapEulerAngle(currentEulerAngles.x);
				int8 currentValueY = wrapEulerAngle(currentEulerAngles.y);
				int8 currentValueZ = wrapEulerAngle(currentEulerAngles.z);

				bool xValueChanged = newValueX != currentValueX || force;
				bool yValueChanged = newValueY != currentValueY || force;
				bool zValueChanged = newValueZ != currentValueZ || force;

				if (xValueChanged || yValueChanged || zValueChanged)
				{
					// Save changed data
					currentValue = newValue;
				}

				bitStream.write(xValueChanged);
				if (xValueChanged)
				{
					// Write changed value
					bitStream.write(newValueX);
				}

				bitStream.write(yValueChanged);
				if (yValueChanged)
				{
					// Write changed value
					bitStream.write(newValueY);
				}

				bitStream.write(zValueChanged);
				if (zValueChanged)
				{
					// Write changed value
					bitStream.write(newValueZ);
				}

				return xValueChanged || yValueChanged || currentValueZ;
			}
			*/

			// TODO(ca) Unused local function
			/*
			static bool checkChangedAndReadRotation(const qsf::game::BitStream& bitStream, glm::quat& currentValue)
			{
				// Positions are stored as 8bit signed values (as euler angles, -180° - 180°)
				bool result = false;
				glm::vec3 eulerAngles = glm::degrees(qsf::EulerAngles::quaternionToEuler(currentValue));

				// Check and read x value
				int8 xValue;
				if (checkChangedAndRead(bitStream, xValue))
				{
					result = true;
					eulerAngles.x = unwrapEulerAngle(xValue);
				}

				// Check and read y value
				int8 yValue;
				if (checkChangedAndRead(bitStream, yValue))
				{
					result = true;
					eulerAngles.y = unwrapEulerAngle(yValue);
				}

				// Check and read y value
				int8 zValue;
				if (checkChangedAndRead(bitStream, zValue))
				{
					result = true;
					eulerAngles.z = unwrapEulerAngle(zValue);
				}

				if (result)
				{
					currentValue = qsf::EulerAngles::eulerToQuaternion(glm::radians(eulerAngles));
				}

				return result;
			}
			*/
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
