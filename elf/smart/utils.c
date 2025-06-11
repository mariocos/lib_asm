#include "packer.h"

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
	void	*map = mmap(NULL, lseek(fd, 0, SEEK_END), PROT_READ, MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED)
	{
		write(2, "problem mapping file\n", 21);
		close(fd);
		return (NULL);
	}
	return (map);
}

int	get_new_file_fd(char *str)
{
	char new_file_name[6] = "woody";
	int fd = open(new_file_name, O_WRONLY | O_CREAT | O_APPEND, 0644cl);
	return (fd);
}