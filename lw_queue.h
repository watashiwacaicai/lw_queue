#ifndef __LW_QUEUE_H
#define __LW_QUEUE_H


/*������нṹ������*/
typedef struct 
{
    void* data;         /*���е�����ָ��*/
    size_t element_size;/*���е�Ԫ�ش�С*/
    size_t capacity;    /*���е�����*/
    size_t head;        /*����ͷ����*/
    size_t tail;        /*����β����*/

}lwq_t;

/*������м��ṹ������, �Ӷ������ͼ̳�*/
typedef struct 
{
    lwq_t** queues;
    size_t queue_count;
    size_t max_queues;

}lwq_set_t;

/*��������ڴ����������*/
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

#endif // !__LW_QUEUE_H
