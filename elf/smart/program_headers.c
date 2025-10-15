#include "packer.h"

void write_new_phdrs(t_offsets *ptrs_to_update, int new_file_fd);

void	handle_phdrs(t_ptrs *ptrs_to_old, t_offsets *ptrs_to_update, void *map, int new_file_fd)
{
	int	phdr_num = ptrs_to_old->Ehdr->e_phnum;

	//save offsets to first phdr to update later
	Elf64_Phdr* phdr_ptr = (Elf64_Phdr*)(map + ptrs_to_old->Ehdr->e_phoff);
	save_offsets(ptrs_to_update, map, phdr_ptr);
	
	//write existing pheaders to file
	ptrs_to_update->bites_written += write(new_file_fd, phdr_ptr, (phdr_num * sizeof(Elf64_Phdr)));

	//save generic offsets
	save_offsets(ptrs_to_update, map, phdr_ptr);
	//save offsets exclusive for the new phdr
	ptrs_to_update->p_memsz = (uint64_t)&phdr_ptr->p_memsz - (uint64_t)map + (phdr_num * sizeof(Elf64_Phdr));
	ptrs_to_update->p_filesz = (uint64_t)&phdr_ptr->p_filesz - (uint64_t)map + (phdr_num * sizeof(Elf64_Phdr));

	write_new_phdrs(ptrs_to_update, new_file_fd);
}

void	save_offsets(t_offsets *ptrs_to_update, void *map, Elf64_Phdr* phdr_ptr)
{
	//what to keep?
	ptrs_to_update->p_align = (uint64_t)&phdr_ptr->p_align - (uint64_t)map;

	ptrs_to_update->p_filesz = (uint64_t)&phdr_ptr->p_filesz - (uint64_t)map;
	
	ptrs_to_update->p_memsz = (uint64_t)&phdr_ptr->p_memsz - (uint64_t)map;
	
	ptrs_to_update->p_vaddr = (uint64_t)&phdr_ptr->p_vaddr - (uint64_t)map;
	
	ptrs_to_update->p_paddr = (uint64_t)&phdr_ptr->p_paddr - (uint64_t)map;
	
	ptrs_to_update->p_offset = (uint64_t)&phdr_ptr->p_offset - (uint64_t)map;


}

void write_new_phdrs(t_offsets *ptrs_to_update, int new_file_fd)
{
	Elf64_Phdr	new_phdr;

	bzero(new_phdr, sizeof(Elf64_Phdr));//TODO: change to ft
	new_phdr.p_type   = PT_LOAD;
    new_phdr.p_flags  = PF_R | PF_X;
	//this is all we know from the beggining lmao so now we will write to file
	ptrs_to_update->bites_written += write(new_file_fd, &new_phdr, sizeof(Elf64_Phdr));
}