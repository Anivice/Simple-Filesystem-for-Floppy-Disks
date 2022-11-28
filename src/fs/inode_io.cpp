#include <inode_io.h>

uint32_t inode_io::read(char *buffer, uint32_t length, uint32_t offset)
{

}

uint32_t inode_io::write(const char *buffer, uint32_t length, uint32_t offset)
{

}

simplefs_inode_t inode_io::get_inode_head()
{
    simplefs_inode_t inode;
    auto block = device.request(head.content.inode_bitmap_blk_start + inode_number);
    block.copy((char*)&inode, BLOCK_SIZE, 0);

    return inode;
}

void inode_io::save_inode_head(simplefs_inode_t inode)
{
    auto block = device.request(head.content.inode_bitmap_blk_start + inode_number);
    block.modify((char*)&inode, BLOCK_SIZE, 0);
}
