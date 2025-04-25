#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

// in-line to align the offset of the new inject code to a 4096 aka 0x1000 usual padding in between sections
#define ALIGN_UP(x, align) (((x) + ((align)-1)) & ~((align)-1)) //not too sure of whats going on w the ~ tho

#define NEW_SECTION_NAME ".inject"

#define FILE "skip"

unsigned char shellcode[] = {0xbb, 0x00, 0x00, 0x00, 0x00, 0xb9, 0x00, 0x00, 0x00, 0x00, 0x48,\
	0x83, 0xfb, 0x0a, 0x7f, 0x0e, 0x48, 0x83, 0xc3, 0x01, 0x48, 0x83, 0xc1, 0x02, \
	0x48, 0x83, 0xfb, 0x0a, 0x7c, 0xec, 0xb8, 0x3c, 0x00, 0x00, 0x00, 0x48, 0x89, \
	0xcf, 0x0f, 0x05};

int	ft_strcmp(const char *s1, const char *s2)
{
	if (s1 == NULL || s2 == NULL)
		return (0);
	while (*s1 && *s2 && (*s1 == *s2))
	{
		if (!*s1 || !*s2)
			break ;
		s1++;
		s2++;
	}
	return (*(const unsigned char *)s1 - *(const unsigned char *)s2);
}

void	*get_map(char *str)
{
	if (!str)
		return (NULL);
	int fd = open(str, O_RDONLY);
	if (fd < 0)
	{
		write(2, "problem opening file\n", 21);
		return (NULL);
	}
	off_t f_size = lseek(fd, 0, SEEK_END);
	void	*map = mmap(NULL, f_size, PROT_READ, MAP_PRIVATE, fd, 0); 
	if (map == MAP_FAILED)
	{
		write(2, "problem mapping file\n", 21);
		close(fd);
		return (NULL);
	}
	return (map);
}

int	ft_isprint(int c)
{
	if ((c <= 31 && c >= 0) || c == 127)
		return (0);
	return (1);
}

void	ft_print_hex(unsigned char c)
{
	write(1, &"0123456789abcdef"[c / 16], 1);
	write(1, &"0123456789abcdef"[c % 16], 1);
}

void	print_header(char *map, int size)
{
	write(1, "made\n", 5);
	for (int i = 0 ; i < size ; i++)
	{
		write(1, "0x", 2);
		ft_print_hex(map[i]);
		write(1, ", ", 2);
	}
}

void	iterate_sheaders(void *map, Elf64_Ehdr *eheader)
{
	Elf64_Shdr *sheader = (Elf64_Shdr *)(map + eheader->e_shoff);
	int	str_ind = eheader->e_shstrndx;
	printf("the strheader index is %d out of %d\n", str_ind, eheader->e_shnum);
	printf("section size is %d\n",sheader[str_ind].sh_size);
	print_header(map + sheader[str_ind].sh_offset, sheader[str_ind].sh_size);
}

int get_text_section_index(void *map, Elf64_Ehdr *eheader)
{
	int	section_nbr = eheader->e_shnum;
	Elf64_Shdr *shdrs = (Elf64_Shdr *)(map + eheader->e_shoff);
	Elf64_Shdr *sh_table = &shdrs[eheader->e_shstrndx];
	int	sh_table_offset = sh_table->sh_offset;
	char *sh_strtable = map + shdrs[eheader->e_shstrndx].sh_offset;


	for (int i = 0 ; i < section_nbr ; i++)
	{
		if (i != eheader->e_shstrndx)
		{
			printf("name of section [%s]\n", (sh_strtable + shdrs[i].sh_name ));
			if (!ft_strcmp(sh_strtable + shdrs[i].sh_name, ".text"))
				return (i);
		}
	}
	return (-1);
}


