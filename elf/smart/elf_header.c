#include "packer.h"

void	parse_header(t_ptrs *ptrs_to_old, t_offsets *ptrs_to_update, void *map, int new_file_fd)
{
	Elf64_Ehdr* E_hdr = (Elf64_Ehdr*)map;


	ptrs_to_update->e_phnum = E_hdr->e_phnum;
	ptrs_to_update->e_shnum = E_hdr->e_shnum;

	ptrs_to_update->E_entry = 0x18;
	ptrs_to_old->E_entry = E_hdr->e_entry;

	ptrs_to_update->E_phoff = 0x20;
	ptrs_to_old->E_phoff = E_hdr->e_phoff;

	ptrs_to_update->E_shoff = 0x28;
	ptrs_to_old->E_shoff = E_hdr->e_shoff;

	ptrs_to_update->bites_written = write(new_file_fd, (char*)E_hdr, 64);//based on 64 bit arch
}