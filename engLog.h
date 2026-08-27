#ifndef NENGINE_LOG_FILE
#define NENGINE_LOG_FILE
#include <stdio.h>
#include <time.h>
#include <time.h>
FILE *logFile;
void logPrint(char *s, ...) {
	va_list	a;
	va_start(a,s);
	vfprintf(logFile,s,a);
	vprintf(s,a);
	va_end(a);
	fflush(logFile);
}
void logOpen(char *name) {
	logFile = fopen(name,"w");
	logPrint(
		"*******************************************************************************\n");
	logPrint("Logfile \"%s\", OpenGL Engine, Copyright(C), Drive && Evil Spirit\n",name);
	time_t t = time(NULL);
	logPrint("Builded %s, %s; Runned %s",__DATE__,__TIME__,ctime(&t));
	logPrint(
		"*******************************************************************************\n");

}
void logClose(void) {
	logPrint(
		"*******************************************************************************\n");
	logPrint("End of Logfile.\n");
	logPrint(
		"*******************************************************************************\n");
	fclose(logFile);
}
#endif
