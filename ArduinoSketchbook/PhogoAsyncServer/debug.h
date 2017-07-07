#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H

// comment out to get rid of the debug output
#define DEBUG 

#ifdef DEBUG

	#define DEBUGGING    \
		Serial.print(millis());     \
		Serial.print(" \t ");    \
		Serial.print(__PRETTY_FUNCTION__); \
		Serial.print(":");      \
		Serial.print(__LINE__);     \
		Serial.print(" \t ");      \
		Serial.printf

	#define DEBUGGINGC    \
		Serial.printf

   #define DEBUGGINGL    \
    Serial.print
#else
	#define DEBUGGING ((void) 0)
	#define DEBUGGINGC ((void) 0)
#endif /* DEBUG */

#endif /* DEBUGUTILS_H */
