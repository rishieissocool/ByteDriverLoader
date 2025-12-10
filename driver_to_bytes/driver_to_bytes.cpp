#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <filesystem>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: driver_to_bytes.exe <driver.sys> [output.h]" << std::endl;
        std::cout << "       If output file is not specified, it will be generated as driver.h" << std::endl;
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file;

    if (argc >= 3)
    {
        output_file = argv[2];
    }
    else
    {
        // Generate output filename from input
        std::filesystem::path input_path(input_file);
        output_file = input_path.replace_extension(".h").string();
    }

    // Read the driver file
    std::ifstream file(input_file, std::ios::binary);
    if (!file)
    {
        std::cout << "[-] Failed to open input file: " << input_file << std::endl;
        return 1;
    }

    // Read all bytes
    std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    if (bytes.empty())
    {
        std::cout << "[-] Input file is empty" << std::endl;
        return 1;
    }

    // Write to output file
    std::ofstream out(output_file);
    if (!out)
    {
        std::cout << "[-] Failed to create output file: " << output_file << std::endl;
        return 1;
    }

    // Write header
    out << "#pragma once\n\n";
    out << "// Driver bytes array\n";
    out << "// Generated from: " << input_file << "\n";
    out << "// Size: " << bytes.size() << " bytes\n\n";
    out << "const unsigned char driver_bytes[] = {\n";

    // Write bytes in hex format
    for (size_t i = 0; i < bytes.size(); ++i)
    {
        if (i % 16 == 0)
        {
            if (i > 0)
                out << "\n";
            out << "    ";
        }

        out << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(bytes[i]);

        if (i < bytes.size() - 1)
            out << ",";

        if (i % 16 != 15 && i < bytes.size() - 1)
            out << " ";
    }

    out << "\n};\n\n";
    out << "const size_t driver_size = sizeof(driver_bytes);\n";

    out.close();

    std::cout << "[+] Successfully converted " << bytes.size() << " bytes to " << output_file << std::endl;
    return 0;
}

