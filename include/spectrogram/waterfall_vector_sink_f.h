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

#ifndef INCLUDED_SPECTROGRAM_WATERFALL_VECTOR_SINK_F_H
#define INCLUDED_SPECTROGRAM_WATERFALL_VECTOR_SINK_F_H

#ifdef ENABLE_PYTHON
#include <Python.h>
#endif

#include <spectrogram/api.h>
#include <gnuradio/sync_block.h>
#include <qapplication.h>

namespace gr
{
namespace spectrogram
{
    /*!
     * \brief A graphical sink to display multiple signals on a
     * waterfall (spectrogram) plot.
     * \ingroup spectrogram
     *
     * \details
     * This is a QT-based graphical sink the takes set of a floating
     * point streams and plots a waterfall (spectrogram) plot.
     */
class SPECTROGRAM_API waterfall_vector_sink_f : virtual public gr::sync_block
{
public:
  typedef boost::shared_ptr<waterfall_vector_sink_f> sptr;
      /*!
       * \brief Build a floating point waterfall sink.
       *
       * \param vecsize size of the input vector to display. If using
       *        the PDU message port to plot samples, the length of
       *        each PDU must be a multiple of the FFT size.
       * \param freqcenter center frequency of signal (use for x-axis labels)
       * \param bandwidth bandwidth of signal (used to set x-axis labels)
       * \param name title for the plot
       * \param nconnections number of signals to be connected to the
       *        sink. The PDU message port is always available for a
       *        connection, and this value must be set to 0 if only
       *        the PDU message port is being used.
       * \param parent a QWidget parent object, if any
       */
  static sptr make(int vecsize,
                   double freqcenter, double bandwidth,
                   const std::string &name,
                   int nconnections = 1,
                   QWidget *parent = NULL);

  virtual void exec_() = 0;
  virtual QWidget *qwidget() = 0;

#ifdef ENABLE_PYTHON
  virtual PyObject *pyqwidget() = 0;
#else
  virtual void *pyqwidget() = 0;
#endif

  virtual void clear_data() = 0;

  virtual void set_vec_size(const int vecsize) = 0;
  virtual int vec_size() const = 0;
  virtual void set_time_per_vec(const double t) = 0;
  virtual void set_vec_average(const float vecavg) = 0;
  virtual float vec_average() const = 0;

  virtual void set_frequency_range(const double centerfreq,
                                   const double bandwidth) = 0;
  virtual void set_intensity_range(const double min,
                                   const double max) = 0;

  virtual void set_update_time(double t) = 0;
  virtual void set_title(const std::string &title) = 0;
  virtual void set_time_title(const std::string &title) = 0;
  virtual void set_line_label(int which, const std::string &line) = 0;
  virtual void set_line_alpha(int which, double alpha) = 0;
  virtual void set_color_map(int which, const int color) = 0;

  virtual std::string title() = 0;
  virtual std::string line_label(int which) = 0;
  virtual double line_alpha(int which) = 0;
  virtual int color_map(int which) = 0;

  virtual void set_size(int width, int height) = 0;

  virtual void auto_scale() = 0;
  virtual double min_intensity(int which) = 0;
  virtual double max_intensity(int which) = 0;

  virtual void enable_menu(bool en = true) = 0;
  virtual void enable_grid(bool en = true) = 0;
  virtual void disable_legend() = 0;
  virtual void enable_axis_labels(bool en = true) = 0;

  QApplication *d_qApplication;
};

}; // namespace spectrogram
} // namespace gr

#endif /* INCLUDED_SPECTROGRAM_WATERFALL_VECTOR_SINK_F_H */
