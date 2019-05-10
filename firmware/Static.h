#pragma once

// Utility class mostly for when certain inputs need singleton callback handlers
template<typename T> class Static {
public:
    static T* instance() {
        return s_instance;
    }
private:
    static T* s_instance;
};

#define NAMED_STATIC_ALLOC(Cls, StaticName) static Cls _staticAlloc__ ## StaticName;\
                          template<> Cls* Static<Cls>::s_instance=&_staticAlloc__ ## StaticName;

#define STATIC_ALLOC(Cls) NAMED_STATIC_ALLOC(Cls, Cls)
