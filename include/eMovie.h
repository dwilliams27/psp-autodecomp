#ifndef EMOVIE_H
#define EMOVIE_H

class eMovie {
public:
    static void SetDrawCallback(void (*callback)(void *), void *data);
    void PlatformPause(void);
    void PlatformResume(void);
};

#endif
