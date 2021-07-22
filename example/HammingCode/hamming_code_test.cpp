// Test project for hamming_code library
// For new versions of library see project's github:

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "hamming_code.h"

void get_bin_input(std::vector<bool>* output)
{
	std::cout << "Enter a binary (e.g. 100101): ";
	while (true)
	{
		std::string v_input_string;
		getline(std::cin, v_input_string);

		output->resize(v_input_string.length(), 0);

		bool error_flag = false, bit;
		for (int i = 0; (i < v_input_string.length()) && !error_flag; i++)
		{
			if (v_input_string[i] == '0' || v_input_string[i] == '1')
			{
				std::stringstream str_stream;
				
				str_stream << v_input_string[i];
				str_stream >> bit;
				output->at(i) = bit;
			}
			else error_flag = true;
		}
		if (!error_flag) break;
		else std::cout << "Incorrect input! Try again...\n\n" << std::endl;
	}
}

int main() 
{
	////////////////////////////
	// Demonstrational algorithm
	////////////////////////////
	std::vector<bool> v_input, v_output_code, v_output_decode;
	
	std::cout << "//// Demonstrational Hamming code algorithm ////\n";
	// Getting binary input from user
	get_bin_input(&v_input);

	// Encoding user-entered binary
	hcode::e_hamming_code coder;
	coder.encode_vector(&v_input, &v_output_code);

	// Printing codeword
	std::cout << "Codeword: ";
	for (int i = 0; i < v_output_code.size(); i++)
		std::cout << v_output_code[i] << " ";
	std::cout << std::endl;

	// Making some errors
	while (true)
	{
		int error_bit1, error_bit2;

		std::cout << "Enter number of error-bit position #1 (from 1): ";
		std::cin >> error_bit1;
		std::cout << "Enter number of error-bit position #2 (enter 0, if want a single error): ";
		std::cin >> error_bit2;
		
		if (std::cin.fail() || error_bit1 > v_output_code.size() || error_bit2 > v_output_code.size())
		{
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			std::cout << "Incorrect input! Try again...\n\n";
			continue;
		}
		else
		{
			v_output_code[error_bit1 + 1] = !v_output_code[error_bit1 + 1];
			if(error_bit2) v_output_code[error_bit2 + 1] = !v_output_code[error_bit2 + 1];
			break;
		}
	}

	// Printing decoding result
	if (coder.decode_vector(&v_output_code, &v_output_decode))
	{
		std::cout << "Decoded: ";
		for (int i = 0; i < v_output_decode.size(); i++)
			std::cout << v_output_decode[i] << " ";
		std::cout << std::endl;
	}
	else std::cout << "Decoding impossible: >1 erros detected!\n";


	///////////////////////////////
	// Hamming code algorithm
	// with practical usage ability
	///////////////////////////////

	std::cout << "//// Hamming code alg. with practical usage ability ////\n";

	int i_input = 14578;
	hcode::int_h_code i_output;
	i_output = coder.encode_int(i_input);
	int ii_output;
	if (coder.decode_int(i_output, &ii_output))
	{
		std::cout << ii_output;
	}

	return 0;
}