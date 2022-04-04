#include <stdio.h>

FILE *yyin;
int code = 0;

int yylex()
{
	int prev = code;
	int i = getc(yyin);
	if (i == 65 || i == 69 || i == 73 || i == 79 || i == 85 ||
	i == 97 || i == 101 || i == 105 || i == 111 || i == 117)
	{
		//ASCII: 1 = Decimal: 49
		code = 49;
	} else if ((65 < i && i <= 90) || (97 < i && i <= 122)) {
		code = 50;
	} else if (48 <= i && i <= 57){
		code = 51;
	} else if (i == 32 || i == 10 || i == 9) {
		code = 53;
	} else if (i == -1){
		code = i;
	} else {
		code = 52;
	}
	
	if(prev == code) {
		//printf("  \'MATCH\'  ");
		return 0;
	}
			   
	return code;
}
