#ifndef GCPLAYER_H
#define GCPLAYER_H

class cFile;

class gcPlayer {
public:
    static void HandleClientWarp(void);
    static void ResetController(int);
    static void ResetControllers(void);
    static void ResetOtherControllers(void);
    void *GetCamera(void) const;
    void GetName(char *) const;
    void Write(cFile &) const;
};

#endif
