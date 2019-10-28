#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "rb_api.h"

#define QUEUE_NUM 125

typedef struct _queue_node_
{
    int i,j;
}queue_node_t;

int over = 0;
void* producer(void* arg)
{
	ring_t *queue_p = (ring_t*)arg;
	int i;

	for(i=0; i<10000; i++)
	{
		qnode_t *qnode = NULL;
		if(rb_readOutValue(queue_p->fq, qnode_t*, qnode) == 0)
		{
			printf("no place left!\n");
			return NULL;
		}
		sprintf(qnode->data, "send data ssssssssssssssssss count = %d\n", i);	
		while(rb_writeInValue(queue_p->bq, qnode_t*, qnode) == 0)
		{
			printf("%s:%d rb_writeInValue failure!\n", __FILE__, __LINE__);
		}
		usleep(100);
	}
	printf("producer send data over!\n");
	return NULL;
}

void* consumer(void *arg)
{
	ring_t 	*queue_p  = (ring_t*)arg;
	
	while(1)
	{
		qnode_t *qnode = NULL;
		if(rb_readOutValue(queue_p->bq, qnode_t*, qnode) == 0)
		{
			printf("no more data could be read!\n");
			over = 1;
			return NULL;
		}
		printf("consumer get: %s", qnode->data);
		while(rb_writeInValue(queue_p->fq, qnode_t*, qnode) == 0)
		{
			printf("%s:%d rb_writeInValue failure!\n", __FILE__, __LINE__);
		}
		usleep(100);
	}	

	return NULL;
}

//这个main1已经能够正常运行了
int main(void)
{
	ring_t queue;
	ring_queue_create(&queue, 1000);

	pthread_t 	tid1;
	pthread_t 	tid2;
	pthread_create(&tid1, NULL, producer, (void*)&queue);
	usleep(100);
	pthread_create(&tid2, NULL, consumer, (void*)&queue);
	while(!over)
	{
		usleep(1000);
	}
	return 0;
}


int main1()
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

