#ifndef FLOPPYFS_SIMPLEFS_H
#define FLOPPYFS_SIMPLEFS_H

#include <cstdint>
#include <sys/types.h>
#include <sys/stat.h>
#include <basic_io.h>

#define SIMPLEFS_MAGIC 0xAABBCCDD

struct simplefs_head_t
{
    struct {
        uint32_t magic;
        uint32_t device_block_count;
        uint32_t inode_bitmap_blk_start;
        uint32_t inode_bitmap_blk_count;
        uint32_t zone_bitmap_blk_start;
        uint32_t zone_bitmap_blk_count;
        uint32_t inode_blk_start;
        uint32_t inode_blk_count;
        uint32_t zone_blk_start;
        uint32_t zone_blk_count;
    } content { };

    char _fill_ [ BLOCK_SIZE - sizeof(content)] { }; // empty filed
};
static_assert(sizeof(simplefs_head_t) == BLOCK_SIZE, "Incorrect head size");

struct simplefs_inode_t
{
    char name [ 32 ] { };
    struct stat inode_stat { };

    uint32_t zone_map [
            84 /* 2nd level block */
    ] { }; /* 84 * 512 / 4 * 512 = 5505024 = 5.25 Mb */
};
static_assert(sizeof(simplefs_inode_t) == BLOCK_SIZE, "Incorrect head size");


void output_head_info(simplefs_head_t head);
bool if_header_valid(simplefs_head_t head);

#endif //FLOPPYFS_SIMPLEFS_H
