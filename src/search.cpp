/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 17:04:39 by faventur          #+#    #+#             */
/*   Updated: 2023/02/14 18:41:35 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int	search(std::string target, char opening, char closure, std::vector<std::string> *arr)
{
	std::ifstream	inFlux("config/local.conf");
	std::string		buffer;
	bool			target_lookup(false);
	size_t			op(0);
	size_t			cl(0);
	size_t			line(0);

	if (!inFlux)
	{
		std::cerr << "Error: impossible to open the config file." << std::endl;
		return (-1);
	}
	while (getline(inFlux, buffer))
	{
		std::string::size_type	t_pos = buffer.find(target);
		line++;
		if (t_pos != std::string::npos)
		{
			target_lookup = true;
		}
		if (target_lookup)
		{
			std::string::size_type	pos = buffer.find(opening);
			if (pos != std::string::npos)
			{
				std::cout << line << std::endl;
				op++;
			}
			if (op >= 1)
			{
				arr->push_back(buffer);
			}
			std::string::size_type	pos2 = buffer.find(closure);
			if (pos2 != std::string::npos)
			{
				std::cout << line << std::endl;
				cl++;
			}
			if (op >= 1 && op == cl)
				break ;
		}
	}
	if (!target_lookup || arr->empty())
		return 1;
	return (0);
}

int	main()
{
	std::vector<std::string>	arr;

	std::cout << search("https", '{', '}', &arr) << std::endl;
	for (std::vector<std::string>::iterator	first = arr.begin(); first != arr.end(); ++first)
		std::cout << *first << std::endl;
}
