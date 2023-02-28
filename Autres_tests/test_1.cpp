/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_1.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averon <averon@student.42Mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:18:42 by averon            #+#    #+#             */
/*   Updated: 2023/02/21 14:24:11 by averon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// extract to string
#include <iostream>
#include <fstream>
#include <string>

int main ()
{
  std::string name;
  std::ifstream ifs("test_file");

  std::cout << "Please, enter your full name: ";
  std::getline (ifs,name);
  std::cout << "Hello, " << name << "!\n";

  return 0;
}