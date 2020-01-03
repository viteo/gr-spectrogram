/* -*- c++ -*- */
/*
 * Copyright 2008-2012 Free Software Foundation, Inc.
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef WATERFALL_VECTOR_DISPLAY_PLOT_H
#define WATERFALL_VECTOR_DISPLAY_PLOT_H

#include <spectrogram/api.h>
#include <gnuradio/high_res_timer.h>
#include <spectrogram/DisplayPlot.h>
#include <spectrogram/WaterfallVectorGlobalData.h>
#include <qwt_plot_spectrogram.h>
#include <stdint.h>
#include <cstdio>
#include <vector>

#if QWT_VERSION < 0x060000
#include <spectrogram/plot_waterfall.h>
#else
// clang-format off
#include <qwt_point_3d.h> // doesn't seem necessary, but is...
#include <qwt_compat.h>
// clang-format on
#endif

/*!
 * \brief QWidget for displaying waterfall (spectrogram) plots.
 * \ingroup spectrogram_blk
 */
class SPECTROGRAM_API WaterfallVectorDisplayPlot : public DisplayPlot
{
    Q_OBJECT

    Q_PROPERTY(int intensity_color_map_type1 READ getIntensityColorMapType1 WRITE
                   setIntensityColorMapType1)
    Q_PROPERTY(QColor low_intensity_color READ getUserDefinedLowIntensityColor WRITE
                   setUserDefinedLowIntensityColor)
    Q_PROPERTY(QColor high_intensity_color READ getUserDefinedHighIntensityColor WRITE
                   setUserDefinedHighIntensityColor)
    Q_PROPERTY(int color_map_title_font_size READ getColorMapTitleFontSize WRITE
                   setColorMapTitleFontSize)

public:
    WaterfallVectorDisplayPlot(int nplots, QWidget *);
    virtual ~WaterfallVectorDisplayPlot();

    void resetAxis();

    void setFrequencyRange(const double,
                           const double,
                           const double units = 1000.0,
                           const std::string &strunits = "kHz");
    double getStartFrequency() const;
    double getStopFrequency() const;

    void plotNewData(const std::vector<double *> dataPoints,
                     const int64_t numDataPoints,
                     const double timePerVec,
                     const gr::high_res_timer_type timestamp,
                     const int droppedFrames);

    // to be removed
    void plotNewData(const double *dataPoints,
                     const int64_t numDataPoints,
                     const double timePerVec,
                     const gr::high_res_timer_type timestamp,
                     const int droppedFrames);

    void setIntensityRange(const double minIntensity, const double maxIntensity);
    double getMinIntensity(int which) const;
    double getMaxIntensity(int which) const;

    void replot(void);
    void clearData();

    int getIntensityColorMapType(int) const;
    int getIntensityColorMapType1() const;
    int getColorMapTitleFontSize() const;
    const QColor getUserDefinedLowIntensityColor() const;
    const QColor getUserDefinedHighIntensityColor() const;

    int getAlpha(int which);
    void setAlpha(int which, int alpha);

    int getNumRows() const;

public slots:
    void setIntensityColorMapType(const int, const int, const QColor, const QColor);
    void setIntensityColorMapType1(int);
    void setColorMapTitleFontSize(int tfs);
    void setUserDefinedLowIntensityColor(QColor);
    void setUserDefinedHighIntensityColor(QColor);
    void disableLegend();
    void enableLegend();
    void enableLegend(bool en);
    void setNumRows(int nrows);

signals:
    void updatedLowerIntensityLevel(const double);
    void updatedUpperIntensityLevel(const double);

private:
    void _updateIntensityRangeDisplay();

    double d_start_frequency;
    double d_stop_frequency;
    double d_center_frequency;
    int d_xaxis_multiplier;
    bool d_legend_enabled;
    int d_nrows;

    std::vector<WaterfallVectorData *> d_data;

#if QWT_VERSION < 0x060000
    std::vector<PlotWaterfall *> d_spectrogram;
#else
    std::vector<QwtPlotSpectrogram *> d_spectrogram;
#endif

    std::vector<int> d_intensity_color_map_type;
    QColor d_user_defined_low_intensity_color;
    QColor d_user_defined_high_intensity_color;
    int d_color_bar_title_font_size;
};

#endif /* WATERFALL_VECTOR_DISPLAY_PLOT_H */
