/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 17:04:39 by faventur          #+#    #+#             */
/*   Updated: 2023/02/13 17:04:48 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
/*					work in progress
static int	ft_str_quote_checker_pt2(int *sq, int *dq)
{
	int	ret;

	if (!*sq && !*dq)
		ret = 0;
	else if (*sq && !*dq)
		ret = 1;
	else
		ret = 2;
	return (ret);
}

static int	ft_str_quote_checker(char *str, size_t *index)
{
	size_t	i;
	int		sq;
	int		dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (str[i] && i < *index)
	{
		if (str[i] == '\'' && !sq && !dq)
			sq++;
		else if (str[i] == '\'' && sq && !dq)
			sq--;
		else if (str[i] == '\"' && !dq && !sq)
			dq++;
		else if (str[i] == '\"' && !sq && dq)
			dq--;
		i++;
	}
	return (ft_str_quote_checker_pt2(&sq, &dq));
}

static int	ft_check_quotes(char *str)
{
	size_t	index;

	index = ft_strlen(str);
	if (index > 0)
		return (ft_str_quote_checker(str, &index));
	return (0);
}

int	search(std::string target, char opening, char closing)
{
	std::ifstream	inFlux("config/local.conf");
	std::string		buffer;

	if (!inFlux)
	{
		std::cerr << "Error: impossible to open the config file." << std::endl;
		return (1);
	}
	std::vector<std::string>	arr;

	while (getline(inFlux, buffer))
	{
		std::string::size_type	i(0);
		std::string::size_type	pos = buffer.find(target);
		if (pos != std::string::npos)
		{

			outFlux << s2;
			i += s1.length();
			pos = buffer.find(s1, pos + 1);
			while (i < pos && buffer[i] != '\0')
			{
				outFlux.put(buffer[i]);
				i++;
			}
			if (buffer[i] == '\0')
				outFlux.put('\n');

		}
	}

	return 0;
}
*/