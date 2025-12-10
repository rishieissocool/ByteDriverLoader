#pragma once

// Simple wrapper for kdmapper
// Just include this header and use SimpleMapper::MapDriver()
// No need to include any kdmapper headers - everything is handled internally

namespace SimpleMapper
{
    // Maps a driver from byte array
    // Returns true on success, false on failure
    // All kdmapper complexity is hidden - just pass your driver bytes!
    bool MapDriver(const unsigned char* driver_bytes, size_t driver_size);
    
    // Maps a driver from byte array with custom options
    // free_memory: Free memory after driver entry returns
    // Returns true on success, false on failure
    bool MapDriver(const unsigned char* driver_bytes, size_t driver_size, bool free_memory);
}

