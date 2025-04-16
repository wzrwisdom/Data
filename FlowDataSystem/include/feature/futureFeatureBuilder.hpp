#ifndef FUTURE_FEATURE_BUILDER_HPP
#define FUTURE_FEATURE_BUILDER_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <core/Utils.hpp>
#include <core/MultiDimData.hpp>
#include <core/MarketDataTypes.hpp>
#include <feature/featureBuilderBase.hpp>

using namespace std;
using namespace std::chrono;
using namespace MarketData;

class FutureFeatureBuilder: public FeatureBuilderBase {
private:
    unordered_map<string, int> value_per_point_;
    int effect_time_index_;

public:
    FutureFeatureBuilder(vector<string> instruments, unordered_map<string, int> vpp);
    Utils::Timestamp build_snap_features(const FutureSnapData& data);
};
#endif // FUTURE_FEATURE_BUILDER_HPP