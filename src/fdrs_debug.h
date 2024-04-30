#define TS millis()
// #define TS ((tv.tv_sec == 0) ? millis(): tv.tv_sec)
void sendJson(String,String); // function template for function used later in fdrs_gateway_webserver.h

#ifndef DBG_LEVEL
    #define DBG_LEVEL GLOBAL_DBG_LEVEL
#endif

int dbgLevelRuntime = DBG_LEVEL;

#ifdef USE_WEBSERVER
    #define WSPRINT0(a) sendJson(String("cmd_dbg"),String(TS) + "\t[0]\t" + String(a));
    #define WSPRINT1(a) sendJson(String("cmd_dbg"),String(TS) + "\t[1]\t" + String(a));
    #define WSPRINT2(a) sendJson(String("cmd_dbg"),String(TS) + "\t[2]\t" + String(a));
#else
    #define WSPRINT0(a)
    #define WSPRINT1(a)
    #define WSPRINT2(a)
#endif

#ifdef USE_OLED
    #define OLEDPRINT(a) debug_OLED(String(a));
#else
    #define OLEDPRINT(a)
#endif

#define LVL0(a) if(dbgLevelRuntime >= DBG_LVL0) { Serial.print(TS); Serial.print(" |     "); Serial.println(a); WSPRINT0(a)  OLEDPRINT(a) }
#define LVL1(a) if(dbgLevelRuntime >= DBG_LVL1) { Serial.print(TS); Serial.print(" | [1] "); Serial.println(a); WSPRINT1(a) }
#define LVL2(a) if(dbgLevelRuntime >= DBG_LVL2) { Serial.print(TS); Serial.print(" | [2] "); Serial.println(a); WSPRINT2(a) }
// #define LVLF0(a, ...) Serial.print(TS); Serial.print(" |     "); Serial.printf((a), ##__VA_ARGS__);
// #define LVLF1(a, ...) Serial.print(TS); Serial.print(" | [1] "); Serial.printf((a), ##__VA_ARGS__);
// #define LVLF2(a, ...) Serial.print(TS); Serial.print(" | [2] "); Serial.printf((a), ##__VA_ARGS__);
#define LVLF0(a, ...)
#define LVLF1(a, ...)
#define LVLF2(a, ...)

#ifdef FDRS_DEBUG
    #if (DBG_LEVEL == 0)
        #define DBG(a) LVL0(a)
        #define DBG1(a);
        #define DBG2(a);
        #define DBGF(a, ...)
        #define DBGF1(a, ...)
        #define DBGF2(a, ...)
    #endif
    #if (DBG_LEVEL == 1)
        #define DBG(a) LVL0(a)
        #define DBG1(a) LVL1(a)
        #define DBG2(a);
        #define DBGF(a, ...) LVLF0(a, ...)
        #define DBGF1(a, ...) LVLF1(a, ...)
        #define DBGF2(a, ...)
    #endif
    #if (DBG_LEVEL >= 2)
        #define DBG(a) LVL0(a)
        #define DBG1(a) LVL1(a)
        #define DBG2(a) LVL2(a)
        #define DBGF(a, ...) LVLF0(a, ...)
        #define DBGF1(a, ...) LVLF1(a, ...)
        #define DBGF2(a, ...) LVLF2(a, ...)
    #endif
#else // FDRS_DEBUG may not be enabled but OLED may be.
    #define DBG(a) OLEDPRINT(a)
    #define DBG1(a)
    #define DBG2(a)
    #define DBGF(a, ...)
    #define DBGF1(a, ...)
    #define DBGF2(a, ...)
#endif // FDRS_DEBUG