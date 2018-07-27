// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/BoostAssertHandler.h>

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		/*!
		 * Simple quadratic or qubic bezier curve.
		 * note: all lengths are 2D (XY-Plane)!
		 */
		class Bezier
		{
		public:
			Bezier() : mDegree(0), mLength(0) {}
			Bezier(int degree, const glm::vec3* pts);
			Bezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);
			Bezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
			Bezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4);

			// compares degree and points
			bool operator== (const Bezier &curve_) const;
			bool operator!= (const Bezier &curve_) const;

			// TODO(co) No (save games) serialization required
			// bool SerializeTo(ESerializer *stream_) const;
			// bool SerializeFrom(ESerializer *stream_);

			int GetDegree() const { return mDegree; }
			const glm::vec3&  GetPoint(int i_) const { return mPoints[i_]; }
			float GetLength() const { return mLength; }

			inline glm::vec3 Eval(float t) const
			{
				switch (mDegree)
				{
					case 2:	return Eval2(t);	break;
					case 3:	return Eval3(t);	break;
					case 4:	return Eval4(t);	break;
					default:	QSF_ASSERT(false, "Bezier::Eval: Invalid degree", QSF_REACT_NONE);	break;
				}
				return glm::vec3();
			}

			inline glm::vec3 Eval0() const
			{
				return mPoints[0];
			}

			inline glm::vec3 Tangent(float t) const
			{
				switch (mDegree)
				{
					case 2:	return Tangent2(t);	break;
					case 3:	return Tangent3(t);	break;
					case 4:	return Tangent4(t);	break;
					default:	QSF_ASSERT(false, "Bezier::Eval: Invalid degree", QSF_REACT_NONE);	break;
				}
				return glm::vec3();
			}

			inline float ArcLength(float t) const { return Integrate(t); }
			float GetParamForLength(float len_) const;

			void Draw(unsigned char r, unsigned char g, unsigned char b, float w = 1.0f) const;

			// TODO(co) Wo we need writing to log?
			// void WriteToLog() const;

			void AdjustTargetY(float y_)
			{
				if (mDegree == 2)
					mPoints[2].y = y_;
				else
					if (mDegree == 3)
						mPoints[2].y = mPoints[3].y = y_;
					else
						mPoints[3].y = mPoints[4].y = y_;
			}

		private:
			inline glm::vec3 Eval2(float t) const
			{
				float t1 = 1 - t;
				float f0 = t1*t1;
				float f1 = 2 * t * t1;
				float f2 = t*t;
				return mPoints[0] * f0 + mPoints[1] * f1 + mPoints[2] * f2;
			}

			inline glm::vec3 Eval3(float t) const
			{
				float t1 = 1 - t;
				float t1_2 = t1*t1;
				float t_2 = t*t;
				float f0 = t1_2*t1;
				float f1 = 3 * t1_2*t;
				float f2 = 3 * t1*t_2;
				float f3 = t_2*t;
				return mPoints[0] * f0 + mPoints[1] * f1 + mPoints[2] * f2 + mPoints[3] * f3;
			}

			inline glm::vec3 Eval4(float t) const
			{
				float t1 = 1 - t;
				float t1_2 = t1*t1;
				float t_2 = t*t;

				float f0 = t1_2*t1_2;
				float f1 = 4 * t1*t1_2*t;
				float f2 = 6 * t1_2*t_2;
				float f3 = 4 * t1*t_2*t;
				float f4 = t_2*t_2;
				return mPoints[0] * f0 + mPoints[1] * f1 + mPoints[2] * f2 + mPoints[3] * f3 + mPoints[4] * f4;
			}

			inline glm::vec3 Tangent2(float t) const
			{
				float t1 = 1 - t;
				return (mDerivPoints[0] * t1 + mDerivPoints[1] * t) * 2.0f;
			}

			inline glm::vec3 Tangent3(float t) const
			{
				float t1 = 1 - t;
				float f0 = t1*t1;
				float f1 = 2 * t*t1;
				float f2 = t*t;
				return (mDerivPoints[0] * f0 + mDerivPoints[1] * f1 + mDerivPoints[2] * f2) * 3.0f;
			}

			inline glm::vec3 Tangent4(float t) const
			{
				float t1 = 1 - t;
				float t1_2 = t1*t1;
				float t_2 = t*t;

				float f0 = t1_2*t1;
				float f1 = 3 * t1_2*t;
				float f2 = 3 * t1*t_2;
				float f3 = t_2*t;
				return (mDerivPoints[0] * f0 + mDerivPoints[1] * f1 + mDerivPoints[2] * f2 + mDerivPoints[3] * f3) * 4.0f;
			}

			float Integrate(float a, float b) const;
			float Integrate(float b) const;

			char mDegree;
			glm::vec3 mPoints[5];
			glm::vec3 mDerivPoints[4];

			float mLength;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
