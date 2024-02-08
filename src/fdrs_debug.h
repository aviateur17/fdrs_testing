#define TS ((tv.tv_sec == NULL) ? millis(): tv.tv_sec)

#if !defined(FDRS_DEBUG) && !defined(FDRS_DEBUG_FINE) && !defined(FDRS_DEBUG_FINEST)
#ifdef USE_OLED
#define DBG(a) debug_OLED(String(a));
#else
#define DBG(a)
#endif
#endif

#ifdef FDRS_DEBUG
#ifdef USE_OLED
#define DBG(a) Serial.print(TS); Serial.print(" | "); Serial.println(a); debug_OLED(String(a));
#else
#define DBG(a) Serial.print(TS); Serial.print(" | "); Serial.println(a);
#endif // USE_OLED
#define DBGFST(a)
#define DBGF(a)
#endif // FDRS_DEBUG

#ifdef FDRS_DEBUG_FINE
#ifdef USE_OLED
#define DBG(a) Serial.print(TS); Serial.print(" |        "); Serial.println(a); debug_OLED(String(a));
#else
#define DBG(a) Serial.print(TS); Serial.print(" |        "); Serial.println(a);
#endif // USE_OLED
#define DBGF(a) Serial.print(TS); Serial.print(" | [fine] "); Serial.println(a);
#define DBGFST(a)
#endif //FDRS_DEBUG_FINE

#ifdef FDRS_DEBUG_FINEST
#ifdef USE_OLED
#define DBG(a) Serial.print(TS); Serial.print(" |          "); Serial.println(a); debug_OLED(String(a));
#else
#define DBG(a) Serial.print(TS); Serial.print(" |          "); Serial.println(a);
#endif // USE_OLED
#define DBGFST(a) Serial.print(TS); Serial.print(" | [finest] "); Serial.println(a);
#define DBGF(a) Serial.print(TS); Serial.print(" | [fine]   "); Serial.println(a);
#endif //FDRS_DEBUG_FINEST
