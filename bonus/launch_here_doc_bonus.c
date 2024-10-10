/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_here_doc_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmychaly <mmychaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 21:17:06 by mmychaly          #+#    #+#             */
/*   Updated: 2024/10/10 03:27:36 by mmychaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	launch_here_doc(char **argv, int pipefd[2])//pipex here_doc
{
	char	*line;

	line = NULL;
	close(pipefd[0]);
	line = get_next_line(0);
	while (line != NULL)//Считываем с командной строки бесконечно
	{
		if ((ft_strncmp(argv[2], line, ft_strlen(line) - 1)) == 0)
		/*Но если считаная строка == стоп слову то выходим из цикла */
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));//Считаную строку записываем в пайп 
		free(line);
		line = get_next_line(0);
	}
	close(pipefd[1]);
	exit(0);//Когда закончили считывать с командой строки закрываем дочерний процесс.
}
