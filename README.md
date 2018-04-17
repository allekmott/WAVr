WAVr
--------------------------

Command line tool for creation (signal generation) & manipulation of WAV files.

As of right now, only signal generation & WAV outputting are implemented.

Usage: `./wavr [options]`

### Options

Input/Output:
- `-o <file>`		set output file path

Signal:
- `-w <waveform>`	select desired waveform
  
  Avaliable waveforms include:
  - `sine`	(**1**) - a sine wave
  - `square`	(**2**) - a square wave
  - `triangle`	(**3**) - a triangle wave
  - `sawtooth`	(**4**) - a sawtooth wave
  - `tangent`	(**5**) - a tangent wave

- `-a <amplitude>`	Set desired amplitude   (0-100% or 0.0-1.0)
- `-f <frequency>`	set desired frequency	(Hz)
- `-d <duration>`	set desired duration	(s)

Sampling:
- `-s <sample_rate>`	set desired sample rate	(kHz OR Hz)
- `-b <bit_depth>`	set desired bit depth	(bits; 8/16/24/32)

Processing (**NOT IMPLEMENTED**):
- `-t <thread_count>`	specify number of threads to use

