// This SECTION CONSISTS OF HEADERS THAT NEED TO BE DEFINED ON HOST OS
//
//
//

#include <syscall.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

struct linux_dirent {
	long int d_ino;
	off_t d_off;
	unsigned short d_reclen;
	char d_name[];
};

struct date
{
	int date,mon,year,wday;
	int h,m,s,yday;
};

// THIS SECTION CONSISTS OF LIBRARY FUNCTIONS
//
//
//

// Included strcmp

int strcmp (const char *p1, const char *p2)
{
	const unsigned char *s1 = (const unsigned char *) p1;
	const unsigned char *s2 = (const unsigned char *) p2;
	unsigned char c1, c2;

	do
	{
		c1 = (unsigned char) *s1++;
		c2 = (unsigned char) *s2++;
		if (c1 == 0)
			return c1 - c2;
	}
	while (c1 == c2);

	return c1 - c2;
}

// Included strlen

size_t strlen (const char *str)
{
	size_t c = 0;
	unsigned char *cc = (unsigned char *) str;
	while (*cc++)
		c++;

	return c + 1;
}

// Included strcpy

char *strcpy(char *dest, const char *src)
{
	char c;
	char *s = (char *) src;
	const ptrdiff_t off = dest - s - 1;

	do
	{
		c = *s++;
		s[off] = c;
	}
	while (c != 0);

	return dest;
}

// Included strcat

char *strcat (char *dest, const char *src)
{
	char *s1 = dest;
	const char *s2 = src;
	char c;

	/* Find the end of the string.  */

	do
		c = *s1++;
	while (c != 0);

	s1 -= 2;

	do
	{
		c = *s2++;
		*++s1 = c;
	}
	while (c != 0);

	return dest;
}

// Included strchr

char *strchr (const char *s, int c_in)
{
	unsigned char *c = (unsigned char *)s;
	int x = (int) c_in;

	while (c++ && *c)
		if (*c == x)
			return (char *) c;

	return NULL;
}

// Included strrev

void strrev(char *p)
{
	char *q = p;
	while(q && *q) ++q;
	for(--q; p < q; ++p, --q)
		*p = *p ^ *q,
			*q = *p ^ *q,
			*p = *p ^ *q;
}

// Included own itoa (base 10 only)

char *itoa(unsigned long long int x , char *buffer)
{
	if (!x)
		return "0";
	char *c = buffer;
	while (x)
	{
		*c = x % 10 + '0';
		x = x / 10;
		c++;
	}
	*c = 0;
	strrev(buffer);
	return buffer;
}

char *itoa_date(unsigned long long int x , char *buffer) //lazy hack
{
	char *c = buffer;

	if (x < 10)
	{
		switch (x)
		{
			case 0:
				return "00";
			case 1:
				return "01";
			case 2:
				return "02";
			case 3:
				return "03";
			case 4:
				return "04";
			case 5:
				return "05";
			case 6:
				return "06";
			case 7:
				return "07";
			case 8:
				return "08";
			case 9:
				return "09";
		}
	}

	while (x)
	{
		*c = x % 10 + '0';
		x = x / 10;
		c++;
	}

	*c = 0;

	strrev(buffer);

	return buffer;
}

// Included own atoll

unsigned long long int atoll (char *s)
{
	char *ss=s;
	unsigned long long int num = 0;
	while(*ss)
	{
		num = num * 10;
		num = num + (int)(*ss - '0');
		ss++;
	}
	return num;
}

// Included own qsort (strings only)

void QSort(char List[][1024], int Left, int Right)
{
	int i, j;
	char *x;
	char TEMP[1024];

	i = Left;
	j = Right;
	x = List[(Left+Right)/2];

	do {
		while((strcmp(List[i],x) < 0) && (i < Right)) {
			i++;
		}
		while((strcmp(List[j],x) > 0) && (j > Left)) {
			j--;
		}
		if(i <= j) {
			strcpy(TEMP, List[i]);
			strcpy(List[i], List[j]);
			strcpy(List[j], TEMP);
			i++;
			j--;
		}
	} while(i <= j);

	if(Left < j) {
		QSort(List, Left, j);
	}
	if(i < Right) {
		QSort(List, i, Right);
	}
}

