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
    qbuf->free_q = (RING_BUFFER*)malloc(rb_memory_required(block_num, sizeof(void *)));
    if(qbuf->free_q == NULL)
    {
        fprintf(stderr, "(%s:%u) create buffer free_q failed.\n", __FILE__, __LINE__);
        exit(-1);
    }
    qbuf->busy_q = (RING_BUFFER*)malloc(rb_memory_required(block_num, sizeof(void *)));
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
        rb_initialize(qbuf->block_num, sizeof(void *), qbuf->busy_q);
    else
        rb_initialize(qbuf->block_num, sizeof(void *), qbuf->busy_q);
    
    uint32_t i;
    for(i = 0; i < qbuf->block_num; i++)
    {
        if(0 == rb_writeInValue(qbuf->free_q, void *, (void *)((char*)qbuf->data + i * qbuf->block_size)))
        {
            fprintf(stderr, "(%s:%u) reset init free_q failed", __FILE__, __LINE__);
            exit(-1);
        }
    }
}

//创建队列
int ring_queue_create(ring_t *ring_p, int ring_num)
{
	int 			pernode = sizeof(qnode_t);
	int 			bs = sizeof(qnode_t*);
	int 			rs = ring_num;
	int 			i = 0;
	qnode_t 		*qnode_p = NULL;

	ring_p->arr = (void*)calloc(rs, pernode);
	if(ring_p->arr == NULL)
	{
		return -1;
	}

	rb_create(rs, bs, ring_p->fq);
	rb_create(rs, bs, ring_p->bq);
	
	for(i = 0; i<rs; i++)
	{
		qnode_p = (qnode_t*)((char*)ring_p->arr + (i * pernode));
		if(rb_writeInValue(ring_p->fq, qnode_t*, qnode_p) == 0)
		{
			printf("rb_writeInValue() failure exit now!\n");
			exit(-1);
		}
	}

	return 0;
}
