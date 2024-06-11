#define TS ((tv.tv_sec == 0) ? millis(): tv.tv_sec)

#define TS millis()

#ifndef DBG_LEVEL
    #define DBG_LEVEL GLOBAL_DBG_LEVEL
#endif

#ifdef FDRS_DEBUG
    #ifdef USE_OLED
        #define DBG(a) Serial.print(TS); Serial.print(" |     "); Serial.println(a); debug_OLED(String(a));
        #define DBGF(_f, ...) Serial.print(TS); Serial.print(" |     "); Serial.printf((_f), ##__VA_ARGS__);
    #else
        #define DBG(a) Serial.print(TS); Serial.print(" |     "); Serial.println(a);
        #define DBGF(_f, ...) Serial.print(TS); Serial.print(" |     "); Serial.printf((_f), ##__VA_ARGS__);
    #endif // USE_OLED
    #if (DBG_LEVEL == 0)
        #define DBG1(a);
        #define DBGF1(_f, ...)
        #define DBG2(a);
        #define DBGF2(_f, ...)
    #endif
    #if (DBG_LEVEL == 1)
        #define DBG1(a) Serial.print(TS); Serial.print(" | [1] "); Serial.println(a);
        #define DBGF1(_f, ...) Serial.print(TS); Serial.print(" | [1] "); Serial.printf((_f), ##__VA_ARGS__);
        #define DBG2(a)
        #define DBGF2(_f, ...)
    #endif
    #if (DBG_LEVEL >= 2)
        #define DBG1(a) Serial.print(TS); Serial.print(" | [1] "); Serial.println(a);
        #define DBGF1(_f, ...) Serial.print(TS); Serial.print(" | [1] "); Serial.printf((_f), ##__VA_ARGS__);
        #define DBG2(a) Serial.print(TS); Serial.print(" | [2] "); Serial.println(a);
        #define DBGF2(_f, ...) Serial.print(TS); Serial.print(" | [2] "); Serial.printf((_f), ##__VA_ARGS__);
    #endif
#else
    #ifdef USE_OLED
        #define DBG(a) debug_OLED(String(a));
    #else
        #define DBG(a)
    #endif // USE_OLED
    #define DBGF(_f, ...)
    #define DBG1(a)
    #define DBGF1(_f, ...)
    #define DBG2(a)
    #define DBGF2(_f, ...)
#endif // FDRS_DEBUG