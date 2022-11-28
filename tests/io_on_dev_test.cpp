#include <basic_io.h>
#include <debug.h>
#include <simple_err.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main(int, char ** argv)
{
    try
    {
        char buff[512];

        initialize_symbol_vector(argv[0]);
        exec_cmd("dd if=/dev/urandom of=./disk.img bs=1M count=1 2>/dev/null");
        io_on_dev device;
        device.open("disk.img");

        auto block = device.request(8);
        auto block2 = device.request(8);

        block.modify("ABCDEFG", 7, 0);
        block2.copy(buff, 7, 0);

        std::cout << buff << std::endl;
    }
    catch (simple_error_t & err)
    {
        std::cout << err.what() << std::endl;
    }
}
