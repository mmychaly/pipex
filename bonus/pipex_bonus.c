/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmychaly <mmychaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 22:27:49 by mmychaly          #+#    #+#             */
/*   Updated: 2024/10/10 03:31:04 by mmychaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	execute_child(t_data *data, int pipefd[2])
{
	if (data->i == 2)
		ft_launch_child_1(data, pipefd);
/*ft_launch_child_1 Отвечает за считывание из файла и передачу резутьтата первой команды в пайп*/
	else if (data->i == data->nbr_cmd + 1)

		ft_launch_child_2(data);
/*ft_launch_child_2 считывание из последнего пайпа и передачу резутьтата в файл вывода*/
	else
		ft_launch_other(data, pipefd);
/*ft_launch_other считывание из прошлого пайпа и передачу резутьтата в текущий пайп.
То есть все команды между первой и последней , запустяться через нее*/
}

void	execute_child_here_doc(t_data *data, int pipefd[2])
{
	if (data->i == 2)
		launch_here_doc(data->argv, pipefd);
/*launch_here_doc считывает данные из командной строки и помещает их в текущий пайп*/
	else if (data->i == data->nbr_cmd + 1)
		ft_launch_child_2(data);
	else
		ft_launch_other(data, pipefd);
}

void	execution(t_data *data)
{
	int	pipefd[2];
	int	pid;

	data->i = 2; // Соответсвует индексу первой команды в argv
	data->prev_pipe = -1; ///Здесь будет храниться конец чтения прошлого пайпа
	while (data->i < data->nbr_cmd + 2) //Выйдем из цикла когда пройдем все команды
	{
		if (data->i != data->nbr_cmd + 1 && pipe(pipefd) == -1) 
		/*Прекратим создавать пайпы когда достигнем последней команды*/
			ft_error_exit(1);
		pid = fork();
		if (pid == -1)
			ft_error_exit(1);
		if (pid == 0)
			execute_child(data, pipefd);//Запускаем дочерний процесс
		if (data->prev_pipe != -1) //Закрываем конец чтения прошлого пайпа.
			close(data->prev_pipe);
		if (data->i != data->nbr_cmd + 1)//Закрываем конец записи текущего пайпа
			close(pipefd[1]);
		if (data->i == data->nbr_cmd + 1)
		/*Когда будем на индексе последней команды запишем в prev_pipe значение pid 
		 используем в wait_processes(data) , что бы понять по завершению какого процесса 
		 выходить из программы.*/
			data->prev_pipe = pid;
		else
			data->prev_pipe = pipefd[0];
			/*Записываем в prev_pipe конец чтения текущего пайпа, эта переменная будет
			 использоваться следующим дочерним процессом для доступа к текущем пайпу , 
			 для дочернего процесса это будет прошлый пайп.*/
		data->i++;
	}
	wait_processes(data);//Для ожидания завершения дочерних процессов 
}

void	execution_here_doc(t_data *data) //Все тоже самое что и функция execution но только для here_doc
{
	int	pipefd[2];
	int	pid;

	data->i = 2;
	data->prev_pipe = -1;
	while (data->i < data->nbr_cmd + 2)
	{
		if (data->i != data->nbr_cmd + 1 && pipe(pipefd) == -1)
			ft_error_exit(1);
		pid = fork();
		if (pid == -1)
			ft_error_exit(1);
		if (pid == 0)
			execute_child_here_doc(data, pipefd);
		if (data->prev_pipe != -1)
			close(data->prev_pipe);
		if (data->i != data->nbr_cmd + 1)
			close(pipefd[1]);
		if (data->i == data->nbr_cmd + 1)
			data->prev_pipe = pid;
		else
			data->prev_pipe = pipefd[0];
		data->i++;
	}
	wait_processes(data);
}

int	main(int argc, char **argv, char *envp[])
{
	t_data	data;

	ft_memset(&data, 0, sizeof(t_data)); //Инициализируем элементы структуры нулями
	data.nbr_cmd = argc - 3; 
	/*data.nbr_cmd == Количество команд //here_doc LIM считаем как 
	команду так как для нее нам нужен 1 пайп */
	data.argv = argv;
	data.envp = envp;
	if (envp == NULL || envp[0][0] == '\0') //Проверяем существует ли окружение или нет
	{
		write(2, "Error: empty envp\n", 18);
		exit(EXIT_FAILURE);
	}
	if ((ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1]))) == 0 && argc >= 6) //Запуск pipex для here_doc
		execution_here_doc(&data);
	else if ((ft_strncmp(argv[1], "here_doc",//Запуск обычного pipex но для неограниченного количества команд
				ft_strlen(argv[1]))) != 0 && argc >= 5)
		execution(&data);
	else
	{
		write(2, "Error: Incorrect number of arguments\n", 37);
		exit(EXIT_FAILURE);
	}
	return (0);
}


/*
./pipex infile.txt sort uniq cat outfile.txt

Порядок запуска основных функций для pipex обычного
-execution
-execute_child
-ft_launch_child_1
-ft_launch_other
-ft_launch_child_2


./pipex here_doc LIM sort uniq cat outfile.txt  //Что бы остановить ввод нужно ввести LIM 

Порядок запуска основных функций для pipex here_doc
-execution_here_doc
-execute_child_here_doc
-launch_here_doc
-ft_launch_other
-ft_launch_child_2
*/