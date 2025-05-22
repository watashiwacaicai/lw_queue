#ifndef __LW_QUEUE_H
#define __LW_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdlib.h"

/*定义队列结构体类型*/
typedef struct 
{
    void* data;         /*队列的数据指针*/
    size_t element_size;/*队列的元素大小*/
    size_t capacity;    /*队列的容量*/
    size_t head;        /*队列头索引*/
    size_t tail;        /*队列尾索引*/

}lwq_t;

/*定义队列集结构体类型, 从队列类型继承*/
typedef struct 
{
    lwq_t** queues;
    size_t queue_count;
    size_t max_queues;

}lwq_set_t;

/*定义队列内存管理钩子类型*/
typedef struct
{
    void* (*lwq_malloc)(size_t size);
    void (*lwq_free)(void* ptr);

}lwq_hook_t;

void lwq_hook_init(lwq_hook_t* hook);
void lwq_create(lwq_t* queue, size_t capacity, size_t element_size);
unsigned char lwq_write(lwq_t* queue, const void* element);
unsigned char lwq_read(lwq_t* queue, void* element);
unsigned char lwq_peek(lwq_t* queue, void* element);
void lwq_force_write_head(lwq_t* queue, const void* element);
unsigned char lwq_is_full(lwq_t* queue);
unsigned char lwq_is_empty(lwq_t* queue);
void lwq_destroy(lwq_t* queue);
void lwq_set_create(lwq_set_t* queue_set, size_t max_queues);
unsigned char lwq_add_to_set(lwq_set_t* queue_set, lwq_t* queue);
lwq_t* lwq_select_from_set(lwq_set_t* queue_set);
void lwq_set_destroy(lwq_set_t* queue_set);

#ifdef __cplusplus
}
#endif

#endif // !__LW_QUEUE_H
