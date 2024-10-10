/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_launch_child_other_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmychaly <mmychaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 02:03:48 by mmychaly          #+#    #+#             */
/*   Updated: 2024/10/10 03:14:08 by mmychaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	ft_redirection_between(int pipefd, t_data *data)
{
	if (dup2(data->prev_pipe, STDIN_FILENO) == -1)//Конец чтения из прошлого пайпа
	{
		perror("Error: dup2 prev_pipe");
		free_pipe(data->prev_pipe);
		/*Если что то пойдет не так нужно освободить данные записанные в прошлый пайп, иначе ошибка*/
		close(data->prev_pipe);
		close(pipefd);
		exit (EXIT_FAILURE);
	}
	close(data->prev_pipe);
	/*Закрываем конец чтения из прошлого пайпа , копия для дочернего процесса.
	В родительском процессе мы так же закроем этот конец после завершения этого дочернего процесса.*/
	if (dup2(pipefd, STDOUT_FILENO) == -1)//Конец записи в текущий пайп 
	{
		perror("Error: dup2 pipefd[1]");
		free_pipe(0);
		close(pipefd);
		exit (EXIT_FAILURE);
	}
	close(pipefd);
}

void	ft_launch_other(t_data *data, int pipefd[2])
{
	char	**strs_argv;
	char	*cmd;

	close(pipefd[0]);
	ft_redirection_between(pipefd[1], data);
	/*ft_redirection_between Переводим стандартный ввод на конец чтения из прошлого пайпа, 
	вывод в конец записи текущего пайп*/
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
		free_error_cmd(strs_argv); //Освобождаем strs_argv а так же конец чтения из пайпа прошлого родителького процесса.
	if (execve(cmd, strs_argv, data->envp) == -1)
		free_fault_execve(strs_argv, cmd);
}
