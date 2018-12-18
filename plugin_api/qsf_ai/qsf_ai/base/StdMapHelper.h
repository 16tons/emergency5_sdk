// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


// file Contains helper functionality for working with std::map
namespace qsf
{
	// Enters a value into a map no matter whether there is already a value with an equivalent key previously while minimizing the number of copies
	template <typename Map>
	void efficientInsertOrOverwrite(Map& map, const typename Map::key_type& key, const typename Map::mapped_type& value)
	{
		const auto lowerBound = map.lower_bound(key);
		if (lowerBound != map.end() && !map.key_comp()(key, lowerBound->first))
		{
			// We have found a real value and it compares equal - so we replace the value instead
			lowerBound->second = value;
			return;
		}
		// else new value - insert with hint
		map.insert(lowerBound, std::make_pair(key, value));
	}

	// not totally correct here, as it can work with any STL container: calls action() for successive pairs in the container
	template <typename T, typename Action>
	void for_pairs(T begin, T end, Action action)
	{
		auto second = begin;
		if (second == end)
			return; // empty

		auto first = second++;
		if (second == end)
			return; // only size 1

		while (second != end)
		{
			action(*first, *second);
			first = second++;
		}
	}
}
