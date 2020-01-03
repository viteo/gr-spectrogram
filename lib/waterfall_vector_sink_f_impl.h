/* -*- c++ -*- */
/* 
 * Copyright 2019 viteo.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_SPECTROGRAM_WATERFALL_VECTOR_SINK_F_IMPL_H
#define INCLUDED_SPECTROGRAM_WATERFALL_VECTOR_SINK_F_IMPL_H

#include <spectrogram/waterfall_vector_sink_f.h>
#include <gnuradio/high_res_timer.h>
#include <spectrogram/WaterfallVectorDisplayForm.h>

namespace gr
{
namespace spectrogram
{

class SPECTROGRAM_API waterfall_vector_sink_f_impl : public waterfall_vector_sink_f
{
private:

  void initialize();

  int d_vecsize;
  float d_vecavg;
  double d_center_freq;
  double d_bandwidth;
  std::string d_name;
  int d_nconnections;
  int d_nrows;

  const pmt::pmt_t d_port;

  int d_index;
  std::vector<double *> d_magbufs;

  int d_argc;
  char *d_argv;
  QWidget *d_parent;
  WaterfallVectorDisplayForm *d_main_gui;

  gr::high_res_timer_type d_update_time;
  gr::high_res_timer_type d_last_time;

  // TODO remove this?
  void check_clicked();

public:
  waterfall_vector_sink_f_impl(int vecsize,
                               double freqcenter, double bandwidth,
                               const std::string &name,
                               int nconnections,
                               QWidget *parent = NULL);
  ~waterfall_vector_sink_f_impl();

  bool check_topology(int ninputs, int noutputs);

  void exec_();
  QWidget *qwidget();

#ifdef ENABLE_PYTHON
  PyObject *pyqwidget();
#else
  void *pyqwidget();
#endif

  void clear_data();

  void set_vec_size(const int fftsize);
  int vec_size() const;
  void set_vec_average(const float fftavg);
  float vec_average() const;

  void set_frequency_range(const double centerfreq, const double bandwidth);
  void set_intensity_range(const double min, const double max);

  void set_update_time(double t);
  void set_time_per_vec(double t);
  void set_title(const std::string &title);
  void set_time_title(const std::string &title);
  void set_line_label(int which, const std::string &label);
  void set_line_alpha(int which, double alpha);
  void set_color_map(int which, const int color);

  std::string title();
  std::string line_label(int which);
  double line_alpha(int which);
  int color_map(int which);

  void set_size(int width, int height);

  void auto_scale();
  double min_intensity(int which);
  double max_intensity(int which);

  void enable_menu(bool en);
  void enable_grid(bool en);
  void disable_legend();
  void enable_axis_labels(bool en);

  int work(int noutput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
};

} // namespace spectrogram
} // namespace gr

#endif /* INCLUDED_SPECTROGRAM_WATERFALL_VECTOR_SINK_F_IMPL_H */
