/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 15:13:31 by Henriette         #+#    #+#             */
/*   Updated: 2024/09/10 17:10:30 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cleanup_and_exit(t_global *global)
{
	int	exit_status;

	exit_status = global->exit_status;
	close(global->history_fd);
	if (global->filenames)
		remove_heredocs(global);
	rl_clear_history();
	free_env_list(&global->env_list);
	free_array(global->env);
	global->env = NULL;
	free(global->prompt);
	free(global->exec);
	if (global->stdin_cp != -1)
	{
		close(global->stdin_cp);
		global->stdin_cp = -1;
	}
	if (global->stdout_cp != -1)
	{
		close(global->stdout_cp);
		global->stdout_cp = -1;
	}
	free(global);
	exit(exit_status);
}

void	shell_exit(t_global *global)
{
	ft_putstr_fd("exit\n", 1);
	cleanup_and_exit(global);
}

void	free_command(t_input **command)
{
	t_input	*temp;

	temp = *command;
	while (*command != NULL)
	{
		temp = *command;
		*command = (*command)->next;
		free_array(temp->words);
		temp->words = NULL;
		free_array(temp->redirections);
		temp->redirections = NULL;
		free_array(temp->heredoc);
		temp->heredoc = NULL;
		free(temp->exp_word);
		temp->exp_word = NULL;
		free(temp->exp_redir);
		temp->exp_redir = NULL;
		free(temp->types);
		temp->types = NULL;
		free(temp);
		temp = NULL;
	}
	*command = NULL;
}

/*void	free_command(t_input **command)
{
	t_input	*temp;

	temp = *command;
	while (*command != NULL)
	{
		temp = *command;
		*command = (*command)->next;
		free_array(temp->words);
		free_array(temp->redirections);
		free_array(temp->heredoc);
		if (temp->exp_word)
			free(temp->exp_word);
		if (temp->exp_redir)
			free(temp->exp_redir);
		if (temp->types)
			free(temp->types);
		if (temp)
			free(temp);
		temp->exp_word = NULL;
		temp->exp_redir = NULL;
		temp->types = NULL;
		temp = NULL;
	}
	*command = NULL;
}*/
void	free_array(char **str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return ;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
	str = NULL;
}

int	error_return(char *message)
{
	char	*line;

	line = ft_strjoin("minishell: ", message);
	if (!line)
	{
		ft_putstr_fd("error allocating perror line", 2);
		return (1);
	}
	perror(line);
	free(line);
	return (1);
}
