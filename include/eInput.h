#ifndef EINPUT_H
#define EINPUT_H

namespace eInput {

struct eButtonState {
    float analog;              // 0
    unsigned char rawValue;    // 4
    unsigned char justPressed; // 5
    unsigned char pad1[2];     // 6-7
    unsigned char justReleased;// 8
    char pad2[7];              // 9-15
    int rawInt;                // 16
    int pad3;                  // 20

    void Update(const eButtonState &old, bool, bool, unsigned char);
};

} // namespace eInput

#endif
