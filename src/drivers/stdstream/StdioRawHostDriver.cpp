/**
 * This file is part of SerialNet.
 *
 *  SerialNet is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SerialNet is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SerialNet.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * StdstreamOpipeHostDriver.cpp
 *
 *  Created on: 14 aug. 2016
 *      Author: mikaelr
 */

#include "StdioRawHostDriver.h"

#include "hal/PosixIf.h"
#include "utility/Log.h"
#include <unistd.h>

StdioRawHostDriver::StdioRawHostDriver(PosixFileIf* posixIf)
    : m_rxAddr(LocalAddress::null_addr), m_destAddr(LocalAddress::null_addr),
      m_txHandler(nullptr), m_posixIf(posixIf)
{
}

StdioRawHostDriver::~StdioRawHostDriver()
{
}

void
StdioRawHostDriver::startStdout(LocalAddress rxAddress)
{
    m_rxAddr = rxAddress;
}

void
StdioRawHostDriver::startStdin(LocalAddress destAddr, MsgHostIf* txIf,
                               EventLoop& mainLoop)
{
    m_destAddr = destAddr;
    m_txHandler = txIf;
    setupCallback(mainLoop);
}

void
StdioRawHostDriver::writeToNet(MsgHostIf::HostPkt packet)
{
    if (m_txHandler && m_destAddr != LocalAddress::null_addr)
    {
        m_txHandler->msgHostTx_sendPacket(packet, m_destAddr,
                                          ChannelType::raw_stream);
    }
}

void
StdioRawHostDriver::setupCallback(EventLoop& mainLoop)
{
    // we'd like to be notified when input is available on stdin
    mainLoop.onReadable(STDIN_FILENO, [this]() -> bool {
        // Enough to keep an ethernet frame.
        const constexpr int maxRead = 2000;
        gsl::byte buffer[maxRead];
        ssize_t dataRead;
        dataRead = m_posixIf->read(STDIN_FILENO, buffer, maxRead);
        if (dataRead > 0)
        {
            writeToNet(MsgHostIf::HostPkt(buffer, dataRead));
        }
        // return true, so that we also return future read events
        return true;
    });
}

void
StdioRawHostDriver::packetReceivedFromNet(const ByteVec& data,
                                          LocalAddress srcAddr,
                                          LocalAddress destAddr,
                                          ChannelType chType)
{
    m_posixIf->write(STDOUT_FILENO, &data[0], data.size());
}
