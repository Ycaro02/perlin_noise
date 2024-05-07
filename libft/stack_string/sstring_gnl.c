#include "stack_string.h"

/**
 * @brief Read in sstring
 * @param sstr Pointer on the stack string to read in
 * @param fd File descriptor
 * @return 1 if success, else 0
 */
static u8 sstring_read(t_sstring *sstr, int fd)
{
	int     read_result = 1;

	empty_sstring(sstr);
	if (fd < 0 || !sstr)
		return (0);
	read_result = read(fd, sstr->data, SSTRING_MAX - 1);
	if (read_result <= 0) {
		return (0);
	}
	return (1);
}

char **sstring_load_file(char *path)
{
	t_sstring	line;
	u8			ret_read = 1;
	char		*map = NULL, **dest = NULL;
	int 		fd = -1;	
	
	fd = open(path, O_RDONLY);
	if (fd < 0) {
		ft_printf_fd(2, "Error\nfd incorect \n");
		return (NULL);
	}
	clear_sstring(&line);
	map = ft_strdup("");
	while (ret_read) {
		ret_read = sstring_read(&line, fd);
		map = ft_strjoin_free(map, line.data, 'f');
		clear_sstring(&line);	
		if (ret_read == 0){
			break ;
		}
		// ft_printf_fd(1, ORANGE"map|%s|"RESET,map);
	}

	ft_printf_fd(1, YELLOW"In sstring load File %s:"RESET"\n"PINK"|%s|"RESET,path,map);
	close(fd);
	dest = ft_split_trim(map, '\n');
	free(map);
	return (dest);
}


// u32 sstring_search(t_sstring *sstr, char c)
// {
// 	if (!sstr)
// 		return (0);
// 	for (uint16_t i = 0; i < sstr->size; ++i) {
// 		if (sstr->data[i] == c)
// 			return (i);
// 	}
// 	return (OUT_OF_SSTRING);
// }


// void sstring_rm_first_word(t_sstring *sstr, u32 end)
// {
// 	t_sstring tmp;

// 	empty_sstring(&tmp);

// 	if ((sstr->size <= end - 1)) {
// 		clear_sstring(sstr);
// 		return;
// 	}

// 	tmp = fill_sstring(sstr->data + end + 1);
// 	clear_sstring(sstr);
// 	ft_printf_fd(1, "tmp: %s\n", tmp.data);
// 	*sstr = fill_sstring(tmp.data);

// }