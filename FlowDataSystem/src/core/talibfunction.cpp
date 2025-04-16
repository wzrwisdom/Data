#include "core/talibfunction.hpp"



std::vector<double> computeEMA(const std::vector<double>& input, int timeperiod) {
    size_t size = input.size();
    std::vector<double> out(size);
    int outBeg = 0, outNbElement = 0;

    TA_RetCode retCode = TA_EMA(
        0,                      // 起始索引
        static_cast<int>(size - 1), // 结束索引
        input.data(),           // 输入数据
        timeperiod,             // 时间周期
        &outBeg,                // 输出的起始索引
        &outNbElement,          // 输出的元素数量
        out.data()              // 输出数据
    );

    if (retCode != TA_SUCCESS) {
        std::cerr << "Error: TA_EMA failed with error code " << retCode << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }

    // 填充未使用的部分为 NaN
    for (size_t i = 0; i < static_cast<size_t>(outBeg); ++i) {
        out[i] = std::numeric_limits<double>::quiet_NaN();
    }

    return out;
}


std::vector<double> computeDEMA(const std::vector<double>& input, int timeperiod) {
    size_t size = input.size();
    std::vector<double> out(size);
    int outBeg = 0, outNbElement = 0;

    TA_RetCode retCode = TA_DEMA(
        0,                      // 起始索引
        static_cast<int>(size - 1), // 结束索引
        input.data(),           // 输入数据
        timeperiod,             // 时间周期
        &outBeg,                // 输出的起始索引
        &outNbElement,          // 输出的元素数量
        out.data()              // 输出数据
    );

    if (retCode != TA_SUCCESS) {
        std::cerr << "Error: TA_DEMA failed with error code " << retCode << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }

    // 填充未使用的部分为 NaN
    for (size_t i = 0; i < static_cast<size_t>(outBeg); ++i) {
        out[i] = std::numeric_limits<double>::quiet_NaN();
    }

    return out;
}

std::vector<double> computeSMA(const std::vector<double>& input, int timeperiod) {
    size_t size = input.size();
    std::vector<double> out(size); // 输出向量
    int outBeg = 0, outNbElement = 0;

    // 调用 TA-Lib 的 TA_SMA 函数
    TA_RetCode retCode = TA_SMA(
        0,                      // 起始索引
        static_cast<int>(size - 1), // 结束索引
        input.data(),           // 输入数据
        timeperiod,             // 时间周期
        &outBeg,                // 输出的起始索引
        &outNbElement,          // 输出的元素数量
        out.data()              // 输出数据
    );

    if (retCode != TA_SUCCESS) {
        std::cerr << "Error: TA_SMA failed with error code " << retCode << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }

    // 填充未使用的部分为 NaN
    for (size_t i = 0; i < static_cast<size_t>(outBeg); ++i) {
        out[i] = std::numeric_limits<double>::quiet_NaN();
    }

    return out;
}


std::vector<double> computeAROONDown(const std::vector<double>& high, const std::vector<double>& low, int timeperiod) {
    size_t size = high.size();
    if (size != low.size()) {
        std::cerr << "Error: High and Low vectors must have the same size." << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }
    std::vector<double> aroonUp(size);   // AROON Up 输出向量
    std::vector<double> aroonDown(size); // AROON Down 输出向量
    int outBeg = 0, outNbElement = 0;

    // 调用 TA-Lib 的 TA_AROON 函数
    TA_RetCode retCode = TA_AROON(
        0,                      // 起始索引
        static_cast<int>(size - 1), // 结束索引
        high.data(),            // High 数据
        low.data(),             // Low 数据
        timeperiod,             // 时间周期
        &outBeg,                // 输出的起始索引
        &outNbElement,          // 输出的元素数量
        aroonDown.data(),       // AROON Down 输出数据
        aroonUp.data()          // AROON Up 输出数据
    );

    if (retCode != TA_SUCCESS) {
        std::cerr << "Error: TA_AROON failed with error code " << retCode << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }

    // 填充未使用的部分为 NaN
    for (size_t i = 0; i < static_cast<size_t>(outBeg); ++i) {
        aroonUp[i] = std::numeric_limits<double>::quiet_NaN();
        aroonDown[i] = std::numeric_limits<double>::quiet_NaN();
    }


    return aroonDown;
}

