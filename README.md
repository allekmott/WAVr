WAVr
--------------------------

Wave-er; A simple, but powerful command line tool to be used in reading,
writing, generating, manipulating, and otherwise hacky processing of
WAV audio files. 

Signal Generator
--------------------------

(As of now) The default action of WAVr is to generate a 1-second sine wave 
with a frequency of 1000 Hz. Eventually, other waveforms will be available in
the mix, as well as not just signal generation, but DSP also.

Typical Usecases
--------------------------

**Signal Generation**
Generate a 4.5 second sine wave with frequency 440 Hz and sample rate 96 kHz,
using 2 threads:

	./wavr -t 4.5 -f 440 -s 96000 -t 2

**Reading WAV files**
Read file "signal.wav" and dump all samples to the standard output:

	./wavr -i signal.wav -d

Parse samples from standard input and output to "signal.wav":
	
	./wavr -c -o signal.wav

Hacky WAV duplication:
	
	./wavr -i signal.wav -d | ./wavr -c -o signal_1.wav