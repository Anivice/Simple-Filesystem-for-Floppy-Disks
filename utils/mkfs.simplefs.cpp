#include <basic_io.h>
#include <simple_err.h>
#include <iostream>
#include <simplefs.h>

simplefs_head_t generate_information(off64_t device_block_count)
{
    simplefs_head_t head { };

    uint32_t inode_blk_count = device_block_count / 3 + (device_block_count / 3 == 0 ? 1 : 0);
    uint32_t inode_bitmap_blk_count = inode_blk_count / (8 * BLOCK_SIZE) + (inode_blk_count % (8 * BLOCK_SIZE) != 0 ? 1 : 0);
    uint32_t blk_left = device_block_count - 1 /* head */ - inode_blk_count - inode_bitmap_blk_count;
    auto zone_blk_count = (uint32_t)( ((double)blk_left * 8 * 512) / (1 + 8 * 512) );
    uint32_t zone_bitmap_blk_count = zone_blk_count / (8 * BLOCK_SIZE) + (zone_blk_count % (8 * BLOCK_SIZE) != 0 ? 1 : 0);

    head.content.magic = SIMPLEFS_MAGIC;
    head.content.device_block_count = device_block_count;

    head.content.inode_bitmap_blk_start = 1 /* head */;
    head.content.inode_bitmap_blk_count = inode_bitmap_blk_count;
    head.content.inode_blk_start = 1 /* head */ + inode_bitmap_blk_count + zone_bitmap_blk_count;
    head.content.inode_blk_count = inode_blk_count;

    head.content.zone_bitmap_blk_start = 1 /* head */ + inode_bitmap_blk_count;
    head.content.zone_bitmap_blk_count = zone_bitmap_blk_count;
    head.content.zone_blk_start = 1 /* head */ + inode_bitmap_blk_count + zone_bitmap_blk_count + inode_blk_count;
    head.content.zone_blk_count = zone_blk_count;

    return head;
}

int main(int, char ** argv)
{
    char empty[BLOCK_SIZE] { };
    try
    {
        io_on_dev device;
        device.open(argv[1]);

        auto head = generate_information(device.get_block_count());
        output_head_info(head);

        auto block0 = device.request(0);
        block0.modify((const char*)&head, BLOCK_SIZE, 0);

        // clear inode bitmaps
        for (uint32_t i = 0; i < head.content.inode_bitmap_blk_count; i++)
        {
            auto blk = device.request(head.content.inode_bitmap_blk_start + i);
            blk.modify(empty, BLOCK_SIZE, 0);
        }

        // clear zone bitmaps
        for (uint32_t i = 0; i < head.content.zone_bitmap_blk_count; i++)
        {
            auto blk = device.request(head.content.zone_bitmap_blk_start + i);
            blk.modify(empty, BLOCK_SIZE, 0);
        }
    }
    catch (simple_error_t & err)
    {
        std::cerr << err.what() << std::endl;
    }
}
