#include <iostream>
#include <windows.h>
#include <cstdlib>

void DisplayBatteryInfo() {
    SYSTEM_POWER_STATUS sps;
    if (GetSystemPowerStatus(&sps)) {
        std::cout << "Power source type: " << (sps.ACLineStatus ? "AC" : "Battery") << std::endl;
        std::cout << "Battery charge level: " << (int)sps.BatteryLifePercent << "%" << std::endl;
        std::cout << "Battery runtime: " << sps.BatteryLifeTime << " seconds" << std::endl;
    } else {
        std::cerr << "Failed to retrieve battery information." << std::endl;
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    while (true) {
        system("cls");
        DisplayBatteryInfo();
        Sleep(1000);
    }
    //DisplayBatteryInfo();
    return 0;
}
