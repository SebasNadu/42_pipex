/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 11:24:25 by sebasnadu         #+#    #+#             */
/*   Updated: 2023/10/23 21:10:29 by johnavar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/includes/libft.h"
# include "../libft/includes/ft_printf.h"

# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>

# define HERE_DOC_PATH ".pipex_here_doc"
# define URANDOM_PATH ".pipex_urandom"
# define NO_INFILE ".pipex_tmp"

enum e_pipex_err
{
	NO_ERR = 0,
	END = 0,
	CMD_NOT_FOUND = 127,
	NO_FILE = -2,
	NO_AUTH = 126,
	INV_ARGS = -4,
	NO_MEMORY = -5,
	PIPE_ERR = -6,
	DUP_ERR = -7,
	FORK_ERR = -8,
	NO_PATH = -9,
	CMD_FAIL = -10,
	NO_READ = -11,
	NO_WRITE = -12,
	NO_EXEC = -13,
};

typedef enum e_bool
{
	false,
	true
}				t_bool;

typedef struct s_pipex
{
	int		fd_in;
	int		fd_out;
	t_bool	here_doc;
	t_bool	is_urandom;
	t_bool	is_in_cpy;
	char	**cmd_paths;
	char	***cmd_args;
	int		cmd_count;
}				t_pipex;

// exit.c
void	*pipex_exit(t_pipex *pipex, char *param, int err);
void	pipex_perror(char *param, int err);
void	free_array(char **array, int size);
void	broken_pipe_perror(t_pipex *pipex, int i);
void	*broken_pipe_exit(t_pipex *pipex, int err);
// parse.c
t_bool	parse_args(int argc, char **argv, t_pipex *pipex);
t_bool	parse_cmd_paths(t_pipex *pipex, int argc, char **argv, char **envp);
t_bool	parse_cmd_args(t_pipex *pipex, int argc, char **argv);
// get_io.c
t_bool	get_infile(t_pipex *pipex, char **argv);
t_bool	get_outfile(t_pipex *pipex, char **argv, int argc);
// get_paths.c
char	*find_cmd_path(char *cmd, char **envp);
// pipex.c
void	pipex_controller(t_pipex *pipex, char **envp);
int		exec_pipex(t_pipex *pipex, char **envp, int i);
void	init_pipex(t_pipex *pipex);
// split.c
char	**split_with_quotes(char *str, char delimiter);

#endif
