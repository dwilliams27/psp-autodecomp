// init_eh_memory_management(void) @ 0x001ba484, gMain_psp.obj
// Split-TU: a_mem_block_descr is not in any shared header, declared locally.

struct a_mem_block_descr {
	void *block;
	void *base;
	int size;
	int used;
	char flags;
};

extern void mem_block_descr_init(a_mem_block_descr *);

static char eh_mem_pool[0x2000];
a_mem_block_descr eh_mem_block;
a_mem_block_descr *eh_mem_block_ptr;

void init_eh_memory_management(void) {
	a_mem_block_descr *d = &eh_mem_block;
	mem_block_descr_init(d);
	d->base = eh_mem_pool;
	__asm__ volatile("" ::: "memory");
	d->size = 0x2000;
	d->used = 0;
	d->flags = 0;
	eh_mem_block_ptr = d;
}