void	inspection(Elf64_Ehdr *header, Elf64_Shdr *section_headers,  void *map, Elf64_Shdr *text_sheader, int text_ind)
{
	Elf64_Addr	entry_point = header->e_entry; // get entry-point of elf, aka where code starts to run
	print_header((map + text_sheader->sh_offset), text_sheader->sh_size);
	printf("Original entry point: 0x%lx\n", entry_point); // testing just to make sure it can be found
	/* 
	TODO: check to see if there is at least 100bytes of free space after the section header
	*/
	Elf64_Shdr next_section = section_headers[text_ind + 1]; // (not sure about this naming?)
	printf(".text file offset: 0x%lx\n", text_sheader->sh_offset);
	printf(".text next section offset: 0x%lx\n", next_section.sh_offset);
	printf(".text size: 0x%x\n", text_sheader->sh_size);
	size_t space = next_section.sh_offset - (text_sheader->sh_offset + text_sheader->sh_size);
	printf("Available padding after .text: %lu bytes\n", space);

	// checkers to see if the file has been stripped completely (meaning there is no section headers)
	// if stuff is at 0 it means it has been stripped, otherwise its healthy
	printf("Section header offset: 0x%lx\n", header->e_shoff);
	printf("Number of section headers: %d\n", header->e_shnum);
	printf("Section header string table index: %d\n", header->e_shstrndx);

	// checker to dump all of the sections and make sure there is no missunderstanding with the .text
	// this should verify if the index function is working properly
	Elf64_Shdr *section_headers_checkers = (Elf64_Shdr *)(map + header->e_shoff);
	const char *sh_strtab = map + section_headers_checkers[header->e_shstrndx].sh_offset;
	const char *name = NULL;

	for (int i = 0; i < header->e_shnum; i++) {
		name = sh_strtab + section_headers_checkers[i].sh_name;
		printf("Section [%2d]: %-16s offset: 0x%06lx size: %06d available space: %lu \n",
	    	i, name, section_headers_checkers[i].sh_offset, section_headers_checkers[i].sh_size, 
			section_headers_checkers[i+1].sh_offset - (section_headers_checkers[i].sh_offset + \
				section_headers_checkers[i].sh_size));
	}
}

void *create_new_file(void *old_map)
{
	int fd = open(str, O_RDONLY);
	if (fd < 0)
	{
		write(2, "problem opening file\n", 21);
		return (NULL);
	}
	off_t f_size = lseek(fd, 0, SEEK_END);
	size_t new_size = f_size + sizeof(shellcode) + 5;

	int new_fd = open("new_file", O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
	{
		write(2, "problem creating file\n", 24);
		return (NULL);
	}

	if (lseek(new_fd, new_size - 1, SEEK_SET) == -1 || write(new_fd, "", 1) != 1)
	{
		perror("extending file");
		close(new_fd);
		close(fd);
		return NULL;
	}

	void *new_map = mmap(NULL, new_size, PROT_READ | PROT_WRITE, MAP_SHARED, new_fd, 0);
	if (new_map == MAP_FAILED)
	{
		perror("mmap");
		close(new_fd);
		free(old_map);
		return NULL;
	}
	memcpy(new_map, old_map, f_size);
	free(old_map);

	return (new_map);
}

int main(void)
{
	void *map = get_map(FILE);

	print_header(map, 64);
	Elf64_Ehdr	*header = (Elf64_Ehdr *)map;
	Elf64_Shdr	*section_headers = (Elf64_Shdr *)(map + header->e_shoff);

	int	sheader_nbr = header->e_shnum;
	int sheader_size = header->e_shentsize;


	int text_ind = get_text_section_index(map, header);
	Elf64_Shdr	*text_sheader = (Elf64_Shdr *)&section_headers[text_ind];
	printf("text section offset [%p] and size [%d]\n", text_sheader->sh_offset, text_sheader->sh_size);

	// vera shenanigans
	inspection(header, section_headers, map, text_sheader, text_ind);
	printf("bro wtf is going on\n");
	create_new_file(map);	

	printf("text section");
	Elf64_Ehdr	*new_header = (Elf64_Ehdr *)new_map;
	Elf64_Shdr	*new_section_headers = (Elf64_Shdr *)(new_map + new_header->e_shoff);

	int	new_sheader_nbr = new_header->e_shnum;
	int new_sheader_size = new_header->e_shentsize;


	int new_text_ind = get_text_section_index(new_map, new_header);
	Elf64_Shdr	*new_text_sheader = (Elf64_Shdr *)&new_section_headers[new_text_ind];
	printf("text section offset [%p] and size [%d]\n", new_text_sheader->sh_offset, new_text_sheader->sh_size);
	inspection(new_header, new_section_headers, new_map, new_text_sheader, new_text_ind);
}
