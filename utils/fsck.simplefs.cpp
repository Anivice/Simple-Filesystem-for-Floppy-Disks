#include <simplefs.h>
#include <basic_io.h>
#include <simple_err.h>
#include <iostream>

int main(int argc, char ** argv)
{
    try
    {
        simplefs_head_t head;
        io_on_dev device;
        device.open(argv[1]);
        auto block0 = device.request(0);
        block0.copy((char*)&head, BLOCK_SIZE, 0);

        if (head.content.magic != SIMPLEFS_MAGIC)
        {
            std::cerr << "Filesystem sanity check failed!" << std::endl;
            return EXIT_FAILURE;
        }

        output_head_info(head);
    }
    catch (simple_error_t & err)
    {
        std::cerr << err.what() << std::endl;
    }
}
