#include <basic_io.h>
#include <cstring>
#include <unistd.h>
#include <simple_err.h>
#include <fcntl.h>

uint64_t block_t::modify(const char * buffer, uint64_t length, uint64_t start)
{
    uint64_t modify_len;
    if (start > BLOCK_SIZE)
    {
        return 0;
    }
    else if ((start + length) > BLOCK_SIZE)
    {
        modify_len = BLOCK_SIZE - start;
    }
    else
    {
        modify_len = length;
    }

    memcpy(data + start, buffer, modify_len);

    flush();

    return modify_len;
}

uint64_t block_t::copy(char * buffer, uint64_t length, uint64_t start)
{
    refresh();

    uint64_t copy_len;
    if (start > BLOCK_SIZE)
    {
        return 0;
    }
    else if ((start + length) > BLOCK_SIZE)
    {
        copy_len = BLOCK_SIZE - start;
    }
    else
    {
        copy_len = length;
    }

    memcpy(buffer, data + start, copy_len);

    return copy_len;
}

void block_t::flush()
{
    lseek(fd, block_num * BLOCK_SIZE, SEEK_SET);
    auto len = write(fd, data, BLOCK_SIZE);
    if (len != BLOCK_SIZE)
    {
        throw simple_error_t(SHORT_WRITE_CDX,
                             std::string("Short write on block ") + std::to_string(block_num));
    }
}

void block_t::refresh()
{
    lseek(fd, block_num * BLOCK_SIZE, SEEK_SET);
    auto rd = read(fd, data, BLOCK_SIZE);
    if (rd != BLOCK_SIZE)
    {
        throw simple_error_t(SHORT_READ_CDX,
                             std::string("Short read on block ") + std::to_string(block_num));
    }
}

void io_on_dev::open(const char *pathname)
{
    fd = ::open(pathname, O_RDWR);
    if (fd == -1)
    {
        throw simple_error_t(CANNOT_OPEN_FILE_CDX);
    }

    block_count = lseek(fd, 0, SEEK_END) / BLOCK_SIZE;
}

block_t io_on_dev::request(off64_t blk_num)
{
    return block_t(fd, blk_num);
}
