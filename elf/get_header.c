#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
/*

typedef struct elf64_hdr {
unsigned char e_ident[EI_NIDENT]; ELF "magic number" 
Elf64_Half e_type;
Elf64_Half e_machine;
Elf64_Word e_version;
Elf64_Addr e_entry;       Entry point virtual address 
Elf64_Off e_phoff;        Program header table file offset
Elf64_Off e_shoff;        Section header table file offset
Elf64_Word e_flags;
Elf64_Half e_ehsize;
Elf64_Half e_phentsize;
Elf64_Half e_phnum;			Program header number
Elf64_Half e_shentsize;
Elf64_Half e_shnum;			Section header number
Elf64_Half e_shstrndx;
} Elf64_Ehdr;

typedef struct elf64_phdr {
  255   Elf64_Word p_type;
  256   Elf64_Word p_flags;
  257   Elf64_Off p_offset;       Segment file offset 
  258   Elf64_Addr p_vaddr;       Segment virtual address 
  259   Elf64_Addr p_paddr;       Segment physical address
  260   Elf64_Xword p_filesz;     Segment size in file 
  261   Elf64_Xword p_memsz;      Segment size in memory
  262   Elf64_Xword p_align;      Segment alignment, file & memory 
  263 } Elf64_Phdr;

 typedef struct elf64_shdr {
  313   Elf64_Word sh_name;       Section name, index in string tbl 
  314   Elf64_Word sh_type;       Type of section 
  315   Elf64_Xword sh_flags;     Miscellaneous section attributes 
  316   Elf64_Addr sh_addr;       Section virtual addr at execution 
  317   Elf64_Off sh_offset;      Section file offset 
  318   Elf64_Xword sh_size;      Size of section in bytes 
  319   Elf64_Word sh_link;       Index of another section 
  320   Elf64_Word sh_info;       Additional section information 
  321   Elf64_Xword sh_addralign; Section alignment 
  322   Elf64_Xword sh_entsize;   Entry size if section holds table 
  323 } Elf64_Shdr;


*/
#define NEW_SECTION_NAME ".inject"

#define FILE "skip"

unsigned char jumpShellCode[] = {
    0x48, 0xb8,                  // movabs rax
    0xef, 0xbe, 0xad, 0xde,      // lower 4 bytes (little endian)
    0xbe, 0xba, 0xfe, 0xca,      // upper 4 bytes (adjust this!)
    0xff, 0xe0                   // jmp rax
};

unsigned char baseShellCode[] = {0xb8, 0x41, 0x00, 0x00, 0x00, 0x50, 0xb8, 0x01, 0x00, 0x00, 0x00, \
	0xbf, 0x01, 0x00, 0x00, 0x00, 0x48, 0x89, 0xe6, 0xba, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x05};

unsigned char shellcode[sizeof(baseShellCode) + sizeof(jumpShellCode)];


size_t size_of_file = 0;

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
			if (!ft_strcmp(sh_strtable + shdrs[i].sh_name, ".text"))
			{
				printf("text at .text: ");
				for (unsigned long i = 0; i <= shdrs[i].sh_size; i++)
					printf("0x%02x ", (unsigned char*)(map + shdrs[i].sh_offset + i));
				printf("\n");
				return (i);
			}
		}
	}
	return (-1);
}


void	inspection(Elf64_Ehdr *header, Elf64_Shdr *section_headers,  void *map, Elf64_Shdr *text_sheader, int text_ind)
{
	Elf64_Addr	entry_point = header->e_entry; // get entry-point of elf, aka where code starts to run
	print_header((map + text_sheader->sh_offset), text_sheader->sh_size);
	printf("Original entry point: 0x%lx\n", entry_point); // testing just to make sure it can be found
	
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
	int fd = open(FILE, O_RDONLY);
	if (fd < 0)
	{
		write(2, "problem opening file\n", 21);
		return (NULL);
	}
	off_t f_size = lseek(fd, 0, SEEK_END);
	size_t new_size = f_size + sizeof(shellcode) + sizeof(Elf64_Shdr);

	int new_fd = open("new_file", O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (new_fd < 0)
	{
		write(2, "problem creating file\n", 23);
		return (NULL);
	}

	if (lseek(new_fd, new_size, SEEK_SET) == -1 || write(new_fd, "", 1) != 1)
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
		close(fd);
		return NULL;
	}
	memcpy(new_map, old_map, f_size);
	
	msync(new_map, new_size, MS_SYNC);
	// munmap(new_map, new_size);
	close(fd);
	
	return (new_map);
}

