#ifndef TALIBFUNCTION_HPP
#define TALIBFUNCTION_HPP

#include <ta_libc.h>
#include <vector>
#include <iostream>
#include <limits>

// 函数声明
std::vector<double> computeEMA(const std::vector<double>& input, int timeperiod);
std::vector<double> computeDEMA(const std::vector<double>& input, int timeperiod);
std::vector<double> computeSMA(const std::vector<double>& input, int timeperiod);
std::vector<double> computeAROONDown(const std::vector<double>& high, const std::vector<double>& low, int timeperiod);
std::vector<double> computeAROONOSC(const std::vector<double>& high, const std::vector<double>& low, int timeperiod = 14);
std::vector<double> computeSTOCHRSIFASTD(const std::vector<double>& input, int timeperiod = 14, int fastk_period = 5, int fastd_period = 3, TA_MAType fastd_matype = TA_MAType_SMA);
std::vector<double> computeSTOCHRSIFASTK(const std::vector<double>& input, int timeperiod = 14, int fastk_period = 5, int fastd_period = 3, TA_MAType fastd_matype = TA_MAType_SMA);
std::vector<double> computeMACD(const std::vector<double>& input, int fastPeriod = 12, int slowPeriod = 26, int signalPeriod = 9);
std::vector<double> computeWILLR(const std::vector<double>& high, const std::vector<double>& low, const std::vector<double>& close, int timeperiod = 14);
std::vector<double> computeRSI(const std::vector<double>& input, int timeperiod = 14);
std::vector<double> computeAPO(const std::vector<double>& input, int fastPeriod = 12, int slowPeriod = 26, TA_MAType matype = TA_MAType_SMA);
std::vector<double> computeWCLPRICE(const std::vector<double>& high, const std::vector<double>& low, const std::vector<double>& close);
std::vector<double> computeMomentum(const std::vector<double>& input, int timeperiod = 10);
std::vector<double> computeTRIMA(const std::vector<double>& input, int timeperiod = 10);
std::vector<double> computeATR(const std::vector<double>& high, const std::vector<double>& low, const std::vector<double>& close, int timeperiod = 14);

#endif // TALIBFUNCTION_HPP