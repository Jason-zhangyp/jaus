////////////////////////////////////////////////////////////////////////////////////
///
///  \file ReportStillImageSensorCapabilities.cpp
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber, Jonathan Harris
///  <br>Created: 20 March 2013
///  <br>Copyright (c) 2013
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu, jharris@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
///  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
///  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
///  DISCLAIMED. IN NO EVENT SHALL UCF BE LIABLE FOR ANY
///  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
///  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
///  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
///  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
///  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
///  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
////////////////////////////////////////////////////////////////////////////////////
#include "jaus/environment/stillimage/ReportStillImageSensorCapabilities.h"

using namespace JAUS;

ReportStillImageSensorCapabilities::ReportStillImageSensorCapabilities(const Address& dest, const Address& src) : 
Message(REPORT_STILL_IMAGE_SENSOR_CAPABILITIES, dest, src)
{
    mSensorList = List();
}

ReportStillImageSensorCapabilities::ReportStillImageSensorCapabilities(const ReportStillImageSensorCapabilities& message) : 
Message(REPORT_STILL_IMAGE_SENSOR_CAPABILITIES)
{
    *this = message;
}

ReportStillImageSensorCapabilities::~ReportStillImageSensorCapabilities()
{
    mSensorList.clear();
}

int ReportStillImageSensorCapabilities::WriteMessageBody(Packet& packet) const
{
    UInt startPos = packet.GetWritePos();
    UShort count = (UShort)mSensorList.size();
    List::const_iterator sensor = mSensorList.begin();
    packet.Write(count);
    while(sensor != mSensorList.end())
    {
        sensor->WriteMessageBody(packet);
        sensor++;
    }
    return packet.GetWritePos() - startPos;
}

int ReportStillImageSensorCapabilities::ReadMessageBody(const Packet& packet)
{
    UInt startPos = packet.GetReadPos();
    UShort count = 0;
    packet.Read(count);
    for(int i = 0; i < (int)count; i++)
    {
        StillImageSensorCapabilities sisc = StillImageSensorCapabilities();
        sisc.ReadMessageBody(packet);
        mSensorList.push_back(sisc);
    }
    return packet.GetReadPos() - startPos;
}

void ReportStillImageSensorCapabilities::ClearMessageBody()
{
    mSensorList.clear();
}

bool ReportStillImageSensorCapabilities::IsLargeDataSet(const UInt maxPayloadSize) const
{
    //Quick Check to possibly avoid the time consuming loop (assume worst case of all fields supported)
    if(maxPayloadSize > (BYTE_SIZE * 1 + USHORT_SIZE * 2 + UINT_SIZE * 1) * mSensorList.size())
    {
        return false;
    }

    UInt currentSize = 0;
    List::const_iterator iter = mSensorList.begin();
    while(iter != mSensorList.end())
    {
        currentSize += iter->GetSize();        
        iter++;
    }
    
    return currentSize > maxPayloadSize;
}

void ReportStillImageSensorCapabilities::PrintMessageBody() const
{
    std::cout << "Sensor Count: " << mSensorList.size() << std::endl;
    UShort count = (UShort)mSensorList.size();
    List::const_iterator sensor = mSensorList.begin();
    while(sensor != mSensorList.end())
    {
        std::cout << "<Sensor>" <<  std::endl;
        sensor->PrintSensorFields();
        std::cout << "</Sensor>" <<  std::endl;
        sensor++;
    }
}

/*  End of File */
