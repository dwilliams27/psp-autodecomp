class eMoviePlatform {
public:
    static void dispbuf_func(unsigned int, void *);
    void dispbuf_thread();
};

void eMoviePlatform::dispbuf_func(unsigned int, void *argp) {
    (*(eMoviePlatform **)argp)->dispbuf_thread();
}
