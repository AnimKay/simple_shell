#include "shell.h"

/**
 * main - main function and entry point
 * @ac: count of arg
 * @av: arg vector
 *
 * Return: 0 on success, 1 if failed
 */
int main(int ac, char **av)
{
	info_t info[] = { INFO_INIT };
	int bk = 2;

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (bk)
		: "r" (bk));

	if (ac == 2)
	{
		bk = open(av[1], O_RDONLY);
		if (bk == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_eputs(av[0]);
				_eputs(": 0: Can't open ");
				_eputs(av[1]);
				_eputchar('\n');
				_eputchar(BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		info->readbk = bk;
	}
	populate_env_list(info);
	read_history(info);
	hsh(info, av);
	return (EXIT_SUCCESS);
}
