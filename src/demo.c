#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rb_api.h"

#define QUEUE_NUM 125

typedef struct _queue_node_
{
    int i,j;
}queue_node_t;

//这个main1已经能够正常运行了
int main1(void)
{
	ring_t queue;
	ring_queue_create(&queue, 100);

	qnode_t *qnode_wb = NULL;
	if(rb_readOutValue(queue.fq, qnode_t*, qnode_wb) == 0)
	{
		printf("no place left!\n");
		exit(-1);
	}
	strcpy(qnode_wb->str1, "test1");	
	strcpy(qnode_wb->str2, "test2");
	while(rb_writeInValue(queue.bq, qnode_t*, qnode_wb) == 0)
	{
		printf("%s:%d rb_writeInValue failure!\n", __FILE__, __LINE__);
	}

	qnode_t *qnode_rb = NULL;
	if(rb_readOutValue(queue.bq, qnode_t*, qnode_rb) == 0)
	{
		printf("no data could be read out!\n");
	}
	else
	{
		printf("ring data is %s:%s\n", qnode_rb->str1, qnode_rb->str2);
	}

	return 0;
}


int main()
{
    queue_t q;
    create_queue_buffer(&q, sizeof(queue_node_t), QUEUE_NUM, 0);

    //线程1：生产
    queue_node_t *q_node;
    if(rb_readOutValue(q.free_q, queue_node_t *, q_node) == 0)
    {
        printf("队列中没有空闲的元素\n");
    }
    else
    {
        q_node->i = 1;
        q_node->j = 2;
        rb_writeInValue(q.busy_q, queue_node_t *, q_node);
    }
    
    //线程2：消费
    queue_node_t *q_node2;
    if(rb_readOutValue(q.busy_q, queue_node_t *, q_node2) == 0)
    {
        printf("队列中没有可以处理的元素\n");
    }
    else
    {
        printf("i:%d j:%d\n", q_node2->i, q_node2->j);
        rb_writeInValue(q.free_q, queue_node_t *, q_node2);
    }

	return 0;
}

