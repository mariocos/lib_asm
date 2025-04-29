#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

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
	printf("the file is of size [%ld]\n", f_size);
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

unsigned char shellcode[] = {0xbb, 0x00, 0x00, 0x00, 0x00, 0xb9, 0x00, 0x00, 0x00, 0x00, 0x48, 0x83, 0xfb, 0x0a, 0x7f, 0x0e, 0x48, 0x83, 0xc3, 0x01, 0x48, 0x83, 0xc1, 0x02, 0x48, 0x83, 0xfb, 0x0a, 0x7c, 0xec, 0xb8, 0x3c, 0x00, 0x00, 0x00, 0x48, 0x89, 0xcf, 0x0f, 0x05};

void check_placement_of_header_tables(void *map, Elf64_Ehdr *ehdr)
{
	long space_amount;
	Elf64_Shdr *shdr_table_start = (Elf64_Phdr*)(map + ehdr->e_shoff);
	Elf64_Shdr *shdrs = (Elf64_Phdr*)(map + ehdr->e_shoff);

	Elf64_Phdr *phdr_table_start  = (Elf64_Phdr*)(map + ehdr->e_phoff);
	Elf64_Phdr *phdrs = (Elf64_Phdr*)(map + ehdr->e_phoff);


	printf("wich header table comes first: [%s]\n",(shdrs < phdrs) ? "sections!" : "programs:(" );
	printf("yes? [%d]\n",(void*)(&phdrs[ehdr->e_phnum] + 64) - (void*)shdrs);
	for (short i = 0; i < (short)ehdr->e_shnum; i++)
	{
		if (shdrs[i].sh_offset > shdr_table_start || shdrs[i].sh_offset > phdr_table_start)
		{
			printf("start of section comes after header tables\n");
		}
	}
	for (short i = 0; i < (short)ehdr->e_phnum; i++)
	{
		if (phdrs[i].p_offset > shdr_table_start || phdrs[i].p_offset > phdr_table_start)
		{
			printf("start of segment comes after header tables\n");
		}
	}
}


int main(void)
{
	void *map = get_map("a.out");

	print_header(map, 64);
	Elf64_Ehdr	*header = (Elf64_Ehdr *)map;
	Elf64_Shdr	*section_headers = (Elf64_Shdr *)(map + header->e_shoff);

	int	sheader_nbr = header->e_shnum;
	int sheader_size = header->e_shentsize;


	//int text_ind = get_text_section_index(map, header);
	//Elf64_Shdr	*text_sheader = (Elf64_Shdr *)&section_headers[text_ind];
	check_placement_of_header_tables(map, header);


}
