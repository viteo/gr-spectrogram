/* -*- c++ -*- */

#define SPECTROGRAM_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "spectrogram_swig_doc.i"

%{
#include "spectrogram/waterfall_vector_sink_f.h"
%}


%include "spectrogram/waterfall_vector_sink_f.h"
GR_SWIG_BLOCK_MAGIC2(spectrogram, waterfall_vector_sink_f);
