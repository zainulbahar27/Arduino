/*****************************************************************************************************
 * UltrasonicDistanceSensor.h - Arduino library for retrieving data from ultrasonic distance sensors *
 * Copyright 2011-2012 Jeroen Doggen (jeroendoggen@gmail.com)                                        *
 *****************************************************************************************************
 * Version History:                                                                                  *
 *  Version 0.1:                                                                                     *
 *                                                                                                   *
 * Roadmap:                                                                                          *
 *  Version 0.2: ??                                                                                  *                                                                               
 *****************************************************************************************************
 * This library is free software; you can redistribute it and/or                                     *
 * modify it under the terms of the GNU Lesser General Public                                        *
 * License as published by the Free Software Foundation; either                                      *
 * version 2.1 of the License, or (at your option) any later version.                                *
 *                                                                                                   *
 * This library is distributed in the hope that it will be useful,                                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                                    *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                                 *
 * Lesser General Public License for more details.                                                   *
 *                                                                                                   *
 * You should have received a copy of the GNU Lesser General Public                                  *
 * License along with this library; if not, write to the Free Software                               *
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA                        *
 ****************************************************************************************************/

#ifndef UltrasonicDistanceSensor_h
#define UltrasonicDistanceSensor_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <pins_arduino.h>
#endif

#define __cxa_pure_virtual()

#include <DistanceSensor.h>

class UltrasonicDistanceSensor : public DistanceSensor
{
	public:
		virtual int getDistanceTime()=0;
		
		void begin();
		void begin(int echoPin, int trigPin);

		int getDistanceCentimeter();
	
		void setAveraging(int avg); 

	protected:
		int _trigPin;
		int _echoPin;
		int _average;

};
#endif