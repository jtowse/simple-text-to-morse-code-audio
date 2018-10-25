//written by john towse in 2018/10/25 All right reserved.
//wave.h and morse 2.h must be included in this piece of code
//It use wave.h to generate a audio file, use morse.h to convert single char into string of numbers
//to compile and run: gcc text-to-morse-audio.c -o test
//to run: ./test
//it will then generate an audiofile named as "something"

#include<math.h>
#include<stdio.h>
#include<string.h>
#include"morse 2.h"
#include"wave.h"

#define _USE_MATH_DEFINES
//#include <cmath>
#define MAX 1000

void small(MONO_PCM wav);
void large(MONO_PCM wav);
void quite(MONO_PCM wav);
float A = 0.25; //Amplitude
float f0 = 800.0; //Foundamental Frequency

int main()
{
    char string[160], ch, mors[160];
    int i = 0;
    int c = 0;
    char m[100] ;
    int tape = 0, a = 0;
    printf("Enter a line to convert into morse code \n ");
    while(ch != '\n')    // over if user hit enter
    {
        ch = getchar();
        string[i] = ch;
        i++;
    }
    string[i] = '\0';       // inserting eof
    //get information to convert
    int j = i - 1;//j now is the maximum bits of the char to be tested
    MONO_PCM wav;
    wav.sr = 8000;
    wav.bits = 16; //Bit Depth
    wav.length = 4000 * 12 * i; //Sample Length determined by the length of wavfile
    wav.signal = calloc(wav.length, sizeof(double)); //Allocate memory space 
    //allocate memory space over the number characters
    //use morse 2.h to convert character into morse scructure
    for(int k = 0; k < j; k++)//iterate to convert every letter into string
    {
        strcpy(m, morse(string[k], mors)) ;
        printf("working on letter \"%c\" \n morse scructure: %s \n", string[k],m);
        if(m[0] == '9'){//find the first bit
            for (tape = a; tape < a+ 2000; tape++)
                    {
                        for (i = 1; i <= 15; i++) //Up to 15th harmonics
                        {
                            if (i % 2 == 1) //Only odd harmonics
                            {
                                wav.signal[tape] += 0.001 / i * sin(2.0 * M_PI * 1000 * i * tape / wav.sr);
                            }
                        }
                    }
            a = tape;
            for(int y = 1 ; y < 15; y = y + 1)
                    {//to iterate it for maximum 15 times unitl reaches 9
                        if(m[y] == '1'){//1 means short in mors.h
                            for (tape = a; tape < a + 1000; tape++)//generate a short square wave
                                {
                                    for (i = 1; i <= 15; i++)
                                    {
                                        if (i % 2 == 1)
                                        {
                                            wav.signal[tape] += A / i * sin(2.0 * M_PI * f0 * i * tape / wav.sr);
                                        }
                                    }
                                }
                            a = tape;
                        }else if(m[y] == '2'){
                            for (tape = a; tape < a + 3000; tape++)//generate a long square wave
                                {
                                    for (i = 1; i <= 15; i++)
                                    {
                                        if (i % 2 == 1)
                                        {
                                            wav.signal[tape] += A / i * sin(2.0 * M_PI * f0 * i * tape / wav.sr);
                                        }
                                    }
                                }
                            a = tape;
                        }else if(m[y] == '0'){
                            for (tape = a; tape < a + 500; tape++)//generate empty
                                {
                                    for (i = 1; i <= 15; i++)
                                    {
                                        if (i % 2 == 1)
                                        {
                                            wav.signal[tape] += 0.0001 / i * sin(2.0 * M_PI * f0 * i * tape / wav.sr);
                                        }
                                    }
                                }
                            a = tape;                     
                        }else if(m[y] == '9'){//search for the end of file in morse.h
                            for (tape = a; tape < a + 2000; tape++)
                                {
                                    for (i = 1; i <= 15; i++) //Up to 15th harmonics
                                    {
                                        if (i % 2 == 1) //Only odd harmonics
                                        {
                                            wav.signal[tape] += 0.001 / i * sin(2.0 * M_PI * 1000 * i * tape / wav.sr);
                                        }
                                    }
                                }
                            a = tape;
                            break;
                        }//end of search for 9
                    }//end of iteration for the string of morse
                }//find the begin of string      
            }//letter iteration stop
    mono_wave_write(&wav, "something.wav"); //Save sound data as wave file
    printf("File succesfully generated! Please check your inventory\n");
    free(wav.signal); //Free memory space
  return 0;
}
