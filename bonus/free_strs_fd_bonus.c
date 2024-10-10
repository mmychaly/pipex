/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_strs_fd_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmychaly <mmychaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 03:56:22 by mmychaly          #+#    #+#             */
/*   Updated: 2024/10/10 03:22:08 by mmychaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	free_fault_execve(char **strs, char *cmd)
{
	int	i;

	i = 0;
	while (strs[i] != NULL)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
	strs = NULL;
	free(cmd);
	cmd = NULL;
	free_pipe(0);
	perror("ERROR execve");
	exit(126);
}

void	ft_free_strs(char **strs)
{
	int	i;

	i = 0;
	while (strs[i] != NULL)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
	strs = NULL;
}

void	ft_error_exit(int nb)
{
	perror("ERROR");
	if (nb == 0)
		exit(0);
	else
		exit(EXIT_FAILURE);
}

void	free_pipe(int fd)
/*Очень важно при незапланированном прекращении процесса ,
 считывать информацию из прошлого пайпа куда сохранились результат работы прошлой команды, 
 иначе будет ошибка.*/
{
	char	*line;

	line = get_next_line(fd);
	while (line != NULL)
	{
		free(line);
		line = get_next_line(fd);
	}
}

void	error_split(void)
{
	perror("ERROR split: ");
	free_pipe(0);
	exit(EXIT_FAILURE);
}
