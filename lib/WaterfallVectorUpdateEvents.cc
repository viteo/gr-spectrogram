/* -*- c++ -*- */
/*
 * Copyright 2008-2013 Free Software Foundation, Inc.
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

#ifndef SPECTRUM_UPDATE_EVENTS_C
#define SPECTRUM_UPDATE_EVENTS_C

#include <spectrogram/WaterfallVectorUpdateEvents.h>

WaterfallUpdateEvent::WaterfallUpdateEvent(const std::vector<double *> dataPoints,
                                           const uint64_t numDataPoints,
                                           const gr::high_res_timer_type dataTimestamp)
    : QEvent(QEvent::Type(SpectrumUpdateEventType))
{
    if (numDataPoints < 1)
    {
        _numDataPoints = 1;
    }
    else
    {
        _numDataPoints = numDataPoints;
    }

    _nplots = dataPoints.size();
    for (size_t i = 0; i < _nplots; i++)
    {
        _dataPoints.push_back(new double[_numDataPoints]);
        if (numDataPoints > 0)
        {
            memcpy(_dataPoints[i], dataPoints[i], _numDataPoints * sizeof(double));
        }
    }

    _dataTimestamp = dataTimestamp;
}

WaterfallUpdateEvent::~WaterfallUpdateEvent()
{
    for (size_t i = 0; i < _nplots; i++)
    {
        delete[] _dataPoints[i];
    }
}

const std::vector<double *> WaterfallUpdateEvent::getPoints() const { return _dataPoints; }

uint64_t WaterfallUpdateEvent::getNumDataPoints() const { return _numDataPoints; }

gr::high_res_timer_type WaterfallUpdateEvent::getDataTimestamp() const
{
    return _dataTimestamp;
}

/***************************************************************************/

SetFreqEvent::SetFreqEvent(const double centerFreq, const double bandwidth)
    : QEvent(QEvent::Type(SpectrumFrequencyRangeEventType))
{
    _centerFrequency = centerFreq;
    _bandwidth = bandwidth;
}

SetFreqEvent::~SetFreqEvent() {}

double SetFreqEvent::getCenterFrequency() const { return _centerFrequency; }

double SetFreqEvent::getBandwidth() const { return _bandwidth; }




#endif /* SPECTRUM_UPDATE_EVENTS_C */
