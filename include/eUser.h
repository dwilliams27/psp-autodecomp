#ifndef EUSER_H
#define EUSER_H

class eUser {
public:
    static bool PlatformSignIn(void);
    static void PlatformUpdate(void);
    static void Update(void);
    static void SignIn(unsigned short user);
    static void ChangeMask(unsigned short pos, bool set, unsigned short &mask);
    static bool GetUserName(unsigned short user, char *dst);
};

#endif
