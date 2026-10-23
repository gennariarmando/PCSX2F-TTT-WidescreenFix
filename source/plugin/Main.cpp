#include "core/Api.h"
#include "core/Plugin.h"
#include "game/TekkenAddresses.h"

extern "C" {
    int CompatibleCRCList[] = { CTekkenAddresses::DiscCrc };
    char PluginData[MaxIniSize] = { 0 };
    int PCSX2Data[PCSX2Data_Size] = { 1 };
}

extern "C" void init() {
    CPlugin::Install();
}

int main() {
    return 0;
}
