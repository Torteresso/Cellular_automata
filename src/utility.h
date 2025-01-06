#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <numeric>

namespace Utility
{
	float average(std::vector<float> const& v){

		if(v.empty()){
			return 0;
		}

		auto const count = static_cast<float>(v.size());
		return std::reduce(v.begin(), v.end()) / count;
	}
}

#endif