std::vector<double> computeAROONOSC(const std::vector<double>& high, const std::vector<double>& low, int timeperiod) {
    size_t size = high.size();
    if (size != low.size()) {
        std::cerr << "Error: High and Low vectors must have the same size." << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }
    std::vector<double> aroonOsc(size); // AROON Oscillator 输出向量
    int outBeg = 0, outNbElement = 0;

    // 调用 TA-Lib 的 TA_AROONOSC 函数
    TA_RetCode retCode = TA_AROONOSC(
        0,                      // 起始索引
        static_cast<int>(size - 1), // 结束索引
        high.data(),            // High 数据
        low.data(),             // Low 数据
        timeperiod,             // 时间周期
        &outBeg,                // 输出的起始索引
        &outNbElement,          // 输出的元素数量
        aroonOsc.data()         // AROON Oscillator 输出数据
    );

    if (retCode != TA_SUCCESS) {
        std::cerr << "Error: TA_AROONOSC failed with error code " << retCode << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }

    // 填充未使用的部分为 NaN
    for (size_t i = 0; i < static_cast<size_t>(outBeg); ++i) {
        aroonOsc[i] = std::numeric_limits<double>::quiet_NaN();
    }

    return aroonOsc;
}

std::vector<double> computeSTOCHRSIFASTD(const std::vector<double>& input, int timeperiod , int fastk_period, int fastd_period, TA_MAType fastd_matype) {
    size_t size = input.size();
    std::vector<double> fastk(size); // STOCHRSI FastK 输出向量
    std::vector<double> fastd(size); // STOCHRSI FastD 输出向量
    int outBeg = 0, outNbElement = 0;

    // 调用 TA-Lib 的 TA_STOCHRSI 函数
    TA_RetCode retCode = TA_STOCHRSI(
        0,                      // 起始索引
        static_cast<int>(size - 1), // 结束索引
        input.data(),           // 输入数据
        timeperiod,             // RSI 的时间周期
        fastk_period,           // FastK 的时间周期
        fastd_period,           // FastD 的时间周期
        fastd_matype,           // FastD 的移动平均类型
        &outBeg,                // 输出的起始索引
        &outNbElement,          // 输出的元素数量
        fastk.data(),           // FastK 输出数据
        fastd.data()            // FastD 输出数据
    );

    if (retCode != TA_SUCCESS) {
        std::cerr << "Error: TA_STOCHRSI failed with error code " << retCode << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }

    // 填充未使用的部分为 NaN
    for (size_t i = 0; i < static_cast<size_t>(outBeg); ++i) {
        fastd[i] = std::numeric_limits<double>::quiet_NaN();
    }

    return fastd;
}


std::vector<double> computeSTOCHRSIFASTK(const std::vector<double>& input, int timeperiod, int fastk_period, int fastd_period, TA_MAType fastd_matype) {
    size_t size = input.size();
    std::vector<double> fastk(size); // STOCHRSI FastK 输出向量
    std::vector<double> fastd(size); // STOCHRSI FastD 输出向量
    int outBeg = 0, outNbElement = 0;

    // 调用 TA-Lib 的 TA_STOCHRSI 函数
    TA_RetCode retCode = TA_STOCHRSI(
        0,                      // 起始索引
        static_cast<int>(size - 1), // 结束索引
        input.data(),           // 输入数据
        timeperiod,             // RSI 的时间周期
        fastk_period,           // FastK 的时间周期
        fastd_period,           // FastD 的时间周期
        fastd_matype,           // FastD 的移动平均类型
        &outBeg,                // 输出的起始索引
        &outNbElement,          // 输出的元素数量
        fastk.data(),           // FastK 输出数据
        fastd.data()            // FastD 输出数据
    );

    if (retCode != TA_SUCCESS) {
        std::cerr << "Error: TA_STOCHRSI failed with error code " << retCode << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }

    // 填充未使用的部分为 NaN
    for (size_t i = 0; i < static_cast<size_t>(outBeg); ++i) {
        fastk[i] = std::numeric_limits<double>::quiet_NaN();
    }

    return fastk;
}

