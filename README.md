# ByteDriverLoader

An example project for loading a kernel driver from bytes in memory.

Convert your `.sys` driver file to bytes using the `driver_to_bytes` tool, then load it directly from memory without writing to disk.

## Basic Tutorial

### Step 1: Convert Driver to Bytes

Run the `driver_to_bytes` tool to convert your `.sys` file to a C header:

```
driver_to_bytes.exe your_driver.sys driver.h
```

### Step 2: Place driver.h

Copy the generated `driver.h` file into the `loader1` directory.

### Step 3: Build and Run

Build the solution in Visual Studio and run `ByteDriverLoader.exe` as administrator. The driver will be loaded from memory.
