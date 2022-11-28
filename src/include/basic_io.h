#ifndef FLOPPYFS_BASIC_IO_H
#define FLOPPYFS_BASIC_IO_H

#include <cstdint>
#include <vector>
#include <sys/types.h>
#include <string>
#define BLOCK_SIZE 512

class block_t
{
private:
    unsigned char data [BLOCK_SIZE] { };
    uint32_t block_num = 0;
    int fd = 0;

    void flush();
    void refresh();
public:
    // assign block info
    explicit block_t(int _fd, off64_t _block_num)
    {
        fd = _fd;
        block_num = _block_num;
        refresh();
    }

    uint32_t modify(const char * buffer, uint32_t length, uint32_t start);
    uint32_t copy(char * buffer, uint32_t length, uint32_t start);

    block_t & operator = (const block_t &) = delete;
};


class io_on_dev
{
private:
    int fd = 0;
    off64_t block_count = 0;
public:
    // open device
    void open(const char * pathname);

    // get current device block count
    [[nodiscard]] off64_t get_block_count() const { return block_count; }

    // request a block
    block_t request(off64_t blk_num);
};

#endif //FLOPPYFS_BASIC_IO_H
