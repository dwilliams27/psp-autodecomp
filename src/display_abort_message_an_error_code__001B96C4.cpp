// display_abort_message(an_error_code) @ 0x001b96c4
// Obj: gMain_psp.obj

enum an_error_code { AEC_DUMMY };

struct SomeContext {
    char pad[12];
    void *unkC;
};

extern SomeContext *g_ctx;
extern const char *error_text(an_error_code);
extern int fprintf(void *stream, const char *fmt, ...);
extern const char g_fmt[];

void display_abort_message(an_error_code code)
{
    void *stream = g_ctx->unkC;
    const char *fmt = g_fmt;
    const char *first = error_text((an_error_code)1);
    fprintf(stream, fmt, first, error_text(code));
}
