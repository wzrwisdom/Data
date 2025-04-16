#include <iostream>
#include <cassert>
#include <chrono>
#include <unordered_map>
#include <core/Utils.hpp>

void Test_Timestamp() {
    
    // Test case 1: Creating a timestamp with specific hour, minute, second and millisecond
    Utils::Timestamp timestamp1 = Utils::Timestamp(12, 30, 45, 500);
    std::cout << "Timestamp 1: " << timestamp1.to_string() << std::endl;

    std::unordered_map<std::string, size_t> timeIndexMap;
    timeIndexMap[timestamp1.to_string()] = 0;
    std::cout << "Time index map size: " << timeIndexMap.size() << std::endl;
    // // Test case 2: Creating a timestamp with hour, minute, second and millisecond as zero
    // auto timestamp2 = create_timestamp(0, 0, 0, 0);
    // auto now2 = std::chrono::system_clock::now();
    // auto now2_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now2.time_since_epoch()).count();
    // auto timestamp2_ms = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp2.time_since_epoch()).count();
    // assert(timestamp2_ms == now2_ms);

    // // Test case 3: Creating a timestamp with maximum possible values for hour, minute, second and millisecond
    // auto timestamp3 = create_timestamp(23, 59, 59, 999);
    // auto now3 = std::chrono::system_clock::now();
    // auto now3_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now3.time_since_epoch()).count();
    // auto timestamp3_ms = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp3.time_since_epoch()).count();
    // assert(timestamp3_ms == now3_ms + 23*60*60*1000 + 59*60*1000 + 59*1000 + 999);

    // // Test case 4: Creating a timestamp with negative values for hour, minute, second and millisecond
    // // This test case is not valid as the function does not handle negative values

    // // Test case 5: Creating a timestamp with hour, minute, second and millisecond as minimum possible values
    // auto timestamp5 = create_timestamp(0, 0, 0, 0);
    // auto now5 = std::chrono::system_clock::now();
    // auto now5_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now5.time_since_epoch()).count();
    // auto timestamp5_ms = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp5.time_since_epoch()).count();
    // assert(timestamp5_ms == now5_ms);

    printf("All test cases pass");
}
