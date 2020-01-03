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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "waterfall_vector_sink_f_impl.h"
#include <gnuradio/io_signature.h>
#include <gnuradio/prefs.h>
#include <volk/volk.h>

namespace gr
{
namespace spectrogram
{

waterfall_vector_sink_f::sptr waterfall_vector_sink_f::make(int vecsize,
                                                            double freqcenter,
                                                            double bandwidth,
                                                            const std::string &name,
                                                            int nconnections,
                                                            QWidget *parent)
{
  return gnuradio::get_initial_sptr(
      new waterfall_vector_sink_f_impl(vecsize, freqcenter, bandwidth, name, nconnections, parent));
}

/*
* The private constructor
*/
waterfall_vector_sink_f_impl::waterfall_vector_sink_f_impl(int vecsize,
                                                           double freqcenter,
                                                           double bandwidth,
                                                           const std::string &name,
                                                           int nconnections,
                                                           QWidget *parent)
    : gr::sync_block("waterfall_vector_sink_f",
                     io_signature::make(0, nconnections, sizeof(float) * vecsize),
                     io_signature::make(0, 0, 0)),
      d_vecsize(vecsize), d_vecavg(1.0),
      d_center_freq(freqcenter), d_bandwidth(bandwidth), d_name(name), d_nconnections(nconnections), d_nrows(200),
      d_parent(parent), d_port(pmt::mp("freq"))
{
  // Required now for Qt; argc must be greater than 0 and argv
  // must have at least one valid character. Must be valid through
  // life of the qApplication:
  // http://harmattan-dev.nokia.com/docs/library/html/qt4/qapplication.html
  d_argc = 1;
  d_argv = new char;
  d_argv[0] = '\0';

  d_main_gui = NULL;

  d_index = 0;
  // save the last "connection" for the PDU memory
  for (int i = 0; i < d_nconnections; i++)
  {
    d_magbufs.push_back((double *)volk_malloc(d_vecsize * sizeof(double), volk_get_alignment()));
    memset(d_magbufs[i], 0, d_vecsize * sizeof(double));
  }

  initialize();
}

/*
* Our virtual destructor.
*/
waterfall_vector_sink_f_impl::~waterfall_vector_sink_f_impl()
{
  if (!d_main_gui->isClosed())
    d_main_gui->close();

  for (int i = 0; i < d_nconnections; i++)
  {
    volk_free(d_magbufs[i]);
  }

  delete d_argv;
}

bool waterfall_vector_sink_f_impl::check_topology(int ninputs, int noutputs)
{
  return ninputs == d_nconnections;
}

void waterfall_vector_sink_f_impl::initialize()
{
  if (qApp != NULL)
  {
    d_qApplication = qApp;
  }
  else
  {
#if QT_VERSION >= 0x040500
    std::string style = prefs::singleton()->get_string("qtgui", "style", "raster");
    QApplication::setGraphicsSystem(QString(style.c_str()));
#endif
    d_qApplication = new QApplication(d_argc, &d_argv);
  }

  // If a style sheet is set in the prefs file, enable it here.
  check_set_qss(d_qApplication);

  int numplots = (d_nconnections > 0) ? d_nconnections : 1;
  d_main_gui = new WaterfallVectorDisplayForm(numplots, d_parent);
  set_vec_size(d_vecsize);
  set_frequency_range(d_center_freq, d_bandwidth);

  if (d_name.size() > 0)
    set_title(d_name);

  // initialize update time to 10 times a second
  set_update_time(0.1);
}

void waterfall_vector_sink_f_impl::exec_() { d_qApplication->exec(); }

QWidget *waterfall_vector_sink_f_impl::qwidget() { return d_main_gui; }

#ifdef ENABLE_PYTHON
PyObject *waterfall_vector_sink_f_impl::pyqwidget()
{
  PyObject *w = PyLong_FromVoidPtr((void *)d_main_gui);
  PyObject *retarg = Py_BuildValue("N", w);
  return retarg;
}
#else
void *waterfall_vector_sink_f_impl::pyqwidget()
{
  return NULL;
}
#endif

void waterfall_vector_sink_f_impl::clear_data()
{
  d_main_gui->clearData();
}

void waterfall_vector_sink_f_impl::set_vec_size(const int vecsize) { d_main_gui->setVecSize(vecsize); }

int waterfall_vector_sink_f_impl::vec_size() const { return d_vecsize; }

void waterfall_vector_sink_f_impl::set_vec_average(const float vecavg)
{
  d_main_gui->setVecAverage(vecavg);
}

float waterfall_vector_sink_f_impl::vec_average() const { return d_vecavg; }

void waterfall_vector_sink_f_impl::set_frequency_range(const double centerfreq, const double bandwidth)
{
  d_center_freq = centerfreq;
  d_bandwidth = bandwidth;
  d_main_gui->setFrequencyRange(d_center_freq, d_bandwidth);
}

void waterfall_vector_sink_f_impl::set_intensity_range(const double min, const double max)
{
  d_main_gui->setIntensityRange(min, max);
}

void waterfall_vector_sink_f_impl::set_update_time(double t)
{
  // convert update time to ticks
  gr::high_res_timer_type tps = gr::high_res_timer_tps();
  d_update_time = t * tps;
  d_main_gui->setUpdateTime(t);
  d_last_time = 0;
}

void waterfall_vector_sink_f_impl::set_title(const std::string &title)
{
  d_main_gui->setTitle(title.c_str());
}

void waterfall_vector_sink_f_impl::set_time_title(const std::string &title)
{
  d_main_gui->setTimeTitle(title);
}

void waterfall_vector_sink_f_impl::set_line_label(int which, const std::string &label)
{
  d_main_gui->setLineLabel(which, label.c_str());
}

void waterfall_vector_sink_f_impl::set_color_map(int which, const int color)
{
  d_main_gui->setColorMap(which, color);
}

void waterfall_vector_sink_f_impl::set_line_alpha(int which, double alpha)
{
  d_main_gui->setAlpha(which, (int)(255.0 * alpha));
}

void waterfall_vector_sink_f_impl::set_size(int width, int height)
{
  d_main_gui->resize(QSize(width, height));
}

std::string waterfall_vector_sink_f_impl::title() { return d_main_gui->title().toStdString(); }

std::string waterfall_vector_sink_f_impl::line_label(int which)
{
  return d_main_gui->lineLabel(which).toStdString();
}

int waterfall_vector_sink_f_impl::color_map(int which) { return d_main_gui->getColorMap(which); }

double waterfall_vector_sink_f_impl::line_alpha(int which)
{
  return (double)(d_main_gui->getAlpha(which)) / 255.0;
}

void waterfall_vector_sink_f_impl::auto_scale() { d_main_gui->autoScale(); }

double waterfall_vector_sink_f_impl::min_intensity(int which)
{
  return d_main_gui->getMinIntensity(which);
}

double waterfall_vector_sink_f_impl::max_intensity(int which)
{
  return d_main_gui->getMaxIntensity(which);
}

void waterfall_vector_sink_f_impl::enable_menu(bool en) { d_main_gui->enableMenu(en); }

void waterfall_vector_sink_f_impl::enable_grid(bool en) { d_main_gui->setGrid(en); }

void waterfall_vector_sink_f_impl::enable_axis_labels(bool en) { d_main_gui->setAxisLabels(en); }

void waterfall_vector_sink_f_impl::disable_legend() { d_main_gui->disableLegend(); }

void waterfall_vector_sink_f_impl::check_clicked()
{
  if (d_main_gui->checkClicked())
  {
    double freq = d_main_gui->getClickedFreq();
    message_port_pub(d_port, pmt::cons(d_port, pmt::from_double(freq)));
  }
}

void waterfall_vector_sink_f_impl::set_time_per_vec(double t) { d_main_gui->setTimePerVec(t); }

int waterfall_vector_sink_f_impl::work(int noutput_items,
                                       gr_vector_const_void_star &input_items,
                                       gr_vector_void_star &output_items)
{
  const float *in = (const float *)input_items[0];

  // Update the vec size from the application
  check_clicked();

  for (int i = 0; i < noutput_items; i++)
  {
    if (gr::high_res_timer_now() - d_last_time > d_update_time)
    {
      for (int n = 0; n < d_nconnections; n++)
      {
        in = ((const float *)input_items[n]) + d_vecsize;
        for (int x = 0; x < d_vecsize; x++)
        {
          d_magbufs[n][x] =
              (double)((1.0 - d_vecavg) * d_magbufs[n][x] + (d_vecavg)*in[x]);
        }
      }

      d_last_time = gr::high_res_timer_now();
      d_qApplication->postEvent(
          d_main_gui, new WaterfallUpdateEvent(d_magbufs, d_vecsize, d_last_time));
    }
  }
  // Tell runtime system how many output items we produced.
  return noutput_items;
}

} // namespace spectrogram
} // namespace gr
