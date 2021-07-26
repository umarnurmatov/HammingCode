#include "hamming_code.h"
#include <iostream>
using namespace hcode;

void e_hamming_code::encode_vector(std::vector<bool>* input, OUT std::vector<bool>* output)
{
	std::vector<bool> rbit;
	bool rtbit = 0;
	int rbit_ctr = 0;

	output->push_back(0);
	for (int i = 1; ; i++)
	{
		if (i - rbit_ctr - 1 == input->size()) break;
		else if (is_power_of_two(i))
		{
			output->push_back(0);
			rbit_ctr++;
		}
		else output->push_back(input->at(i - rbit_ctr - 1));
	}

	for (int i = 0; i < rbit_ctr; i++)
	{
		rbit.push_back(0);
		for (int n = 1; n < output->size(); n++)
			if (output->at(n) && read_bit(n, i)) rbit[i] = rbit[i] ^ 1;

		output->at(pow(2, i)) = rbit[i];
	}

	for (int i = 0; i < output->size(); i++) if (output->at(i)) rtbit ^= 1;
	output->at(0) = rtbit;
}

bool e_hamming_code::decode_vector(std::vector<bool>* input, OUT std::vector<bool>* output)
{
	int ebit = 0, rbit_ctr = 0;
	bool etbit;

	for (int i = 1; i < input->size(); i++)
		if (is_power_of_two(i)) rbit_ctr++;

	for (int i = 0; i < rbit_ctr; i++)
	{
		for (int n = 1; n < input->size(); n++)
			if (input->at(n) && (1 << i) & n) ebit ^= (1 << i);
	}

	if (ebit)
	{
		input->at(ebit) = !input->at(ebit);

		etbit = input->at(0);
		for (int i = 1; i < input->size(); i++) if (input->at(i)) etbit ^= 1;
		if (etbit) return false;
	}

	for (int i = 1; i < input->size(); i++)
		if (!is_power_of_two(i)) output->push_back(input->at(i));

	return true;
}

int_h_code e_hamming_code::encode_int(int input)
{
	int_h_code output;

	for (short k = 0; k < i_bit_size / data_size; k++)
	{
		int rbit = 0;
		bool rtbit = 0;
		short rbit_ctr = 0;
		for (short i = 1; i < code_size; i++)
		{
			if (is_power_of_two(i))
			{
				write_to_bit(&output.cw[k], i, 0);
				rbit_ctr++;
			}
			else write_to_bit(&output.cw[k], i, read_bit(input, i - rbit_ctr - 1 + data_size * k));
		}

		for (int i = 0; i < rbit_ctr; i++)
		{
			for (int n = 1; n < code_size; n++)
				if (read_bit(output.cw[k], n) && read_bit(n, i)) write_to_bit(&rbit, i, read_bit(rbit, i) ^ 1);

			write_to_bit(&output.cw[k], pow(2, i), read_bit(rbit, i));
		}

		for (int i = 1; i < code_size; i++) if (read_bit(output.cw[k], i)) rtbit ^= 1;
		write_to_bit(&output.cw[k], 0, rtbit);
	}
	return output;
}

float_h_code e_hamming_code::encode_float(float input)
{
	float_h_code output;

	long* r_input = reinterpret_cast<long*>(&input);

	for (short k = 0; k < f_bit_size / data_size; k++)
	{
		int rbit = 0;
		bool rtbit = 0;
		short rbit_ctr = 0;

		for (short i = 1; i < code_size; i++)
		{
			if (is_power_of_two(i))
			{
				write_to_bit(&output.cw[k], i, 0);
				rbit_ctr++;
			}
			else write_to_bit(&output.cw[k], i, read_bit(*r_input, i - rbit_ctr - 1 + data_size * k));
		}

		for (int i = 0; i < rbit_ctr; i++)
		{
			for (int n = 1; n < code_size; n++)
				if (read_bit(output.cw[k], n) && read_bit(n, i)) write_to_bit(&rbit, i, read_bit(rbit, i) ^ 1);

			write_to_bit(&output.cw[k], pow(2, i), read_bit(rbit, i));
		}

		for (int i = 1; i < code_size; i++) if (read_bit(output.cw[k], i)) rtbit ^= 1;
		write_to_bit(&output.cw[k], 0, rtbit);
	}
	return output;
}


bool e_hamming_code::decode_int(int_h_code input, OUT int* output)
{
	for (short k = 0; k < i_bit_size / data_size; k++)
	{
		short ebit = 0;
		bool etbit;

		for (int i = 0; i < rbit_num; i++)
		{
			for (int n = 1; n < code_size; n++)
				if (read_bit(input.cw[k], n) && (1 << i) & n) ebit ^= (1 << i);
		}

		if (ebit)
		{
			invert_bit(&input.cw[k], ebit);

			etbit = read_bit(input.cw[k], 0);
			for (int i = 1; i < code_size; i++) if (read_bit(input.cw[k], i)) etbit ^= 1;
			if (etbit) return false;
		}

		short rbit_ctr = 0;
		for (int i = 1; i < code_size; i++)
		{
			if (!is_power_of_two(i)) write_to_bit(output, i - rbit_ctr - 1 + data_size * k, read_bit(input.cw[k], i));
			else rbit_ctr++;
		}
	}

	return true;
}

bool e_hamming_code::decode_float(float_h_code input, OUT float* output)
{
	long i_output;

	for (short k = 0; k < f_bit_size / data_size; k++)
	{
		short ebit = 0;
		bool etbit;

		for (int i = 0; i < rbit_num; i++)
		{
			for (int n = 1; n < code_size; n++)
				if (read_bit(input.cw[k], n) && (1 << i) & n) ebit ^= (1 << i);
		}

		if (ebit)
		{
			invert_bit(&input.cw[k], ebit);

			etbit = read_bit(input.cw[k], 0);
			for (int i = 1; i < code_size; i++) if (read_bit(input.cw[k], i)) etbit ^= 1;
			if (etbit) return false;
		}

		short rbit_ctr = 0;
		for (int i = 1; i < code_size; i++)
		{
			if (!is_power_of_two(i)) write_to_bit(&i_output, i - rbit_ctr - 1 + data_size * k, read_bit(input.cw[k], i));
			else rbit_ctr++;
		}
	}
	*output = *reinterpret_cast<float*>(&i_output);

	return true;
}
