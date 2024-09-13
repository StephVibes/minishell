/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:51:05 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/04 15:42:44 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	error_identifier(char *str, char *command)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

static int	is_syntax_ok(char *str)
{
	int	i;

	i = 0;
	if (ft_isdigit(str[0]))
		return (0);
	while (str[i] != '\0')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	unset_env(char *key, t_env *env_list)
{
	t_env	*current;
	t_env	*prev;

	current = env_list;
	prev = NULL;
	while (current)
	{
		if (!ft_strncmp(current->key, key, ft_strlen(key))
			&& ft_strlen(current->key) == ft_strlen(key))
		{
			if (prev)
				prev->next = current->next;
			else
				env_list = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	unset(char **args, t_global *global)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (is_syntax_ok(args[i]))
		{
			unset_env(args[i], global->env_list);
			set_env_array(global->env_list, &global->env);
			global->exit_status = 0;
		}
		else
		{
			error_identifier(args[i], "unset");
			global->exit_status = 1;
		}
		i++;
	}
	return ;
}
