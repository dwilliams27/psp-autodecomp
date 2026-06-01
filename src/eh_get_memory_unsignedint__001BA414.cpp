// eh_get_memory(unsigned int) @ 0x001ba414 (gMain_psp.obj)
// Symbol: __0FNeh_get_memoryUi

extern "C" void *malloc(unsigned int);
extern void __call_terminate(void);

void *eh_get_memory(unsigned int size)
{
	void *p = malloc(size);
	if (p == 0) {
		__call_terminate();
	}
	return p;
}
