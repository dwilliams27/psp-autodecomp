#ifndef GCPLAYER_H
#define GCPLAYER_H

class cFile;

class gcCamera;

class gcPlayer {
public:
    static void HandleClientWarp(void);
    static void ResetController(int);
    static void ResetControllers(void);
    static void ResetOtherControllers(void);
    static int BindDefaultController(void);
    static int AssignNew(void);
    static int GetStartingController(void);
    static int BindLocalController(void);
    static void AssignLocalController(int, bool);
    static gcPlayer *GetPlayerForCamera(const gcCamera *);
    void *GetCamera(void) const;
    void GetName(char *) const;
    void Write(cFile &) const;
};

#endif