// THIS SECTION HAS #define-s THAT I THOUGHT WERE USEFUL
//
//
//

#define _write_wrapper(string) syscall(SYS_write , STDOUT_FILENO , string , strlen(string))
#define endl _write_wrapper("\n\0")
#define tabspace _write_wrapper("\t\0")

#define LS_L 0x01
#define LS_A 0x02
#define LS_H 0x04

#define BUFFER_SIZE 4096

#define YEAR0           1900
#define EPOCH_YR        1970
#define SECS_DAY        (24L * 60L * 60L)
#define LEAPYEAR(year)  (!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define YEARSIZE(year)  (LEAPYEAR(year) ? 366 : 365)
#define FIRSTSUNDAY(timp)       (((timp)->tm_yday - (timp)->tm_wday + 420) % 7)
#define FIRSTDAYOF(timp)        (((timp)->tm_wday - (timp)->tm_yday + 420) % 7)
#define TIME_MAX        ULONG_MAX
#define ABB_LEN         3

#define HOUR_OFFSET 5
#define MINUTE_OFFSET 30
#define SECOND_OFFSET 0

const int _ytab[2][12] = {
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

// PROGRAM SPECIFIC FUNCTIONS
//
//
//

void findAndPrint (char *l)
{
	char *line = l;
	int len=0;

	while(*line != ':' && *line)
	{
		line++;
		len++;
	}
	line++;

	if(l[0] == 'N' && l[1] == 'a' && len == 4) // Laziness ki koi hadd nahi :P
	{
		_write_wrapper("Process Name      : \t\t");
		_write_wrapper(line);
	}

	if(l[0] == 'S' && len == 5)
	{
		_write_wrapper("Process Status    : \t\t");
		_write_wrapper(line);
	}

	if(l[0] == 'P' && len == 4)
	{
		_write_wrapper("Parent PID        : \t\t");
		_write_wrapper(line);
	}

	if(l[0] == 'T' && len == 7)
	{
		_write_wrapper("Threads           : \t\t");
		_write_wrapper(line);
	}
}

void printStatus(char *path)
{
	char readBuffer[ 1048576 ]; // Read All in one pass. Fuck loops.
	int i;
	int fd = syscall (SYS_open , path , O_RDONLY );
	syscall( SYS_read , fd , readBuffer , sizeof(readBuffer) );
	char *poof = readBuffer;
	while (*poof && poof)
	{
		i = 0;
		char temp_line[1000];
		while (*poof && *poof != '\n')
		{
			temp_line[i] = *poof;
			poof++;
			i++;
		}
		temp_line[i] = '\n';
		temp_line[i + 1] = 0;
		findAndPrint(temp_line);
		poof++;
	}

	syscall(SYS_close , fd);
}

const char * getNum( const int argc , const char ** argv)
{
	int i;
	for (i = 1 ; i < argc ; i++)
	{
		char *ch = (char *)argv[i];
		while (*ch)
		{
			if ( ((*ch - '0') >= 0) && ((*ch - '0') <= 9) )
				ch++;
			else
				break;
		}
		if (!(*ch))
			return argv[i];
	}
	return NULL;
}

int main(const int argc, char const ** argv)
{
	const char *PID = getNum(argc,argv);

	if (PID)
	{
		//_write_wrapper(PID);
		char path[100];
		strcpy(path , "/proc/");
		strcat(path , PID);
		strcat(path , "/status");

		if (syscall(SYS_access , path , F_OK) != 0)
		{
			_write_wrapper("No Process found having this PID\n");
			return -1;
		}
		else
			printStatus(path);
	}
	else
	{
		_write_wrapper("Enter Numbers\n");
	}
	return 0;
}
