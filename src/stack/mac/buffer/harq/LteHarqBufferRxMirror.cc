//
//                           SimuLTE
//
// This file is part of a software released under the license included in file
// "license.pdf". This license can be also found at http://www.ltesimulator.com/
// The above file and the present reference are part of the software itself,
// and cannot be removed from it.
//

#include "LteHarqBufferRxMirror.h"
#include "LteHarqBufferRx.h"
#include "LteCommon.h"

LteHarqBufferRxMirror::LteHarqBufferRxMirror(LteHarqBufferRx* harqBuffer,unsigned char maxHarqRtx,MacNodeId peer)
{
    peerId_ = peer;
    numHarqProcesses_ = harqBuffer->getProcesses();
    processes_.resize(numHarqProcesses_);
    maxHarqRtx_ = maxHarqRtx;
    for(unsigned int j=0;j<numHarqProcesses_;++j)
    {
        processes_[j] = new LteHarqProcessRxMirror(j,maxHarqRtx);
        for(Codeword cw = 0; cw < MAX_CODEWORDS; ++cw)
        {
           processes_[j]->status_.at(cw) = harqBuffer->getProcess(j)->getUnitStatus(cw);
           processes_[j]->pdu_length_.at(cw) = harqBuffer->getProcess(j)->getByteLength(cw) ;
        }
    }

}

LteHarqBufferRxMirror::~LteHarqBufferRxMirror()
{
    std::vector<LteHarqProcessRxMirror*>::iterator it = processes_.begin();
    for(;it!=processes_.end();++it)
    {
        delete *it;
    }
}


void LteHarqBufferRxMirror::checkCorrupted()
{
    for (unsigned int i = 0; i < numHarqProcesses_; i++)
    {
        for (Codeword cw = 0; cw < MAX_CODEWORDS; ++cw)
        {
            if( processes_[i]->status_.at(cw) == RXHARQ_PDU_CORRUPTED )
                EV << NOW << " LteHarqBufferRxMirror::checkCorrupted - Process: " << i << " is in CORRUPTED state"<<endl;
        }
    }
}