void update_phdr(void *map, Elf64_Ehdr *ehdr, Elf64_Shdr *new_shdr) 
{
    Elf64_Phdr *phdr = (Elf64_Phdr *)(map + ehdr->e_phoff);

	Elf64_Addr inject_shellcode_virtual_address; // added for more code clarity, TODO: take it off maybe?
	Elf64_Off inject_shellcode_offset; // added for more code clarity, TODO: take it off maybe?

	// For debug purposes
	Elf64_Shdr *shdr = (Elf64_Shdr *)(map + ehdr->e_shoff);
	char *shstrtab = (char *)(map + shdr[ehdr->e_shstrndx].sh_offset);

	short	section_nbr = ehdr->e_shnum;

    for (int i = section_nbr; i > 0; i--) {
        if (phdr[i].p_type == PT_LOAD && (phdr[i].p_flags & PF_X)) {


            inject_shellcode_virtual_address = (phdr[i].p_vaddr + phdr[i].p_memsz + 0xF) & ~0xF;
            inject_shellcode_offset = (phdr[i].p_offset + phdr[i].p_filesz + 0xF) & ~0xF;

            new_shdr->sh_addr = inject_shellcode_virtual_address;
            new_shdr->sh_offset = inject_shellcode_offset;

            phdr[i].p_memsz = (inject_shellcode_virtual_address + sizeof(shellcode)) - phdr[i].p_vaddr;
			phdr[i].p_filesz = (inject_shellcode_offset + sizeof(shellcode)) - phdr[i].p_offset;
			
            phdr[i].p_flags |= PF_X; // Ensure it's executables
            break;
        }
    }
}

/* adds jump to original entry to the shellcode */
void addJump(Elf64_Addr ogEntry)
{
	uint64_t orig_entry = ogEntry;
	memcpy(jumpShellCode + 2, &orig_entry, sizeof(orig_entry));

	memcpy(shellcode, baseShellCode, sizeof(baseShellCode));
	memcpy(shellcode + sizeof(baseShellCode), jumpShellCode, sizeof(jumpShellCode));
}


void	inject_new_header(void *map)
{
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)map;
	Elf64_Shdr *shdr = (Elf64_Shdr *)(map + ehdr->e_shoff);
	Elf64_Addr ogEntry = ehdr->e_entry;

	addJump(ogEntry);

	off_t shellcode_offset = size_of_file - sizeof(shellcode) - sizeof(Elf64_Shdr);
	
		// this should be sec of map + offset till section headers + number of section headers * size (typically 64 cuz 64ELF)
	Elf64_Shdr *new_shdr = (Elf64_Shdr *)(map + ehdr->e_shoff + (ehdr->e_shnum * sizeof(Elf64_Shdr)));
	memset(new_shdr, 0, sizeof(Elf64_Shdr));

	new_shdr->sh_name = 0;
    new_shdr->sh_type = SHT_PROGBITS;
    new_shdr->sh_flags = SHF_ALLOC | SHF_EXECINSTR;
    new_shdr->sh_size = sizeof(shellcode);
    new_shdr->sh_addralign = 0x10;

	update_phdr(map, ehdr, new_shdr);

	memcpy(map + new_shdr->sh_offset, shellcode, sizeof(shellcode));

	ehdr->e_shnum += 1;
	ehdr->e_entry = new_shdr->sh_addr;

}

int main(void)
{
	void *map = get_map(FILE);

	// print_header(map, 64);
	Elf64_Ehdr	*header = (Elf64_Ehdr *)map;
	Elf64_Shdr	*section_headers = (Elf64_Shdr *)(map + header->e_shoff);

	int	sheader_nbr = header->e_shnum;
	int sheader_size = header->e_shentsize;


	int text_ind = get_text_section_index(map, header);
	Elf64_Shdr	*text_sheader = (Elf64_Shdr *)&section_headers[text_ind];
	// printf("text section offset [%p] and size [%d]\n", text_sheader->sh_offset, text_sheader->sh_size);

	// vera shenanigans
	inspection(header, section_headers, map, text_sheader, text_ind);
	void *new_map = create_new_file(map);
	inject_new_header(new_map);

	printf("text section");
	Elf64_Ehdr	*new_header = (Elf64_Ehdr *)new_map;
	Elf64_Shdr	*new_section_headers = (Elf64_Shdr *)(new_map + new_header->e_shoff);

	int	new_sheader_nbr = new_header->e_shnum;
	int new_sheader_size = new_header->e_shentsize;


	int new_text_ind = get_text_section_index(new_map, new_header);
	Elf64_Shdr	*new_text_sheader = (Elf64_Shdr *)&new_section_headers[new_text_ind];
	printf("text section offset [%p] and size [%d]\n", new_text_sheader->sh_offset, new_text_sheader->sh_size);
	inspection(new_header, new_section_headers, new_map, new_text_sheader, new_text_ind);

	// check_placement_of_header_tables(map, header);
}
