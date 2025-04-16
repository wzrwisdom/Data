#include <iostream>
#include <ta_libc.h>

int main() {
    TA_RetCode retCode = TA_Initialize();
    if (retCode != TA_SUCCESS) {
        std::cerr << "TA-Lib initialization failed." << std::endl;
        return 1;
    }

    const int dataSize = 5;
    double in[dataSize] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double out[dataSize] = {0};
    int outBeg = 0, outNum = 0;

    TA_SMA(0, dataSize - 1, in, 3, &outBeg, &outNum, out);

    std::cout << "SMA: ";
    for (int i = 0; i < outNum; ++i) {
        std::cout << out[i] << " ";
    }
    std::cout << std::endl;

    TA_Shutdown();
    return 0;
}
