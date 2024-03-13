#define TS millis()
// #define TS ((tv.tv_sec == 0) ? millis(): tv.tv_sec)
void sendJson(String,String); // used in fdrs_gateway_webserver.h

#ifndef DBG_LEVEL
    #define DBG_LEVEL GLOBAL_DBG_LEVEL
#endif

#if (DBG_LEVEL == 0) 
    const uint debugLevel = DBG_LVL0; 
#endif
#if (DBG_LEVEL == 1) 
    const uint debugLevel = DBG_LVL1; 
#endif
#if (DBG_LEVEL >= 2) 
    const uint debugLevel = DBG_LVL2;
#endif
uint dbgLevelRuntime = debugLevel;

#define LVL0(a) if(dbgLevelRuntime >= DBG_LVL0) { Serial.print(TS); Serial.print(" |     "); Serial.println(a); sendJson(String("cmd_dbg"),String(TS) + "\t[0]\t" + String(a)); }
#define LVL1(a) if(dbgLevelRuntime >= DBG_LVL1) { Serial.print(TS); Serial.print(" | [1] "); Serial.println(a); sendJson(String("cmd_dbg"),String(TS) + "\t[1]\t" + String(a)); }
#define LVL2(a) if(dbgLevelRuntime >= DBG_LVL2) { Serial.print(TS); Serial.print(" | [2] "); Serial.println(a); sendJson(String("cmd_dbg"),String(TS) + "\t[2]\t" + String(a)); }
// #define LVLF0(a, ...) Serial.print(TS); Serial.print(" |     "); Serial.printf((a), ##__VA_ARGS__);
// #define LVLF1(a, ...) Serial.print(TS); Serial.print(" | [1] "); Serial.printf((a), ##__VA_ARGS__);
// #define LVLF2(a, ...) Serial.print(TS); Serial.print(" | [2] "); Serial.printf((a), ##__VA_ARGS__);
#define LVLF0(a, ...)
#define LVLF1(a, ...)
#define LVLF2(a, ...)

#ifdef FDRS_DEBUG
    #ifdef USE_OLED
        #define DBG(a) LVL0(a) debug_OLED(String(a));
        #define DBGF(a, ...) LVLF0(a, ...)
    #else
        #define DBG(a) LVL0(a)
        #define DBGF(a, ...)
    #endif // USE_OLED
    #if (DBG_LEVEL == 0)
        #define DBG1(a);
        #define DBGF1(a, ...)
        #define DBG2(a);
        #define DBGF2(a, ...)
    #endif
    #if (DBG_LEVEL == 1)
        #define DBG1(a) LVL1(a)
        #define DBGF1(a, ...) LVLF1(a, ...)
        #define DBGF2(a, ...)
    #endif
    #if (DBG_LEVEL >= 2)
        #define DBG1(a) LVL1(a)
        #define DBGF1(a, ...)
        #define DBG2(a) LVL2(a)
        #define DBGF2(a, ...)
    #endif
#else
    #ifdef USE_OLED
        #define DBG(a) debug_OLED(String(a));
    #else
        #define DBG(a)
    #endif // USE_OLED
    #define DBGF(a, ...)
    #define DBG1(a)
    #define DBGF1(a, ...)
    #define DBG2(a)
    #define DBGF2(a, ...)
#endif // FDRS_DEBUG