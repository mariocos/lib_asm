#ifndef PACKER_H
#define PACKER_H

//libs
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>


typedef struct s_ptrs
{
	/* data */
	void* E_entry;
	void* E_shoff;
	void* E_phoff;
}			t_ptrs;

typedef struct s_offsets
{
	size_t	bites_written;
	int		e_phnum;
	int		e_shnum;
	/* data */
	size_t E_entry;
	size_t E_shoff;
	size_t E_phoff;
}			t_offsets;


//functons
void	*get_map(char *str);
int	get_new_file_fd(char *str);



void	parse_header(t_ptrs *ptrs_to_old, t_offsets *ptrs_to_update, void *map, int new_file_fd);











#endif