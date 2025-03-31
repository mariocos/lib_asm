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

void	ft_print_hex(char c)
{
	write(1, &"0123456789abcdef"[c / 16], 1);
	write(1, &"0123456789abcdef"[c % 16], 1);
}

void	print_header(char *map, int size)
{
	for (int i = 0 ; i < size ; i++)
	{
		if (!ft_isprint(map[i]))
		{
			ft_print_hex(map[i]);
			write(1, " ", 1);
		}
		else
			write(1, &map[i], 1);
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

int get_text_section(void *map, Elf64_Ehdr *eheader)
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

int main(void)
{
	void *map = get_map("Hello_world");

	print_header(map, 64);
	Elf64_Ehdr	*header = (Elf64_Ehdr *)map;
	Elf64_Shdr	*section_headers;

	int	sheader_nbr = header->e_shnum;
	int sheader_size = header->e_shentsize;

	int text_ind = get_text_section(map, header);
	printf("the text section index is %d\n", text_ind);	
}