std::vector<double> computeMACD(const std::vector<double>& input, int fastPeriod, int slowPeriod, int signalPeriod) {
    size_t size = input.size();
    std::vector<double> macd(size);       // MACD Line 输出向量
    std::vector<double> signal(size);    // Signal Line 输出向量
    std::vector<double> hist(size);      // Histogram 输出向量
    int outBeg = 0, outNbElement = 0;

    // 调用 TA-Lib 的 TA_MACD 函数
    TA_RetCode retCode = TA_MACD(
        0,                      // 起始索引
        static_cast<int>(size - 1), // 结束索引
        input.data(),           // 输入数据
        fastPeriod,             // 快速 EMA 周期
        slowPeriod,             // 慢速 EMA 周期
        signalPeriod,           // 信号线 EMA 周期
        &outBeg,                // 输出的起始索引
        &outNbElement,          // 输出的元素数量
        macd.data(),            // MACD Line 输出数据
        signal.data(),          // Signal Line 输出数据
        hist.data()             // Histogram 输出数据
    );

    if (retCode != TA_SUCCESS) {
        std::cerr << "Error: TA_MACD failed with error code " << retCode << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }

    // 填充未使用的部分为 NaN
    for (size_t i = 0; i < static_cast<size_t>(outBeg); ++i) {
        macd[i] = std::numeric_limits<double>::quiet_NaN();
        signal[i] = std::numeric_limits<double>::quiet_NaN();
        hist[i] = std::numeric_limits<double>::quiet_NaN();
    }

    return macd;
}


std::vector<double> computeWILLR(const std::vector<double>& high, const std::vector<double>& low, const std::vector<double>& close, int timeperiod) {
    size_t size = high.size();
    if (size != low.size() || size != close.size()) {
        std::cerr << "Error: High, Low, and Close vectors must have the same size." << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }

    std::vector<double> willr(size); // 输出向量
    int outBeg = 0, outNbElement = 0;

    // 调用 TA-Lib 的 TA_WILLR 函数
    TA_RetCode retCode = TA_WILLR(
        0,                      // 起始索引
        static_cast<int>(size - 1), // 结束索引
        high.data(),            // High 数据
        low.data(),             // Low 数据
        close.data(),           // Close 数据
        timeperiod,             // 时间周期
        &outBeg,                // 输出的起始索引
        &outNbElement,          // 输出的元素数量
        willr.data()            // 输出数据
    );

    if (retCode != TA_SUCCESS) {
        std::cerr << "Error: TA_WILLR failed with error code " << retCode << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }

    // 填充未使用的部分为 NaN
    for (size_t i = 0; i < static_cast<size_t>(outBeg); ++i) {
        willr[i] = std::numeric_limits<double>::quiet_NaN();
    }

    return willr;
}

std::vector<double> computeRSI(const std::vector<double>& input, int timeperiod) {
    size_t size = input.size();
    std::vector<double> rsi(size); // 输出向量
    int outBeg = 0, outNbElement = 0;

    // 调用 TA-Lib 的 TA_RSI 函数
    TA_RetCode retCode = TA_RSI(
        0,                      // 起始索引
        static_cast<int>(size - 1), // 结束索引
        input.data(),           // 输入数据
        timeperiod,             // 时间周期
        &outBeg,                // 输出的起始索引
        &outNbElement,          // 输出的元素数量
        rsi.data()              // 输出数据
    );

    if (retCode != TA_SUCCESS) {
        std::cerr << "Error: TA_RSI failed with error code " << retCode << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }

    // 填充未使用的部分为 NaN
    for (size_t i = 0; i < static_cast<size_t>(outBeg); ++i) {
        rsi[i] = std::numeric_limits<double>::quiet_NaN();
    }

    return rsi;
}

std::vector<double> computeAPO(const std::vector<double>& input, int fastPeriod, int slowPeriod, TA_MAType matype) {
    size_t size = input.size();
    std::vector<double> apo(size); // 输出向量
    int outBeg = 0, outNbElement = 0;

    // 调用 TA-Lib 的 TA_APO 函数
    TA_RetCode retCode = TA_APO(
        0,                      // 起始索引
        static_cast<int>(size - 1), // 结束索引
        input.data(),           // 输入数据
        fastPeriod,             // 快速移动平均线的时间周期
        slowPeriod,             // 慢速移动平均线的时间周期
        matype,                 // 移动平均线类型（0 表示简单移动平均线）
        &outBeg,                // 输出的起始索引
        &outNbElement,          // 输出的元素数量
        apo.data()              // 输出数据
    );

    if (retCode != TA_SUCCESS) {
        std::cerr << "Error: TA_APO failed with error code " << retCode << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }

    // 填充未使用的部分为 NaN
    for (size_t i = 0; i < static_cast<size_t>(outBeg); ++i) {
        apo[i] = std::numeric_limits<double>::quiet_NaN();
    }

    return apo;
}

