// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		const int MAX_INT = 0x7fffffff;
		const int MIN_INT = ~MAX_INT;

		extern bool IntersectLines2DPoint(	double start1x_, double start1y_, double end1x_, double end1y_,
										double start2x_, double start2y_, double end2x_, double end2y_,
										double& resultX, double& resultY);


		extern bool IntersectLines2D(	double start1x_, double start1y_, double end1x_, double end1y_,
										double start2x_, double start2y_, double end2x_, double end2y_,
										double& resultT, double& resultX, double& resultY);


		extern inline void CreateRotationMatrixAroundAxis( float *matrix_, float xAxis_, float yAxis_, float zAxis_, float rad_);

		extern float NormalizeEulerAngle(float Angle_);	// bringt Angle_ auf den Wertebereich -180 ... +180
		extern float NormalizeRadAngle(float Angle_);	// bringt Angle_ auf den Wertebereich -PI ... +PI

		inline float Square(float X_)
		{
			return X_ * X_;
		}

		inline float Log2(float X_)
		{
			return logf(X_) / logf(2.0f);
		}

		inline float Round(float X_)
		{
			return floorf(X_ + 0.5f);
		}

		inline int RoundToInt(float X_)
		{
			return static_cast<int>(Round(X_));
		}

		/*!
		 *	\brief Zufallszahl aus dem Intervall [0,1]
		 */
		inline float Random01()
		{
			return rand() / 32767.0f;
		}

		inline float Random(float min_, float max_)
		{
			return Random01() * (max_ - min_) + min_;
		}

		/*!
		 *	\brief Zufallszahl aus dem Intervall [0,1[
		 */
		inline float Random0X1()
		{
			return rand() / 32768.0f;
		}

		/*!
		 *	\brief Zufallszahl aus dem Intervall [-1,+1]
		 */
		inline float Random11()
		{
			return rand() / 16383.5f - 1.0f;
		}

		/*!
		 *	\brief Zufallszahl aus dem Intervall ]-1,+1[
		 */
		inline float Random1X1()
		{
			return (rand() + 1) / 16384.5f - 1.0f;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
