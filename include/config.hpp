/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faventur <faventur@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 15:03:16 by stissera          #+#    #+#             */
/*   Updated: 2023/02/22 16:35:56 by faventur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <cstring>
#include <vector>
#include <map>

struct	import_config
{
	std::vector<std::multimap<std::string, std::string> >	raw;
};

#endif