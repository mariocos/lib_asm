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
	Elf64_Ehdr*	Ehdr;
	void* 		E_entry;
	void* 		E_shoff;
	void* 		E_phoff;
}			t_ptrs;

typedef struct s_offsets	//theese are not values but offsets to where the values should be stored
{
	size_t	bites_written;
	int		e_phnum;
	int		e_shnum;
	/* data */
	size_t E_entry;
	size_t E_shoff;
	size_t E_phoff;
	/*phdr data*/	//theese are for the pre existing phdrs to be ajusted as well as the new phdr
	size_t	phnum;
	size_t	p_offset; //should use ptr arithmetic to reach the rest of the phdrs by knowing sizeof(phdr) and phnum
	size_t	p_vaddr;
	size_t	p_paddr;
	size_t	p_align;
	/*new_phdr data*/ // the parts we dont know exactly what will be for the new phdr
	size_t	p_filesz;
	size_t	p_memsz;


}			t_offsets;


//functons
void	*get_map(char *str);
int	get_new_file_fd(char *str);



void	parse_header(t_ptrs *ptrs_to_old, t_offsets *ptrs_to_update, void *map, int new_file_fd);











#endif