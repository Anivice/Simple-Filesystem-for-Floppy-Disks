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

    uint64_t modify(const char * buffer, uint64_t length, uint64_t start);
    uint64_t copy(char * buffer, uint64_t length, uint64_t start);

    block_t & operator = (const block_t &) = delete;
};

#endif //FLOPPYFS_BASIC_IO_H