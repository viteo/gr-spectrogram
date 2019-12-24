title: The SPECTROGRAM OOT Module
brief: Gnuradio waterfall_f_sink module without FFT
tags: # Tags are arbitrary, but look at CGRAN what other authors are using
  - qtgui
author:
  - viteo <viteo@rambler.ru>
copyright_owner:
  - GNURadio
license:
repo: https://github.com/viteo/gr-spectrogram.git
#website: <module_website> # If you have a separate project website, put it here
#icon: <icon_url> # Put a URL to a square image here that will be used as an icon on CGRAN
---
Built-in Waterfall sink in gnuradio performes FFT on input data. There is no way to display already transformed data. Spectrogram OOT module is stripped Waterfall sink without FFT.