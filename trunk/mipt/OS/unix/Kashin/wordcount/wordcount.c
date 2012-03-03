#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char s[255];
int main(int argc, char** argv)
{
	int words_count = 0;
	int inside_gaps = 0;
	int gaps_filled = 0;
	int inside_word = 0;
	while (!feof(stdin))
	{
		gets(s);
		if (feof(stdin))
			break;
		int length = strlen(s);
		for(int i = 0; i < length; i++)
		{
			if (s[i] == '"')
			{
				gaps_filled = 0;
				inside_gaps ^= 1;
			}
			else
			if (s[i] < 33)
			{
				if (inside_word)
				{
					if (inside_gaps)
					{
						if (gaps_filled == 0)
							words_count++;
						gaps_filled = 1;
					}
					else
					{
						words_count++;
					}
					inside_word = 0;
				}
			}
			else
			{
				inside_word = 1;
			}
		}
		if (inside_word)
			words_count++;
		inside_word = 0;
	}
	if (inside_gaps)
		printf("Error, gaps are unbalanced.\n");
	else
		printf("%d\n", words_count);
	return 0;
}
