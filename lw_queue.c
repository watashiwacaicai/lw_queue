#include "lw_queue.h"
#include "stdlib.h"
#include "string.h"

/*��̬��������*/

/*������п���ڴ�����ӣ�Ĭ�ϲ���c��׼�Ĺ���*/
static lwq_hook_t lwq_mem_hook =
{
    .lwq_malloc = malloc,
    .lwq_free = free
};

/*ȫ�ֺ�������*/

/**
 * @brief ���ö��е��ڴ湳��
 * @param hook �����ڴ湳�ӱ�����ָ��
 */
void lwq_hook_init(lwq_hook_t* hook)
{
    lwq_mem_hook.lwq_malloc = hook->lwq_malloc;
    lwq_mem_hook.lwq_free = hook->lwq_free;
}

/**
 * @brief ��������
 * @param queue ���о����ָ��
 * @param capacity ���е�����
 * @param element_size ����Ԫ�صĴ�С
 */
void lwq_create(lwq_t* queue, size_t capacity, size_t element_size)
{
    queue->data = lwq_mem_hook.lwq_malloc(element_size * (capacity + 1));
    queue->element_size = element_size;
    queue->capacity = (capacity + 1);
    queue->head = 0;
    queue->tail = 0;
}

/**
 * @brief д�����ݵ�����
 * @param queue ���о����ָ��
 * @param element Ҫд������ָ��
 * @return 1���ɹ� 0��ʧ��
 */
unsigned char lwq_write(lwq_t* queue, const void* element)
{
    size_t next_tail = (queue->tail + 1) % queue->capacity;

    if (next_tail == queue->head) 
    {
        return 0; /*��������*/
    }
    memcpy((char*)queue->data + queue->tail * queue->element_size, element, queue->element_size);
    queue->tail = next_tail;
    return 1;
}
 
/**
 * @brief �Ӷ��ж�ȡ����
 * @param queue ���о����ָ��
 * @param element ���ڱ�������ȡ��������ݵ�ָ��
 * @return 1���ɹ� 0��ʧ��
 */
unsigned char lwq_read(lwq_t* queue, void* element)
{
    if (queue->head == queue->tail) 
    {
        return 0; /*����Ϊ��*/
    }
    memcpy(element, (char*)queue->data + queue->head * queue->element_size, queue->element_size);
    queue->head = (queue->head + 1) % queue->capacity;
    return 1;
}

/**
 * @brief �Ӷ��п�������
 * @param queue ���о����ָ��
 * @param element ���ڱ�������ȡ��������ݵ�ָ��
 * @return 1���ɹ� 0��ʧ��
 */
unsigned char lwq_peek(lwq_t* queue, void* element)
{
    if (queue->head == queue->tail) 
    {
        return 0; /*����Ϊ��*/
    }
    memcpy(element, (char*)queue->data + queue->head * queue->element_size, queue->element_size);
    return 1;
}

/**
 * @brief ǿ��д�����ͷ
 * @param queue ���о����ָ��
 * @param element Ҫд������ָ��
 */
void lwq_force_write_head(lwq_t* queue, const void* element)
{
    if (queue->head == 0) 
    {
        queue->head = queue->capacity - 1;
    }
    else 
    {
        queue->head--;
    }
    memcpy((char*)queue->data + queue->head * queue->element_size, element, queue->element_size);
}
 
/**
 * @brief ��ѯ�����Ƿ�����
 * @param queue ���о����ָ��
 * @return 1������ 0��δ��
 */
unsigned char lwq_is_full(lwq_t* queue)
{
    return ((queue->tail + 1) % queue->capacity) == queue->head;
}

/**
 * @brief ��ѯ�����Ƿ�Ϊ��
 * @param queue ���о����ָ��
 * @return 1�����п� 0�����зǿ�
 */
unsigned char lwq_is_empty(lwq_t* queue)
{
    return queue->head == queue->tail;
}

/**
 * @brief ���ٶ���
 * @param queue ���о����ָ��
 */
void lwq_destroy(lwq_t* queue)
{
    lwq_mem_hook.lwq_free(queue->data);
}

