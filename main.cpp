#include <iostream>
#include <windows.h>

class CLS_CChargingMode {
};

typedef CLS_CChargingMode (*FUNCTYPE_CChargingMode)(CLS_CChargingMode *);

typedef int (*FUNCTYPE_GetChargingMode)(CLS_CChargingMode *);

typedef int (*FUNCTYPE_SetChargingMode)(CLS_CChargingMode *, int);

typedef int (*FUNCTYPE_SetChargingMode2)(CLS_CChargingMode *, int, bool);

const char *MODE_CONSERVATIVE = "Conservative";
const char *MODE_NORMAL = "Normal";
const char *MODE_RAPID = "Rapid";

FUNCTYPE_GetChargingMode d_getChargingMode;
FUNCTYPE_CChargingMode d_cChargingMode;
FUNCTYPE_SetChargingMode d_setChargingMode;
FUNCTYPE_SetChargingMode2 d_setChargingModeFallback;

int getChargingMode();

int setChargingMode(int mode);

const char *chargingModeToString(int mode);

bool verifySetChargingModeResult(int mode);

bool initializeDll();

const char *chargingModeToString(int mode) {
    switch (mode) {
        case 0:
            return MODE_NORMAL;
        case 1:
            return MODE_CONSERVATIVE;
        case 2:
            return MODE_RAPID;
        default:
            return "Unknown";
    }
}

int getChargingMode() {
    auto *instance = new CLS_CChargingMode();
    *instance = d_cChargingMode(instance);
    int ret = d_getChargingMode(instance);
    free(instance);
    return ret;
}

int setChargingMode(int mode) {
    auto *instance = new CLS_CChargingMode();
    *instance = d_cChargingMode(instance);
    int ret = d_setChargingMode(instance, mode);
    if (ret != 0) {//fallback
        ret = d_setChargingModeFallback(instance, mode, false);
    }
    free(instance);
    return ret;
}

bool verifySetChargingModeResult(int mode) {
    int currentMode = getChargingMode();
    if (currentMode != mode) {
        std::cout << "Error: setChargingMode failed" << std::endl;
        std::cout << "Expected: " << chargingModeToString(mode) << std::endl;
        std::cout << "Actual: " << chargingModeToString(currentMode) << std::endl;
        return false;
    }
    return true;
}

bool initializeDll() {
    //check if dll is in the same directory as the executable
    std::string dllPath = "PowerBattery.dll";
    if (GetFileAttributes(dllPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        std::cout << "Error: PowerBattery.dll not found" << std::endl;
        return false;
    }

    HINSTANCE hDLL = LoadLibrary(dllPath.c_str());
    if (hDLL == nullptr) {
        std::cout << "Error: LoadLibrary" << std::endl;
        return false;
    }

    d_cChargingMode = (FUNCTYPE_CChargingMode) GetProcAddress
            (hDLL,
             "??0CChargingMode@PowerBattery@@QEAA@XZ");     // constructor
    if (d_cChargingMode == nullptr) {
        std::cout << "Error: GetProcAddress d_cChargingMode" << std::endl;
        return false;
    }

    d_getChargingMode = (FUNCTYPE_GetChargingMode) GetProcAddress
            (hDLL,
             "?GetChargingMode@CChargingMode@PowerBattery@@QEBAHXZ");
    if (d_getChargingMode == NULL) {
        std::cout << "Error: GetProcAddress d_getChargingMode" << std::endl;
        return false;
    }

    d_setChargingMode = (FUNCTYPE_SetChargingMode) GetProcAddress
            (hDLL,
             "?SetChargingMode@CChargingMode@PowerBattery@@QEBAHH@Z");
    if (d_setChargingMode == nullptr) {
        std::cout << "Error: GetProcAddress d_setChargingMode" << std::endl;
        return false;
    }

    d_setChargingModeFallback = (FUNCTYPE_SetChargingMode2) GetProcAddress
            (hDLL,
             "?SetChargingMode@CChargingMode@PowerBattery@@QEBAHH@Z");
    if (d_setChargingModeFallback == nullptr) {
        std::cout << "Error: GetProcAddress d_setChargingModeFallback" << std::endl;
        return false;
    }

    return true;
}


int main(int argc, char **argv) {
    if (!initializeDll()) {
        std::cout << "Error: initializeDll" << std::endl;
        return 1;
    }

    if (argc < 2) {
        std::cout << "Error: No argument provided" << std::endl;
        std::cout << "Usage: " << argv[0] << " <operation> [mode]" << std::endl;
        std::cout << "Supported operations: " << "get_charging_status, set_charging_status" << std::endl;
        std::cout << "Supported modes: " << "c(conservative), n(normal), r(rapid)" << std::endl;
        return 1;
    }

    if (strcmp(argv[1], "get_charging_status") == 0) {
        std::cout << "Current charging mode: " << chargingModeToString(getChargingMode()) << std::endl;
        return 0;
    }

    if (strcmp(argv[1], "set_charging_status") == 0) {
        if (argc < 3) {
            std::cout << "Error: No mode provided" << std::endl;
            return 1;
        }
        int mode;
        switch (argv[2][0]) {
            case 'c':
                mode = 1;
                break;
            case 'n':
                mode = 0;
                break;
            case 'r':
                mode = 2;
                break;
            default:
                std::cout << "Error: Unknown mode" << std::endl;
                return 1;
        }
        setChargingMode(mode);
        Sleep(1000);//wait for the mode to be set
        bool ret = verifySetChargingModeResult(mode);
        if (!ret) {
            std::cout << "Error: setChargingMode" << std::endl;
            return 1;
        }
        std::cout << "Charging mode set to: " << chargingModeToString(getChargingMode()) << std::endl;
        return 0;
    }
    std::cout << "Error: Unknown operation" << std::endl;
    return 1;
}
