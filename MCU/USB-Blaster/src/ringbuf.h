#ifndef __RINGBUF_H__
#define __RINGBUF_H__

#include "base.h"

typedef struct ring_buffer {
    void *buffer;   //������
    u32 size;       //��С
    u32 in;         //���λ��
    u32 out;        //����λ��
} ringbuf_t;

u32 ringbuf_init(ringbuf_t *ring_buf, void *buffer, u32 size);
u32 ringbuf_use_len(const ringbuf_t *ring_buf);
u32 ringbuf_is_full(const ringbuf_t *ring_buf);
u32 ringbuf_get(ringbuf_t *ring_buf, void *buffer, u32 size);
u32 ringbuf_put(ringbuf_t *ring_buf, const void *buffer, u32 size);
u32 ringbuf_putc(ringbuf_t *ring_buf, const u8 c);

#endif //__RINGBUF_H__
