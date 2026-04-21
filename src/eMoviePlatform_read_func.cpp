class eMoviePlatform {
public:
    static void read_func(unsigned int, void *);
    void read_thread();
};

void eMoviePlatform::read_func(unsigned int, void *argp) {
    (*(eMoviePlatform **)argp)->read_thread();
}
