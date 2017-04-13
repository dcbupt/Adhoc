#include"log.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

FILE* out;
void PrintfLog(int level,char * fromat, ...){
	OS_ENTER_CRITICAL();
	#if LOG_EN ==1u
	time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	fprintf(out,"The current date/time is: %s", asctime(timeinfo));

	int int_argu;
	char char_argu, *string_argu, tmp;
	float float_argu;
	va_list args;
	if (level >= LOG_LEVEL){
		va_start(args, fromat);
		while (*fromat)
		{
			if (*fromat == '%'){
				tmp = *(fromat + 1);
				if (tmp){
					switch (tmp)
					{
					case 's':
						string_argu = va_arg(args, char *);
						printf("%s", string_argu);
						fprintf(out, "%s", string_argu);
						fromat += 2;
						break;
					case 'd':
						int_argu = va_arg(args, int);
						printf("%d", int_argu);
						fprintf(out, "%d", int_argu);
						fromat += 2;
						break;
					case 'c':
						char_argu = va_arg(args, char);
						printf("%c", char_argu);
						fprintf(out, "%c", char_argu);
						fromat += 2;
						break;
					case 'f':
						float_argu = va_arg(args, double);
						printf("%f", float_argu);
						fprintf(out, "%f", float_argu);
						fromat += 2;
						break;
					default:
						printf("%c", *fromat);
						fprintf(out, "%c", *fromat);
						fromat += 1;
						break;
					}
				}
				else{
					printf("%c", *fromat);
					fprintf(out, "%c", *fromat);
					fromat += 1;
				}
			}
			else if (*fromat == '\n'){
				printf("\n");
				fprintf(out, "\n");
				fromat += 1;
			}
			else if (*fromat == '\t'){
				printf("\t");
				fprintf(out, "\t");
				fromat += 1;
			}
			else{
				printf("%c", *fromat);
				fprintf(out, "%c", *fromat);
				fromat += 1;
			}
		}
		va_end(args);
	}
	fflush(out);
	#endif
	OS_EXIT_CRITICAL();
}


