#include "benford_rand.hpp"

#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <cmath>
#include <algorithm>

class benford_rand{
    
    private:
        int ms_time(){
            //Get current time in microseconds
            namespace c = std::chrono;
            long int nanos = static_cast<long int>(c::duration_cast<c::nanoseconds>(c::high_resolution_clock::now().time_since_epoch()).count());
            return abs((int)nanos);
        }

        int time_rand(int min, int max){
            //Generate random number from microsecond time stamps
            int nanos = ms_time();
            for (int i = 3; i < 10; i++){
                if (nanos % i == 0){
                    return min + (nanos) % (int)(max - min + 1);
                }
                nanos = ms_time();
            }
            
            return min + (nanos) % (int)(max - min + 1);
        }
    
        double get_probability(double n, int digit=1){
            //Probability a number will exist based on Bendford's law
            double probability = 0;
            if (digit == 1){
                if (n == 0.0){
                    probability = 0.12;
                }
                else{
                    probability = log10(1+(1/n)); //Benford's law probability rule
                }
            }
            else if (digit < 4){ //if digit is 2-3 use Benford's law generalized past 1 digit
                double max_value = pow(10,(digit-1))-1;
                double k = 0;
                for (int i = 0; i < max_value; i++){
                    k += pow(10,(digit-2));
                    probability += log10(1+(1/((10*k)+n)));
                }
            }
            else{ //Digits greater than four have probability of 10% for each number
                probability = 0.1;
            }
            return probability;
        }
    
    public:
        int rand(int pool_size = 100000){
            unsigned int arr_size = 0;
            
            //Determine size of random number pool for the first digit
            for (int i = 0; i < 10; i++){
                double probability = get_probability(i*1.0);
                arr_size += (int)((probability*pool_size)+0.5);
            }
            
            int first_digit_pool[arr_size];
            int arr_counter = 0;
            //Creates pool of numbers following Benford's law to pull randomly from
            for (int i = 0 ; i < 10; i++){
                double probability = get_probability(i*1.0);
                int n_norm_probability = (int)((probability*pool_size)+0.5);
                for (int p = 0; p < n_norm_probability; p++){
                    first_digit_pool[arr_counter] = i;
                    arr_counter+=1;
                }
            }
            std::random_shuffle(&first_digit_pool[0], &first_digit_pool[arr_size]);
            
            
            

            
            
            int rand_size_min = 1;
            int rand_size_max = 9;
            int rand_size = time_rand(rand_size_min,rand_size_max); //Determines number of digits within random number
            std::string rand_str;
            for (int i = 0; i < rand_size;i++){
                int rand_index_min = 1;
                int rand_index_max = arr_size;
                int rand_index = time_rand(rand_index_min,rand_index_max);
                rand_str += std::to_string(first_digit_pool[rand_index]);
            }
            //std::cout << rand_str << std::endl;
            int rand = std::stoi(rand_str);
            return rand;
        }
};
