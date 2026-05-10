namespace std {
    class exception {
    public:
        virtual ~exception(void);
        virtual const char *what(void) const;
    };

    class bad_exception : public exception {
    public:
        virtual ~bad_exception(void);
        virtual const char *what(void) const;
    };
}

std::bad_exception::~bad_exception(void) {
}
