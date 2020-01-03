/* -*- c++ -*- */
/*
 * Copyright 2008-2014 Free Software Foundation, Inc.
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

#ifndef WATERFALL_VECTOR_UPDATE_EVENTS_H
#define WATERFALL_VECTOR_UPDATE_EVENTS_H

#include <gnuradio/high_res_timer.h>
#include <spectrogram/api.h>
#include <gnuradio/tags.h>
#include <stdint.h>
#include <QEvent>
#include <QString>
#include <complex>
#include <vector>

static const int SpectrumUpdateEventType = 10005;
static const int SpectrumWindowCaptionEventType = 10008;
static const int SpectrumWindowResetEventType = 10009;
static const int SpectrumFrequencyRangeEventType = 10010;

class SPECTROGRAM_API WaterfallUpdateEvent : public QEvent
{
public:
    WaterfallUpdateEvent(const std::vector<double *> dataPoints,
                         const uint64_t numDataPoints,
                         const gr::high_res_timer_type dataTimestamp);

    ~WaterfallUpdateEvent();

    int which() const;
    const std::vector<double *> getPoints() const;
    uint64_t getNumDataPoints() const;
    bool getRepeatDataFlag() const;

    gr::high_res_timer_type getDataTimestamp() const;

    static QEvent::Type Type() { return QEvent::Type(SpectrumUpdateEventType); }

protected:
private:
    size_t _nplots;
    std::vector<double *> _dataPoints;
    uint64_t _numDataPoints;

    gr::high_res_timer_type _dataTimestamp;
};

/********************************************************************/

class SPECTROGRAM_API SetFreqEvent : public QEvent
{
public:
    SetFreqEvent(const double, const double);
    ~SetFreqEvent();
    double getCenterFrequency() const;
    double getBandwidth() const;

private:
    double _centerFrequency;
    double _bandwidth;
};

#endif /* WATERFALL_VECTOR_UPDATE_EVENTS_H */
