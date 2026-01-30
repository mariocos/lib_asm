#ifndef LIB_ASM_H
#define LIB_ASM_H


int ft_strlen(char* str);

char* ft_strcpy(char* dest, char* src);

long int ft_write(int fd, char* str, int bts);
long int ft_read(int fd, char* str, int bts);


#endif