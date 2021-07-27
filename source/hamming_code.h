// Created by Umar Nurmatov, 2021
// For new versions of library see project's github: umarnurmatov

#pragma once

#include <vector>	

#define OUT

namespace hcode
{
	const short byte_size = 8;

	const short i_bit_size = byte_size * sizeof(int);
	const short f_bit_size = byte_size * sizeof(float);
	const short l_bit_size = byte_size * sizeof(long);

	// Bit size of (7, 4) extended Hamming code
	const short code_size = 8;
	// Bit size of data for (7, 4) ex.HC
	const short data_size = 4;
	// Num of right-cheking bits for (7, 4) HC
	const short rbit_num = 3;

	// Struct contains int value's codeword
	struct int_h_code 
	{
		char cw[i_bit_size / data_size];
		int_h_code() { for (short i = 0; i < i_bit_size / data_size; i++) cw[i] = 0; }
	};
	struct float_h_code
	{
		char cw[f_bit_size / data_size];
		float_h_code() { for (short i = 0; i < f_bit_size / data_size; i++) cw[i] = 0; }
	};

	class e_hamming_code {


		// Writes "val" in "x" at "pos" position
		inline void write_to_bit(char* x, short pos, bool val)
		{
			if (val) *x |= (1 << pos);
			else *x &= ~(1 << pos);
		}
		inline void write_to_bit(int* x, short pos, bool val)
		{
			if (val) *x |= (1 << pos);
			else *x &= ~(1 << pos);
		}
		inline void write_to_bit(long* x, short pos, bool val)
		{
			if (val) *x |= (1 << pos);
			else *x &= ~(1 << pos);
		}

		// Inverts a bit in "x" at "pos" position
		inline void invert_bit(char* x, short pos) { *x ^= (1 << pos); }

		// Returns a value of bit in "x" at "pos" position
		inline bool read_bit(char x, short pos) { return bool((1 << pos) & x); }
		inline bool read_bit(int x, short pos) { return bool((1 << pos) & x); }
		inline bool read_bit(long x, short pos) { return bool((1 << pos) & x); }

		// True if x is a power of 2; false if not
		inline bool is_power_of_two(int x) { return (x != 0) && ((x & (x - 1)) == 0); }

	public:

		///////////////////////////////////////////////////////////
		/* Extended Hamming code algorithm funcs for vectors
		   No limitations for data vector
		   Note: ineffectiv; use only for demonstration purposes */
		///////////////////////////////////////////////////////////

		   // Encoding a data bool vector
		void encode_vector(std::vector<bool>* input, OUT std::vector<bool>* output);

		/* Decoding a codeword bool vector
		* Fixes a 1-bit error or discover a 2-bit error */
		bool decode_vector(std::vector<bool>* input, OUT std::vector<bool>* output);



		///////////////////////////////////////////////////////////////////
		/* Extended Hamming code algorithm funcs for integer values
		* ! Do not use at the same time on devices with different bitness !
		* Note#1: effectiv, can be used in practice
		* Note#2: automatically adapts to your [float/int]-size,
		  so you can use it on different machines / compilators */
		///////////////////////////////////////////////////////////////////

		/* Encoding value
		* Algorithm:
		* 1. Split int-val into a 4-bit groups
		* 2. Encode each group in ex. (7, 4) Hamming code -> getting 8-bit codewords group
		* 3. Writing codewords into codeword structure (e.g. int_h_code) */

		int_h_code encode_int(int input);
		float_h_code encode_float(float input);

		/* Decoding codeword
		* Can [fix 1-bit error] or [dicover 2-bit errors] in each part-codeword -->
		* finally can [fix 4 or 8 1-bit error] or [dicover 8 or 16 2-bit errors] (depends on int size) */
		bool decode_int(int_h_code input, OUT int* output);
		bool decode_float(float_h_code input, OUT float* output);
	};
}




