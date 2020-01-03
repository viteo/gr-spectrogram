/* -*- c++ -*- */
/*
 * Copyright 2012 Free Software Foundation, Inc.
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

#ifndef WATERFALL_VECTOR_DISPLAY_FORM_H
#define WATERFALL_VECTOR_DISPLAY_FORM_H

#include <spectrogram/api.h>
#include <spectrogram/form_menus.h>
#include <spectrogram/WaterfallVectorDisplayPlot.h>
#include <spectrogram/WaterfallVectorUpdateEvents.h>
#include <QtGui/QtGui>
#include <vector>

#include <spectrogram/displayform.h>

/*!
 * \brief DisplayForm child for managing waterfall (spectrogram) plots.
 * \ingroup spectrogram_blk
 */
class SPECTROGRAM_API WaterfallVectorDisplayForm : public DisplayForm
{
    Q_OBJECT

public:
    WaterfallVectorDisplayForm(int nplots = 1, QWidget *parent = 0);
    ~WaterfallVectorDisplayForm();

    WaterfallVectorDisplayPlot *getPlot();

    int getVecSize() const;
    double getTimePerVec();
    float getVecAverage() const;

    int getColorMap(int which);
    int getAlpha(int which);
    double getMinIntensity(int which);
    double getMaxIntensity(int which);

    void clearData();

    // returns the frequency that was last double-clicked on by the user
    float getClickedFreq() const;

    // checks if there was a double-click event; reset if there was
    bool checkClicked();

public slots:
    void customEvent(QEvent *e);
    void setTimeTitle(const std::string);
    void setSampleRate(const QString &samprate);
    void setVecSize(const int);
    void setVecAverage(const float);

    void setFrequencyRange(const double centerfreq, const double bandwidth);

    void setIntensityRange(const double minIntensity, const double maxIntensity);
    void setMaxIntensity(const QString &m);
    void setMinIntensity(const QString &m);

    void setAlpha(int which, int alpha);

    void setColorMap(int which,
                     const int newType,
                     const QColor lowColor = QColor("white"),
                     const QColor highColor = QColor("white"));

    void autoScale(bool en = false);
    void setTimePerVec(double t);
    void setUpdateTime(double t);

private slots:
    void newData(const QEvent *updateEvent);
    void onPlotPointSelected(const QPointF p);

private:
    QIntValidator *d_int_validator;

    double d_samp_rate, d_center_freq;
    int d_vecsize;
    double d_time_per_vec;
    float d_vecavg;
    double d_units;

    bool d_clicked;
    double d_clicked_freq;

    double d_min_val, d_cur_min_val;
    double d_max_val, d_cur_max_val;

    VecSizeMenu *d_sizemenu;
    AverageMenu *d_avgmenu;
};

#endif /* WATERFALL_VECTOR_DISPLAY_FORM_H */
