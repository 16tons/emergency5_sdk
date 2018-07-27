// Copyright (C) 2012-2017 Promotion Software GmbH


#pragma once


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <boost/container/flat_map.hpp>


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
	*     Linear gradient function
	*
	*  @remarks
	*     Evaluates a function defined by a mapping of key values (usually float) to points (e.g. float or glm::vec3).
	*     Returns linear interpolated values, or the left or right border value if evaluating outside of the range of defined keys.
	*/
	template<typename T, typename INPUT = float>
	class LinearGradientFunction
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline LinearGradientFunction()
		{
			// Nothing here
		}

		inline ~LinearGradientFunction()
		{
			// Nothing here
		}

		inline void clear()
		{
			mMap.clear();
		}

		inline bool empty() const
		{
			return mMap.empty();
		}

		inline void insert(INPUT x, const T& value)
		{
			mMap.emplace(x, value);
		}

		inline void erase(INPUT x)
		{
			mMap.erase(x);
		}

		inline T get(INPUT x) const
		{
			if (mMap.empty())
			{
				// Fallback: return default value
				return static_cast<INPUT>(0);
			}

			typename Map::const_iterator it = mMap.lower_bound(x);
			if (it == mMap.end())
				return mMap.rbegin()->second;
			if (it == mMap.begin())
				return mMap.begin()->second;

			typename Map::const_iterator it0 = it;
			--it0;

			// Interpolate
			const INPUT interpolationFactor = (x - it0->first) / (it->first - it0->first);
			return it0->second + (it->second - it0->second) * interpolationFactor;
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		typedef typename boost::container::flat_map<INPUT, T> Map;
		Map mMap;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
