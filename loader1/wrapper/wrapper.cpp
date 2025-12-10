#include "wrapper.hpp"
#include "include/kdmapper.hpp"
#include "include/utils.hpp"
#include "include/intel_driver.hpp"

LONG WINAPI SimplestCrashHandler(EXCEPTION_POINTERS* ExceptionInfo)
{
    if (ExceptionInfo && ExceptionInfo->ExceptionRecord)
        kdmLog(L"[!!] Crash at addr 0x" << ExceptionInfo->ExceptionRecord->ExceptionAddress << L" by 0x" << std::hex << ExceptionInfo->ExceptionRecord->ExceptionCode << std::endl);
    else
        kdmLog(L"[!!] Crash" << std::endl);

    if (intel_driver::hDevice)
        intel_driver::Unload();

    return EXCEPTION_EXECUTE_HANDLER;
}

bool SimpleMapper::MapDriver(const unsigned char* driver_bytes, size_t driver_size)
{
    return MapDriver(driver_bytes, driver_size, false);
}

bool SimpleMapper::MapDriver(const unsigned char* driver_bytes, size_t driver_size, bool free_memory)
{
    // Set up crash handler
    SetUnhandledExceptionFilter(SimplestCrashHandler);

    // Validate input
    if (!driver_bytes || driver_size == 0)
    {
        kdmLog(L"[-] Invalid driver bytes" << std::endl);
        return false;
    }

    kdmLog(L"[+] Mapping driver (" << driver_size << L" bytes)..." << std::endl);

    // Load vulnerable driver
    if (!NT_SUCCESS(intel_driver::Load()))
    {
        kdmLog(L"[-] Failed to load vulnerable driver" << std::endl);
        return false;
    }

    // Map the driver
    NTSTATUS exitCode = 0;
    ULONG64 mappedBase = kdmapper::MapDriver(
        const_cast<BYTE*>(driver_bytes),
        0,  // param1
        0,  // param2
        free_memory,  // free memory after usage
        true,   // destroy header
        kdmapper::AllocationMode::AllocatePool,  // allocation mode
        false,  // pass allocation address as first param
        nullptr,  // callback
        &exitCode
    );

    if (!mappedBase)
    {
        kdmLog(L"[-] Failed to map driver" << std::endl);
        intel_driver::Unload();
        return false;
    }

    kdmLog(L"[+] Driver mapped successfully at 0x" << std::hex << mappedBase << std::endl);
    kdmLog(L"[+] DriverEntry returned: 0x" << std::hex << exitCode << std::endl);

    // Unload vulnerable driver
    if (!NT_SUCCESS(intel_driver::Unload()))
    {
        kdmLog(L"[-] Warning: Failed to fully unload vulnerable driver" << std::endl);
    }

    kdmLog(L"[+] Success!" << std::endl);
    return true;
}

