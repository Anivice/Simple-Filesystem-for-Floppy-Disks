#ifndef FLOPPYFS_INODE_IO_H
#define FLOPPYFS_INODE_IO_H

#include <simple_err.h>
#include <simplefs.h>
#include <basic_io.h>

class inode_io
{
private:
    io_on_dev device;
    uint32_t inode_number;
    simplefs_head_t head;

    simplefs_inode_t get_inode_head();
    void save_inode_head(simplefs_inode_t inode);

public:
    explicit inode_io(io_on_dev & _device, uint32_t _inode_number)
        : device(_device), inode_number(_inode_number)
            {
                _device.request(0).copy((char*)&head, BLOCK_SIZE, 0);
            }

    uint32_t read(char * buffer, uint32_t length, uint32_t offset);
    uint32_t write(const char * buffer, uint32_t length, uint32_t offset);
};


#endif //FLOPPYFS_INODE_IO_H
