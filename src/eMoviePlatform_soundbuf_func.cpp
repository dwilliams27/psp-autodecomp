class eMoviePlatform {
public:
    static void soundbuf_func(unsigned int, void *);
    void soundbuf_thread();
};

void eMoviePlatform::soundbuf_func(unsigned int, void *argp) {
    (*(eMoviePlatform **)argp)->soundbuf_thread();
}