std::vector<double> computeWCLPRICE(const std::vector<double>& high, const std::vector<double>& low, const std::vector<double>& close) {
    size_t size = high.size();
    if (size != low.size() || size != close.size()) {
        std::cerr << "Error: High, Low, and Close vectors must have the same size." << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }

    std::vector<double> wclprice(size); // 输出向量
    int outBeg = 0, outNbElement = 0;

    // 调用 TA-Lib 的 TA_WCLPRICE 函数
    TA_RetCode retCode = TA_WCLPRICE(
        0,                      // 起始索引
        static_cast<int>(size - 1), // 结束索引
        high.data(),            // High 数据
        low.data(),             // Low 数据
        close.data(),           // Close 数据
        &outBeg,                // 输出的起始索引
        &outNbElement,          // 输出的元素数量
        wclprice.data()         // 输出数据
    );

    if (retCode != TA_SUCCESS) {
        std::cerr << "Error: TA_WCLPRICE failed with error code " << retCode << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }

    // 填充未使用的部分为 NaN
    for (size_t i = 0; i < static_cast<size_t>(outBeg); ++i) {
        wclprice[i] = std::numeric_limits<double>::quiet_NaN();
    }

    return wclprice;
}

std::vector<double> computeMomentum(const std::vector<double>& input, int timeperiod) {
    size_t size = input.size();
    std::vector<double> momentum(size); // 输出向量
    int outBeg = 0, outNbElement = 0;

    // 调用 TA-Lib 的 TA_MOM 函数
    TA_RetCode retCode = TA_MOM(
        0,                      // 起始索引
        static_cast<int>(size - 1), // 结束索引
        input.data(),           // 输入数据
        timeperiod,             // 时间周期
        &outBeg,                // 输出的起始索引
        &outNbElement,          // 输出的元素数量
        momentum.data()         // 输出数据
    );

    if (retCode != TA_SUCCESS) {
        std::cerr << "Error: TA_MOM failed with error code " << retCode << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }

    // 填充未使用的部分为 NaN
    for (size_t i = 0; i < static_cast<size_t>(outBeg); ++i) {
        momentum[i] = std::numeric_limits<double>::quiet_NaN();
    }

    return momentum;
}

std::vector<double> computeTRIMA(const std::vector<double>& input, int timeperiod) {
    size_t size = input.size();
    std::vector<double> trima(size); // 输出向量
    int outBeg = 0, outNbElement = 0;

    // 调用 TA-Lib 的 TA_TRIMA 函数
    TA_RetCode retCode = TA_TRIMA(
        0,                      // 起始索引
        static_cast<int>(size - 1), // 结束索引
        input.data(),           // 输入数据
        timeperiod,             // 时间周期
        &outBeg,                // 输出的起始索引
        &outNbElement,          // 输出的元素数量
        trima.data()            // 输出数据
    );

    if (retCode != TA_SUCCESS) {
        std::cerr << "Error: TA_TRIMA failed with error code " << retCode << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }

    // 填充未使用的部分为 NaN
    for (size_t i = 0; i < static_cast<size_t>(outBeg); ++i) {
        trima[i] = std::numeric_limits<double>::quiet_NaN();
    }

    return trima;
}

std::vector<double> computeATR(const std::vector<double>& high, const std::vector<double>& low, const std::vector<double>& close, int timeperiod) {
    size_t size = high.size();
    if (size != low.size() || size != close.size()) {
        std::cerr << "Error: High, Low, and Close vectors must have the same size." << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }

    std::vector<double> atr(size); // 输出向量
    int outBeg = 0, outNbElement = 0;

    // 调用 TA-Lib 的 TA_ATR 函数
    TA_RetCode retCode = TA_ATR(
        0,                      // 起始索引
        static_cast<int>(size - 1), // 结束索引
        high.data(),            // High 数据
        low.data(),             // Low 数据
        close.data(),           // Close 数据
        timeperiod,             // 时间周期
        &outBeg,                // 输出的起始索引
        &outNbElement,          // 输出的元素数量
        atr.data()              // 输出数据
    );

    if (retCode != TA_SUCCESS) {
        std::cerr << "Error: TA_ATR failed with error code " << retCode << std::endl;
        return std::vector<double>(size, std::numeric_limits<double>::quiet_NaN());
    }

    // 填充未使用的部分为 NaN
    for (size_t i = 0; i < static_cast<size_t>(outBeg); ++i) {
        atr[i] = std::numeric_limits<double>::quiet_NaN();
    }

    return atr;
}


