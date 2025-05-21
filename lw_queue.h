#ifndef __LW_QUEUE_H
#define __LW_QUEUE_H


/*定义队列结构体类型*/
typedef struct 
{
    void* data;         /*队列的数据指针*/
    size_t element_size;/*队列的元素大小*/
    size_t capacity;    /*队列的容量*/
    size_t head;        /*队列头索引*/
    size_t tail;        /*队列尾索引*/

}lwq_t;

/*定义队列内存管理类型*/
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

#endif // !__LW_QUEUE_H
