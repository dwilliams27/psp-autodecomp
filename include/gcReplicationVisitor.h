#ifndef GCREPLICATIONVISITOR_H
#define GCREPLICATIONVISITOR_H

class gcRegionBase {
public:
    gcRegionBase &operator=(const gcRegionBase &);
};

class gcLoadingScreen {
public:
    gcLoadingScreen &operator=(const gcLoadingScreen &);
};

class gcMap {
public:
    gcMap &operator=(const gcMap &);
};

class gcGame {
public:
    gcGame &operator=(const gcGame &);
};

#endif
