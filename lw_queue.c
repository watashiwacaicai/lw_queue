#include "lw_queue.h"
#include "stdlib.h"
#include "string.h"

/*静态变量定义*/

/*定义队列库的内存管理钩子，默认采用c标准的钩子*/
static lwq_hook_t lwq_mem_hook =
{
    .lwq_malloc = malloc,
    .lwq_free = free
};

/*全局函数定义*/

/**
 * @brief 设置队列的内存钩子
 * @param hook 队列内存钩子变量的指针
 */
void lwq_hook_init(lwq_hook_t* hook)
{
    lwq_mem_hook.lwq_malloc = hook->lwq_malloc;
    lwq_mem_hook.lwq_free = hook->lwq_free;
}

/**
 * @brief 创建队列
 * @param queue 队列句柄的指针
 * @param capacity 队列的容量
 * @param element_size 队列元素的大小
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
 * @brief 写入数据到队列
 * @param queue 队列句柄的指针
 * @param element 要写入对象的指针
 * @return 1：成功 0：失败
 */
unsigned char lwq_write(lwq_t* queue, const void* element)
{
    size_t next_tail = (queue->tail + 1) % queue->capacity;

    if (next_tail == queue->head) 
    {
        return 0; /*队列已满*/
    }
    memcpy((char*)queue->data + queue->tail * queue->element_size, element, queue->element_size);
    queue->tail = next_tail;
    return 1;
}
 
/**
 * @brief 从队列读取数据
 * @param queue 队列句柄的指针
 * @param element 用于保存所读取对象的数据的指针
 * @return 1：成功 0：失败
 */
unsigned char lwq_read(lwq_t* queue, void* element)
{
    if (queue->head == queue->tail) 
    {
        return 0; /*队列为空*/
    }
    memcpy(element, (char*)queue->data + queue->head * queue->element_size, queue->element_size);
    queue->head = (queue->head + 1) % queue->capacity;
    return 1;
}

/**
 * @brief 从队列窥读数据
 * @param queue 队列句柄的指针
 * @param element 用于保存所读取对象的数据的指针
 * @return 1：成功 0：失败
 */
unsigned char lwq_peek(lwq_t* queue, void* element)
{
    if (queue->head == queue->tail) 
    {
        return 0; /*队列为空*/
    }
    memcpy(element, (char*)queue->data + queue->head * queue->element_size, queue->element_size);
    return 1;
}

/**
 * @brief 强制写入队列头
 * @param queue 队列句柄的指针
 * @param element 要写入对象的指针
 */
void lwq_force_write_head(lwq_t* queue, const void* element)
{    
    /*强制覆盖写入*/
    memcpy((char*)queue->data, element, queue->element_size);

    /*如果队列为空则移动队尾索引*/
    if (lwq_is_empty(queue)) 
    {              
        queue->tail = 1;
    }    
}
 
/**
 * @brief 查询队列是否已满
 * @param queue 队列句柄的指针
 * @return 1：已满 0：未满
 */
unsigned char lwq_is_full(lwq_t* queue)
{
    return ((queue->tail + 1) % queue->capacity) == queue->head;
}

/**
 * @brief 查询队列是否为空
 * @param queue 队列句柄的指针
 * @return 1：队列空 0：队列非空
 */
unsigned char lwq_is_empty(lwq_t* queue)
{
    return queue->head == queue->tail;
}

/**
 * @brief 销毁队列
 * @param queue 队列句柄的指针
 */
void lwq_destroy(lwq_t* queue)
{
    lwq_mem_hook.lwq_free(queue->data);
}
 
/**
 * @brief 创建队列集
 * @param queue_set 队列集句柄的指针
 * @param max_queues 队列集所容纳的最大队列数量
 */
void lwq_set_create(lwq_set_t* queue_set, size_t max_queues)
{
    queue_set->queues = (lwq_t**)lwq_mem_hook.lwq_malloc(sizeof(lwq_t*) * max_queues);
    queue_set->queue_count = 0;
    queue_set->max_queues = max_queues;
}

/**
 * @brief 添加队列到队列集
 * @param queue_set 队列集句柄的指针
 * @param queue 队列句柄的指针
 * @return 1：成功 0：失败
 */
unsigned char lwq_add_to_set(lwq_set_t* queue_set, lwq_t* queue)
{
    if (queue_set->queue_count >= queue_set->max_queues) 
    {
        return 0; /*队列集已满*/
    }
    queue_set->queues[queue_set->queue_count++] = queue;
    return 1;
}

/**
 * @brief 从队列集中获取有数据的队列
 * @param queue_set 队列集句柄的指针
 * @return 队列句柄的指针，没有队列有数据则返回NULL
 */
lwq_t* lwq_select_from_set(lwq_set_t* queue_set)
{
    for (size_t i = 0; i < queue_set->queue_count; i++) 
    {
        if (!lwq_is_empty(queue_set->queues[i]))
        {
            return queue_set->queues[i];
        }
    }
    return NULL; /*没有可用队列*/
}

/**
 * @brief 销毁队列集
 * @param queue_set 队列集句柄的指针
 */
void lwq_set_destroy(lwq_set_t* queue_set)
{
    lwq_mem_hook.lwq_free(queue_set->queues);
}

