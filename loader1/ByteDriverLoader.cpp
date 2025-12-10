#include "driver.h"
#include "wrapper/wrapper.hpp"

int main()
{

    if (driver_size == 0 || driver_bytes == nullptr)
    {
        return -1;
    }

    if (!SimpleMapper::MapDriver(driver_bytes, driver_size))
    {
        return -1;
    }

    return 0;
}
