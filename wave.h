#include <stdio.h>
#include <stdlib.h>

#define kMaxValue 65536.0 //2^16 (short type) 
#define kMaxDepth 32768.0 //2^16/2 (signed short type covering range -32768~32767)

typedef struct
{
  int sr; //Sample Rate
  int bits; //Bit Depth
  int length; //Sample Length
  double *signal; //Sound Data
} MONO_PCM;

typedef struct
{
  int sr; //Sample Rate
  int bits; //Bit Depth
  int length; //Sample Length
  double *left; //Left Channel Sound Data
  double *right; //Right Channel Sound Data
} STEREO_PCM;

void mono_wave_read(MONO_PCM *pcm, char *file_name)
{
  FILE *fp;
  int n;
  char riff_chunk_ID[4];
  long riff_chunk_size;
  char riff_form_type[4];
  char fmt_chunk_ID[4];
  long fmt_chunk_size;
  short fmt_wave_format_type;
  short fmt_channel;
  long fmt_samples_per_sec;
  long fmt_bytes_per_sec;
  short fmt_block_size;
  short fmt_bits_per_sample;
  char data_chunk_ID[4];
  long data_chunk_size;
  short data;
  
  fp = fopen(file_name, "rb");
  
  fread(riff_chunk_ID, 1, 4, fp);
  fread(&riff_chunk_size, 4, 1, fp);
  fread(riff_form_type, 1, 4, fp);
  fread(fmt_chunk_ID, 1, 4, fp);
  fread(&fmt_chunk_size, 4, 1, fp);
  fread(&fmt_wave_format_type, 2, 1, fp);
  fread(&fmt_channel, 2, 1, fp);
  fread(&fmt_samples_per_sec, 4, 1, fp);
  fread(&fmt_bytes_per_sec, 4, 1, fp);
  fread(&fmt_block_size, 2, 1, fp);
  fread(&fmt_bits_per_sample, 2, 1, fp);
  fread(data_chunk_ID, 1, 4, fp);
  fread(&data_chunk_size, 4, 1, fp);
  
  pcm->sr = fmt_samples_per_sec; //Get Sample Rate
  pcm->bits = fmt_bits_per_sample; //Get Bit Depth
  pcm->length = data_chunk_size / 2; //Get Sample Length
  pcm->signal = (double *) calloc(pcm->length, sizeof(double)); //Allocate memory space for sound data
  
  for (n = 0; n < pcm->length; n++)
  {
    fread(&data, 2, 1, fp); //Read sound data
    pcm->signal[n] = (double) data / kMaxDepth; //Normalize sound data
  }
  
  fclose(fp);
}

