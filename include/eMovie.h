#ifndef EMOVIE_H
#define EMOVIE_H

class cTimeValue;

class eMovie {
public:
    eMovie(void);
    ~eMovie(void);
    static void operator delete(void *);
    static void Initialize(void);
    void Close(void);
    void Update(cTimeValue *);
    static void Pause(void);
    static void Resume(void);
    static void SetDrawCallback(void (*callback)(void *), void *data);
    void PlatformPause(void);
    void PlatformResume(void);
    void ResetTime(void);
};

#endif
