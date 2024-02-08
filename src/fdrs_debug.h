#define TS ((tv.tv_sec == NULL) ? millis(): tv.tv_sec)
#ifdef FDRS_DEBUG
#ifdef USE_OLED
#define DBG(a) debug_OLED(String(a)); \
Serial.print(TS); \
Serial.print(" | "); \
Serial.println(a);
#else
#define DBG(a) \
Serial.print(TS); \
Serial.print(" | "); \
Serial.println(a);
#endif
#else
#ifdef USE_OLED
#define DBG(a) debug_OLED(String(a));
#else
#define DBG(a)
#endif
#endif