void mono_wave_write(MONO_PCM *pcm, char *file_name)
{
  FILE *fp;
  int n;
  char riff_chunk_ID[4];
  long riff_chunk_size;
  char riff_form_type[4];
  char fmt_chunk_ID[4];
  long fmt_chunk_size;
  short fmt_wave_format_type;
  short fmt_channel;
  long fmt_samples_per_sec;
  long fmt_bytes_per_sec;
  short fmt_block_size;
  short fmt_bits_per_sample;
  char data_chunk_ID[4];
  long data_chunk_size;
  short data;
  double signal;
  
  riff_chunk_ID[0] = 'R';
  riff_chunk_ID[1] = 'I';
  riff_chunk_ID[2] = 'F';
  riff_chunk_ID[3] = 'F';
  riff_chunk_size = 36 + pcm->length * 2;
  riff_form_type[0] = 'W';
  riff_form_type[1] = 'A';
  riff_form_type[2] = 'V';
  riff_form_type[3] = 'E';
  
  fmt_chunk_ID[0] = 'f';
  fmt_chunk_ID[1] = 'm';
  fmt_chunk_ID[2] = 't';
  fmt_chunk_ID[3] = ' ';
  fmt_chunk_size = 16;
  fmt_wave_format_type = 1;
  fmt_channel = 1;
  fmt_samples_per_sec = pcm->sr; //Sampling Rate
  fmt_bytes_per_sec = pcm->sr * pcm->bits / 8;
  fmt_block_size = pcm->bits / 8;
  fmt_bits_per_sample = pcm->bits; //Bit Depth
  
  data_chunk_ID[0] = 'd';
  data_chunk_ID[1] = 'a';
  data_chunk_ID[2] = 't';
  data_chunk_ID[3] = 'a';
  data_chunk_size = pcm->length * 2;
  
  fp = fopen(file_name, "wb");
  
  fwrite(riff_chunk_ID, 1, 4, fp);
  fwrite(&riff_chunk_size, 4, 1, fp);
  fwrite(riff_form_type, 1, 4, fp);
  fwrite(fmt_chunk_ID, 1, 4, fp);
  fwrite(&fmt_chunk_size, 4, 1, fp);
  fwrite(&fmt_wave_format_type, 2, 1, fp);
  fwrite(&fmt_channel, 2, 1, fp);
  fwrite(&fmt_samples_per_sec, 4, 1, fp);
  fwrite(&fmt_bytes_per_sec, 4, 1, fp);
  fwrite(&fmt_block_size, 2, 1, fp);
  fwrite(&fmt_bits_per_sample, 2, 1, fp);
  fwrite(data_chunk_ID, 1, 4, fp);
  fwrite(&data_chunk_size, 4, 1, fp);
  
  for (n = 0; n < pcm->length; n++)
  {
    signal = (pcm->signal[n] + 1.0) / 2.0 * kMaxValue;
    
    if (signal > kMaxValue)
    {
      signal = kMaxValue; //Clipping
    }
    else if (signal < 0.0)
    {
      signal = 0.0; //Clipping
    }
    
    data = (short)(signal + 0.5) - kMaxDepth; //Rounding and offseting signal
    fwrite(&data, 2, 1, fp); //Write the sound data
  }
  
  fclose(fp);
}

void stereo_wave_read(STEREO_PCM *pcm, char *file_name)
{
  FILE *fp;
  int n;
  char riff_chunk_ID[4];
  long riff_chunk_size;
  char riff_form_type[4];
  char fmt_chunk_ID[4];
  long fmt_chunk_size;
  short fmt_wave_format_type;
  short fmt_channel;
  long fmt_samples_per_sec;
  long fmt_bytes_per_sec;
  short fmt_block_size;
  short fmt_bits_per_sample;
  char data_chunk_ID[4];
  long data_chunk_size;
  short data;
  
  fp = fopen(file_name, "rb");
  
  fread(riff_chunk_ID, 1, 4, fp);
  fread(&riff_chunk_size, 4, 1, fp);
  fread(riff_form_type, 1, 4, fp);
  fread(fmt_chunk_ID, 1, 4, fp);
  fread(&fmt_chunk_size, 4, 1, fp);
  fread(&fmt_wave_format_type, 2, 1, fp);
  fread(&fmt_channel, 2, 1, fp);
  fread(&fmt_samples_per_sec, 4, 1, fp);
  fread(&fmt_bytes_per_sec, 4, 1, fp);
  fread(&fmt_block_size, 2, 1, fp);
  fread(&fmt_bits_per_sample, 2, 1, fp);
  fread(data_chunk_ID, 1, 4, fp);
  fread(&data_chunk_size, 4, 1, fp);
  
  pcm->sr = fmt_samples_per_sec; //Get Sampling Rate
  pcm->bits = fmt_bits_per_sample; //Get Bit Depth
  pcm->length = data_chunk_size / 4; //Get Sample Length
  pcm->left = (double *) calloc(pcm->length, sizeof(double)); //Allocate memory space for left channel
  pcm->right = (double *) calloc(pcm->length, sizeof(double)); //Allocate memory space for right channel
  
  for (n = 0; n < pcm->length; n++)
  { //channels are interleaved
    fread(&data, 2, 1, fp); //Read left sound data
    pcm->left[n] = (double)data / kMaxDepth; //Normalize left sound data
    
    fread(&data, 2, 1, fp); //Read right sound data
    pcm->right[n] = (double)data / kMaxDepth; //Normalize right sound data
  }
  
  fclose(fp);
}

