// __sccl @ 0x001ab584 (gMain_psp.obj)
// FreeBSD libc vfscanf.c scanset-table scanner.

typedef signed char s8;

extern "C" const s8 *__sccl(s8 *tab, const s8 *fmt);

const s8 *__sccl(s8 *tab, const s8 *fmt)
{
	int c, n, v;

	/* first `clear' the whole table */
	c = *fmt++;		/* first char hat (?) */
	if (c == '^') {
		v = 1;		/* default => accept */
		c = *fmt++;	/* get new first char */
	} else
		v = 0;		/* default => reject */

	for (n = 0; n < 256; n++)
		tab[n] = v;	/* memset(tab, v, 256) */

	if (c == 0)
		return (fmt - 1);/* format ended before closing ] */

	v = 1 - v;
	for (;;) {
		tab[c] = v;		/* take character c */
doswitch:
		n = *fmt++;		/* and examine the next */
		switch (n) {
		case '-':
			n = *fmt;
			if (n == ']' || n < c) {
				c = '-';
				break;	/* resume the for(;;) */
			}
			fmt++;
			do {		/* fill in the range */
				tab[++c] = v;
			} while (c < n);
			goto doswitch;

		case 0:			/* format ended too soon */
			return (fmt - 1);

		case ']':		/* end of scanset */
			return (fmt);

		default:		/* just another character */
			c = n;
			break;
		}
	}
}
