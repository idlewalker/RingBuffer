#ifndef __RB_API_H__
#define __RB_API_H__

#include "ring_buffer.h"

typedef struct _queue_
{
    unsigned char sem;      //用于同步的信号量
    uint32_t block_size;    //数据块大小
    uint32_t blcok_num;     //队列大小
    RING_BUFFER *free_q;    //空闲队列指针
    RING_BUFFER *busy_q;    //非空闲队列指针
    void *data;             //数据块区域

}queue_t;

void create_queue_buffer(queue_t *qbuf, int block_size, int block_num, unsigned char sem);
void reset_queue_buffer(queue_t *qbuf);

#endif