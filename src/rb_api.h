#ifndef __RB_API_H__
#define __RB_API_H__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ring_buffer.h"

typedef struct _queue_t
{
    unsigned char sem;      //用于同步的信号量
    uint32_t block_size;    //数据块大小
    uint32_t block_num;     //队列大小
    RING_BUFFER *free_q;    //空闲队列指针
    RING_BUFFER *busy_q;    //非空闲队列指针
    void *data;             //数据块区域

}queue_t;

typedef struct _ring_t_
{
	void 	*arr;
	void 	*fq;
	void 	*bq;
	int 	ring_num;
}ring_t;

typedef struct _qnode_t
{
	char str1[8];
	char str2[8];
}qnode_t;

int ring_queue_create(ring_t *ring_p, int ring_num);
void create_queue_buffer(queue_t *qbuf, int block_size, int block_num, unsigned char sem);
void reset_queue_buffer(queue_t *qbuf);

#endif

