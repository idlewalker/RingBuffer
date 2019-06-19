#include "rb_api.h"

void create_queue_buffer(queue_t *qbuf, int block_size, int block_num, unsigned char sem)
{
    qbuf->sem = sem;
    qbuf->block_size = block_num;
    qbuf->block_num = block_num;
    qbuf->data = malloc(block_size * block_num);
    if(qbuf->data == NULL)
    {
        fprintf(stderr, "(%s:%u) malloc space for buffer failed.\n", __FILE__, __LINE__);
        exit(-1);
    }
    qbuf->free_q = malloc(rb_memory_required(block_num, sizeof(void *)));
    if(qbuf->free_q == NULL)
    {
        fprintf(stderr, "(%s:%u) create buffer free_q failed.\n", __FILE__, __LINE__);
        exit(-1);
    }
    qbuf->busy_q = malloc(rb_memory_required(block_num, sizeof(void *)));
    if(qbuf->busy_q == NULL)
    {
        fprintf(stderr, "(%s:%u) create buffer busy_q failed.\n", __FILE__, __LINE__);
        exit(-1);
    }
    reset_queue_buffer(qbuf);
}

void reset_queue_buffer(queue_t *qbuf)
{
    rb_initialize(qbuf->block_num, sizeof(void *), qbuf->free_q);
    if(qbuf->sem)
        rb_initialize_s(qbuf->block_num, sizeof(void *), qbuf->busy_q);
    else
        rb_initialize(qbuf->block_num, sizeof(void *), qbuf->busy_q);
    
    uint32_t i;
    for(i = 0; i < qbuf->block_num; i++)
    {
        if(0 == rb_writeInValue(qbuf->free_q, void *, (void *)(qbuf->data + i * qbuf->block_size)))
        {
            fprintf(stderr, "(%s:%u) reset init free_q failed", __FILE__, __LINE__);
            exit(-1);
        }
    }
}