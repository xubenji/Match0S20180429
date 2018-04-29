#ifndef _FIFO_H
#define _FIFO_H
#include"kernel/types.h"
#include"kernel/task.h"

/* fifo.c */
struct FIFO32 {
	int32_t *buf;
	int p, q, size, free, flags;
	struct task *task;
};
void fifo32_init(struct FIFO32 *fifo, int size, int32_t *buf);
int fifo32_put(struct FIFO32 *fifo, int32_t data);
int fifo32_get(struct FIFO32 *fifo);
int fifo32_status(struct FIFO32 *fifo);

#endif

