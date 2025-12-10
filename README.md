# ByteDriverLoader

A simplified, secure wrapper for kdmapper that allows loading kernel drivers directly from memory byte arrays without dropping files to disk.

## 🔒 Security Benefits

**No Disk I/O** - The primary security advantage of this loader is that it eliminates the need to drop `.sys` driver files to disk. Traditional loaders require writing the driver file to the filesystem, which:

- ❌ Leaves forensic artifacts on disk
- ❌ Can be detected by anti-cheat systems scanning for driver files
- ❌ Creates file system traces that can be logged and monitored
- ❌ Requires file permissions and disk access

**Memory-Only Loading** - This loader operates entirely in memory:

- ✅ Driver bytes are embedded directly in the executable
- ✅ No temporary files are created on disk
- ✅ No filesystem traces left behind
- ✅ Reduced detection surface for anti-cheat systems
- ✅ Cleaner execution environment

## 🎯 Use Case

This project is designed for **game cheat developers** and **security researchers** who need to load kernel drivers without leaving disk artifacts. By embedding driver bytes directly in the loader executable, you can:

- Avoid detection mechanisms that monitor file system activity
- Reduce forensic evidence of driver loading
- Create more stealthy loader implementations
- Simplify deployment (single executable, no driver files)

## ✨ Features

- **Simple API** - One function call to map your driver
- **Memory-Only** - No disk I/O required
- **Self-Contained** - All kdmapper functionality bundled
- **Clean Structure** - Organized codebase with wrapper abstraction
- **Easy Integration** - Just include `wrapper.hpp` and use

## 📁 Project Structure

```
ByteDriverLoader/
├── ByteDriverLoader/
│   ├── driver.h              # Your driver bytes (generated)
│   ├── ByteDriverLoader.cpp  # Main entry point
│   └── wrapper/              # All wrapper/kdmapper code
│       ├── wrapper.hpp       # Simple API interface
│       ├── wrapper.cpp       # Implementation
│       ├── include/          # kdmapper headers
│       └── src/              # kdmapper source files
└── driver_to_bytes/
    └── driver_to_bytes.cpp   # Tool to convert .sys to .h
```

## 🚀 Quick Start

### 1. Convert Your Driver to Bytes

Use the included `driver_to_bytes` tool to convert your `.sys` file to a C header:

```bash
driver_to_bytes.exe your_driver.sys driver.h
```

This generates a `driver.h` file with your driver bytes embedded as a C array.

### 2. Copy driver.h

Place the generated `driver.h` in the `ByteDriverLoader/ByteDriverLoader/` directory.

### 3. Build the Loader

Open `ByteDriverLoader.slnx` in Visual Studio and build the solution. The output will be in:
- `bin/x64/Release/ByteDriverLoader.exe` (or Debug)

### 4. Use the Simple API

```cpp
#include "driver.h"
#include "wrapper/wrapper.hpp"

int main()
{
    // Validate driver bytes
    if (driver_size == 0 || driver_bytes == nullptr)
        return -1;

    // Map the driver - that's it!
    if (!SimpleMapper::MapDriver(driver_bytes, driver_size))
        return -1;

    return 0;
}
```

## 📖 API Reference

### `SimpleMapper::MapDriver()`

Maps a driver from a byte array into kernel memory.

```cpp
// Basic usage
bool MapDriver(const unsigned char* driver_bytes, size_t driver_size);

// With memory cleanup option
bool MapDriver(const unsigned char* driver_bytes, size_t driver_size, bool free_memory);
```

**Parameters:**
- `driver_bytes` - Pointer to the driver byte array
- `driver_size` - Size of the driver in bytes
- `free_memory` (optional) - If `true`, frees kernel memory after driver entry returns

**Returns:**
- `true` on success
- `false` on failure

**Example:**
```cpp
// Load driver and keep memory allocated
SimpleMapper::MapDriver(driver_bytes, driver_size);

// Load driver and free memory after entry returns
SimpleMapper::MapDriver(driver_bytes, driver_size, true);
```

## 🛠️ Building

### Requirements

- Visual Studio 2019 or later
- Windows SDK 10.0
- Administrator privileges (for running the loader)

### Build Steps

1. Open `ByteDriverLoader.slnx` in Visual Studio
2. Select your desired configuration (Debug/Release) and platform (x64)
3. Build the solution (F7 or Build → Build Solution)
4. Executables will be in `bin/<Platform>/<Configuration>/`

### Output Structure

```
bin/
├── x64/
│   ├── Debug/
│   │   ├── ByteDriverLoader.exe
│   │   └── driver_to_bytes.exe
│   └── Release/
│       ├── ByteDriverLoader.exe
│       └── driver_to_bytes.exe
└── Win32/
    └── ...
```

## 🔐 Security Considerations

### Advantages

- **No Disk Artifacts**: Driver never touches the filesystem
- **Reduced Detection Surface**: Anti-cheat systems can't scan for driver files
- **Forensic Stealth**: No file creation events logged
- **Single Executable**: Easier to deploy and manage

### Limitations

- Still uses vulnerable driver (Intel driver) for kernel access
- Driver bytes are visible in executable (can be extracted)
- Memory scanning can still detect loaded drivers
- Requires administrator privileges

### Best Practices

1. **Obfuscate driver bytes** in `driver.h` if needed
2. **Use code signing** for the loader executable
3. **Implement anti-debugging** measures in your loader
4. **Clean up traces** after driver loading
5. **Monitor for detection** of the vulnerable driver

## 📝 How It Works

1. **Driver Embedding**: Your `.sys` file is converted to a C byte array in `driver.h`
2. **Vulnerable Driver**: Loads Intel's vulnerable driver (iqvw64e.sys) for kernel access
3. **Memory Mapping**: Maps your driver bytes directly into kernel memory
4. **Import Resolution**: Resolves kernel imports automatically
5. **Relocation**: Handles address relocations
6. **Entry Point**: Calls your driver's entry point
7. **Cleanup**: Unloads vulnerable driver and cleans up

## 🧩 Integration

The wrapper is completely self-contained. You only need to:

1. Include `wrapper/wrapper.hpp` in your project
2. Link against the wrapper source files
3. Call `SimpleMapper::MapDriver()` with your bytes

No need to understand kdmapper internals or include kdmapper headers directly.

## ⚠️ Disclaimer

This project is for **educational and research purposes only**. The authors are not responsible for any misuse of this software. Loading unsigned kernel drivers may:

- Violate terms of service of games/software
- Be illegal in your jurisdiction
- Result in account bans or legal action

Use at your own risk.

## 📄 License

This project includes kdmapper, which has its own license. Please refer to the original kdmapper repository for licensing information.

## 🙏 Credits

- Built on top of [kdmapper](https://github.com/TheCruZ/kdmapper)
- Simplified wrapper implementation
- Focus on security through memory-only loading

## 📧 Support

For issues, questions, or contributions, please open an issue on the repository.

---

**Remember**: The goal is to load drivers securely without disk artifacts. This loader provides that capability while maintaining a simple, easy-to-use API.

