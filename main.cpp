#include <iostream>
#include <windows.h>
#include <powrprof.h>
#include <chrono>
#include <thread>

#pragma comment(lib, "PowrProf.lib")

void printPowerInfo() {
    SYSTEM_POWER_STATUS sps;
    GetSystemPowerStatus(&sps);

    std::cout << "AC Line Status: ";
    if (sps.ACLineStatus == 0) {
        std::cout << "Battery";
    } else {
        std::cout << "AC Power";
    }
    std::cout << std::endl;

    std::cout << "Battery type: ";
    switch (sps.BatteryFlag & 0xF) {
        case 0x1: std::cout << "Lithium-ion"; break;
        case 0x2: std::cout << "Nickel-metal hydride"; break;
        case 0x4: std::cout << "Lead-acid"; break;
        case 0x8: std::cout << "Unknown"; break;
        default: std::cout << "Unknown";
    }
    std::cout << std::endl;

    std::cout << "Battery charge level: " << (int)sps.BatteryLifePercent << "%" << std::endl;

    std::cout << "Power saving mode: " << (sps.SystemStatusFlag ? "Enabled" : "Disabled") << std::endl;
}

void goToSleep() {
    SetSuspendState(FALSE, TRUE, FALSE);
}

void goToHibernate() {
    SetSuspendState(TRUE, TRUE, FALSE);
}

void printBatteryRuntime(std::chrono::steady_clock::time_point start) {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start);

    int hours = duration.count() / 3600;
    int minutes = (duration.count() % 3600) / 60;
    int seconds = duration.count() % 60;

    std::cout << "Battery runtime since unplugged: "
              << hours << " hours "
              << minutes << " minutes "
              << seconds << " seconds" << std::endl;
}

void printRemainingBatteryLife() {
    SYSTEM_POWER_STATUS sps;
    GetSystemPowerStatus(&sps);

    if (sps.ACLineStatus == 0 && sps.BatteryLifeTime != -1) {
        int hours = sps.BatteryLifeTime / 3600;
        int minutes = (sps.BatteryLifeTime % 3600) / 60;
        int seconds = sps.BatteryLifeTime % 60;
        std::cout << "Remaining battery runtime after unplugging: ";
        std::cout << hours << " hours " << minutes << " minutes " << seconds << " seconds" << std::endl;
    } else {
        std::cout << "Computer is connected to AC power or time is not available." << std::endl;
    }
}

int main() {
    std::chrono::steady_clock::time_point unpluggedTime;
    bool isUnplugged = false;

    while (true) {
        system("cls");
        std::cout << "-----------------" << std::endl;
        printPowerInfo();

        SYSTEM_POWER_STATUS sps;
        GetSystemPowerStatus(&sps);

        if (sps.ACLineStatus == 0) {
            if (!isUnplugged) {
                unpluggedTime = std::chrono::steady_clock::now();
                isUnplugged = true;
            }
            printBatteryRuntime(unpluggedTime);
            printRemainingBatteryLife();
        } else {
            isUnplugged = false;
            std::cout << "Connected to AC power." << std::endl;
        }

        // std::cout << "\nChoose an action:" << std::endl;
        // std::cout << "1. Go to sleep mode" << std::endl;
        // std::cout << "2. Go to hibernate mode" << std::endl;
        // std::cout << "3. Exit the program" << std::endl;
        //
        // int choice;
        // std::cin >> choice;
        //
        // switch (choice) {
        //     case 1:
        //         goToSleep();
        //         break;
        //     case 2:
        //         goToHibernate();
        //         break;
        //     case 3:
        //         return 0;
        //     default:
        //         std::cout << "Invalid choice. Press Enter to continue.";
        //         std::cin.ignore();
        //         std::cin.get();
        // }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}