void stereo_wave_write(STEREO_PCM *pcm, char *file_name)
{
  FILE *fp;
  int n;
  char riff_chunk_ID[4];
  long riff_chunk_size;
  char riff_form_type[4];
  char fmt_chunk_ID[4];
  long fmt_chunk_size;
  short fmt_wave_format_type;
  short fmt_channel;
  long fmt_samples_per_sec;
  long fmt_bytes_per_sec;
  short fmt_block_size;
  short fmt_bits_per_sample;
  char data_chunk_ID[4];
  long data_chunk_size;
  short data;
  double s;
  
  riff_chunk_ID[0] = 'R';
  riff_chunk_ID[1] = 'I';
  riff_chunk_ID[2] = 'F';
  riff_chunk_ID[3] = 'F';
  riff_chunk_size = 36 + pcm->length * 4;
  riff_form_type[0] = 'W';
  riff_form_type[1] = 'A';
  riff_form_type[2] = 'V';
  riff_form_type[3] = 'E';
  
  fmt_chunk_ID[0] = 'f';
  fmt_chunk_ID[1] = 'm';
  fmt_chunk_ID[2] = 't';
  fmt_chunk_ID[3] = ' ';
  fmt_chunk_size = 16;
  fmt_wave_format_type = 1;
  fmt_channel = 2;
  fmt_samples_per_sec = pcm->sr; //Sampling Rate
  fmt_bytes_per_sec = pcm->sr * pcm->bits / 8 * 2;
  fmt_block_size = pcm->bits / 8 * 2;
  fmt_bits_per_sample = pcm->bits; //Bit Depth
  
  data_chunk_ID[0] = 'd';
  data_chunk_ID[1] = 'a';
  data_chunk_ID[2] = 't';
  data_chunk_ID[3] = 'a';
  data_chunk_size = pcm->length * 4;
  
  fp = fopen(file_name, "wb");
  
  fwrite(riff_chunk_ID, 1, 4, fp);
  fwrite(&riff_chunk_size, 4, 1, fp);
  fwrite(riff_form_type, 1, 4, fp);
  fwrite(fmt_chunk_ID, 1, 4, fp);
  fwrite(&fmt_chunk_size, 4, 1, fp);
  fwrite(&fmt_wave_format_type, 2, 1, fp);
  fwrite(&fmt_channel, 2, 1, fp);
  fwrite(&fmt_samples_per_sec, 4, 1, fp);
  fwrite(&fmt_bytes_per_sec, 4, 1, fp);
  fwrite(&fmt_block_size, 2, 1, fp);
  fwrite(&fmt_bits_per_sample, 2, 1, fp);
  fwrite(data_chunk_ID, 1, 4, fp);
  fwrite(&data_chunk_size, 4, 1, fp);
  
  for (n = 0; n < pcm->length; n++)
  {//Interleaving two channels
    s = (pcm->left[n] + 1.0) / 2.0 * kMaxValue;
    
    if (s > kMaxValue)
    {
      s = kMaxValue; //Clipping
    }
    else if (s < 0.0)
    {
      s = 0.0; //Clipping
    }
    
    data = (short)(s + 0.5) - kMaxDepth; //Rouding and offsetting signal
    fwrite(&data, 2, 1, fp); //Store left channel
    
    s = (pcm->right[n] + 1.0) / 2.0 * kMaxValue;
    
    if (s > kMaxValue)
    {
      s = kMaxValue; //Clipping
    }
    else if (s < 0.0)
    {
      s = 0.0; //Clipping
    }
    
    data = (short)(s + 0.5) - kMaxDepth; //Rounding and offsetting signal
    fwrite(&data, 2, 1, fp); //Store right channel
  }
  
  fclose(fp);
}
