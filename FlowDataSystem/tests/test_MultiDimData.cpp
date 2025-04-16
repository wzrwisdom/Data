#include <vector>
#include <string>
#include <core/MultiDimData.hpp>
#include <core/Utils.hpp>

void Test_MultiDimData() {
    // 初始化数据
    std::vector<std::string> underlyings = {"AAPL", "MSFT", "GOOG"};
    std::vector<Utils::Timestamp> timePoints = {"2023-01-01", "2023-01-02", "2023-01-03"};
    std::vector<std::string> features = {"ClosePrice", "Volume", "EMA_10"};
    
    ThreeDimData marketData(underlyings, timePoints, features);
    
    // 填充数据
    marketData.at("AAPL", "2023-01-01", "ClosePrice") = 182.01;
    marketData.at("AAPL", "2023-01-01", "Volume") = 12345678;
    marketData.at("MSFT", "2023-01-02", "ClosePrice") = 336.32;
    
    // 访问数据
    std::cout << "AAPL ClosePrice on 2023-01-01: " 
              << marketData.at("AAPL", "2023-01-01", "ClosePrice") << std::endl;
    
    // 使用索引访问
    size_t aaplIdx = marketData.getInstrumentIndex("AAPL");
    size_t t1Idx = marketData.getTimeIndex("2023-01-01");
    size_t closeIdx = marketData.getFeatureIndex("ClosePrice");
    
    std::cout << "AAPL ClosePrice (via indices): " 
              << marketData.at(aaplIdx, t1Idx, closeIdx) << std::endl;
    
    // 计算特征平均值
    std::cout << "Average ClosePrice: " 
              << marketData.featureMean("ClosePrice") << std::endl;
    
}

int main() {
    Test_MultiDimData();
    return 0;
}