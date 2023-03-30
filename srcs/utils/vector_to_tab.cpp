/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_to_tab.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stissera <stissera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 12:03:43 by stissera          #+#    #+#             */
/*   Updated: 2023/03/30 12:12:45 by stissera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.hpp"

char	**ft::vector_to_tab(std::vector<std::string>& env)
{
	size_t i = 0;
	char **ret = new char*[env.size() + 1];
	for (; i < env.size(); i++)
	{
		ret[i] = new char[env[i].size() + 1];
		std::strcpy(ret[i],env[i].c_str());
	}
	ret[i] = 0;
	return (ret);
}