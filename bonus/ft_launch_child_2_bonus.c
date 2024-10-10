/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_launch_child_2_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmychaly <mmychaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 02:32:53 by mmychaly          #+#    #+#             */
/*   Updated: 2024/10/10 03:35:03 by mmychaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	ft_redirection_out(char *argv, t_data *data, int flag)
{
	int	fd_out;

	if (flag == 1)
		fd_out = open(argv, O_WRONLY | O_CREAT | O_APPEND, 0644);
/*pipex c here_doc записывает новые данные в конец файла, модуль O_APPEND*/
	else
		fd_out = open(argv, O_WRONLY | O_TRUNC | O_CREAT, 0644);//Обычный pipex перезаписывает файл
	if (fd_out == -1)
		error_open_outfile(data->prev_pipe);
	if (dup2(fd_out, STDOUT_FILENO) == -1) //Перенаправляем вывод на файл
	{
		perror("dup2 fd_out");
		free_pipe(data->prev_pipe);
		close(data->prev_pipe);
		close(fd_out);
		exit (EXIT_FAILURE);
	}
	close(fd_out);
	if (dup2(data->prev_pipe, STDIN_FILENO) == -1)
	/*Перенаправляем ввод на конец чтения из последнего пайпа*/
		error_dup2_out(data->prev_pipe);
	close(data->prev_pipe);
}

void	ft_launch_child_2(t_data *data)
{
	char	**strs_argv;
	char	*cmd;

	data->flag = 0;//Флаг что бы понять какой из видов пайпа мы будем завершать.
	if ((ft_strncmp(data->argv[1], "here_doc", ft_strlen(data->argv[1]))) == 0) //Если "here_doc" то 1
		data->flag = 1;
	ft_redirection_out(data->argv[data->i + 1], data, data->flag);
	/*ft_redirection_out Читаем из конца чтения прошлого пайпа и записываем в файл */
	if (data->argv[data->i][0] == '\0')
		error_empty_cmd();
	strs_argv = ft_split(data->argv[data->i], ' ');
	if (strs_argv == NULL)
		error_split();
	strs_argv = check_strs(strs_argv, data);
	if (strs_argv == NULL)
		error_cmd();
	if (access(strs_argv[0], F_OK | X_OK) == 0)
		cmd = ft_strdup(strs_argv[0]);
	else
		cmd = ft_envp_cherch(strs_argv[0], data->envp);
	if (cmd == NULL)
		free_error_cmd(strs_argv);
	if (execve(cmd, strs_argv, data->envp) == -1)
		free_fault_execve(strs_argv, cmd);
}
