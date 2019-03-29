//
//  BRChainParams.h
//
//  Created by Aaron Voisine on 1/10/18.
//  Copyright (c) 2019 breadwallet LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#ifndef BRChainParams_h
#define BRChainParams_h

#include "BRMerkleBlock.h"
#include <assert.h>

typedef struct {
    uint32_t height;
    UInt256 hash;
    uint32_t timestamp;
    uint32_t target;
} BRCheckPoint;

typedef struct {
    const char **dnsSeeds; // NULL terminated array of dns seeds
    uint16_t standardPort;
    uint32_t magicNumber;
    uint64_t services;
    int (*verifyDifficulty)(const BRMerkleBlock *block, const BRMerkleBlock *previous, uint32_t transitionTime);
    const BRCheckPoint *checkpoints;
    size_t checkpointsCount;
} BRChainParams;

static const char *BRMainNetDNSSeeds[] = {
    "explorer.iop.cash.", "mainnet.iop.cash.", "main1.iop.cash.", "main2.iop.cash.", "main3.iop.cash.", 
    "main4.iop.cash.", "main5.iop.cash.", "mainnet.iop.global.",NULL
};

static const char *BRTestNetDNSSeeds[] = {
    "testnet.iop.cash.", "test1.iop.cash.", "test2.iop.cash.", "testnet.iop.global.",NULL
};

// blockchain checkpoints - these are also used as starting points for partial chain downloads, so they must be at
// difficulty transition boundaries in order to verify the block difficulty at the immediately following transition
static const BRCheckPoint BRMainNetCheckpoints[] = {
    {      0, uint256("00000000bf5f2ee556cb9be8be64e0776af14933438dbb1af72c41bfb6c82db3"), 1463452181, 0x1d00ffff},
    {  20160, uint256("0000000004c386ce4ba68926a6c5a91c5332127bb7b9e940dbcb1f6789a9aaf7"), 1484253352, 0x1c09fd72},
    {  40320, uint256("0000000007f8e37570026a9219613f1f1122f4273976187e27a21d0018abf997"), 1497363978, 0x1c0cfa29},
    {  60480, uint256("000000000186215285c3164974a6be1e3f67e40dadf5bc5fa77c3134457f1218"), 1507851127, 0x1c018d5a},
    {  80640, uint256("0000000000167332516b8258b34139a8ddaa3ec3ef8be37cf15fd880bc59c1e2"), 1517294643, 0x1b1dce77},
    { 100800, uint256("0000000000062f145230e99035d2bd905b7484b0f5fbbc216f0ef4f52e6ccbf5"), 1528894997, 0x1b1306e6},
    { 120960, uint256("00000000000626d833f02392bb0e7efce81bbaeb6bc4cb3c9342e71afca6e50f"), 1540127974, 0x1b08de0f},
};

static const BRCheckPoint BRTestNetCheckpoints[] = {
    {      0, uint256("000000006f2bb863230cda4f4fbee520314077e599a90b9c6072ea2018d7f3a3"), 1463452342, 0x1d00ffff},
    {  20160, uint256("000000004a21d988dfd5fd10001f7ec94c7f16e95f307c02010fd5f7ed1483dd"), 1520859326, 0x1d00ffff},
    {  40320, uint256("000000004fd1c98e61f6eeaaca1f17219ad62b6745416e03b3e1f9dc07c03b78"), 1532080436, 0x1d00e684},
    {  60480, uint256("0000000032a652f2e7e1fceb550f08e61ec5df00f18f224ddfd4c825ede42624"), 1536631371, 0x1c3fffc0},
    {  80640, uint256("0000000019a8c796084eb81c772f99f0e937ec917d37eee5795be7c3e491fbe7"), 1545781751, 0x1c1a749a},
    { 100800, uint256("000000009bf4d894fb86fe78f6504092d9d3a66b9f6bfe87f127025409f99e39"), 1552846675, 0x1d00b2ff},
};

static int BRTestNetVerifyDifficulty(const BRMerkleBlock *block, const BRMerkleBlock *previous, uint32_t transitionTime)
{
    int r = 1;
    
    assert(block != NULL);
    assert(previous != NULL);
    
    if (! previous || !UInt256Eq(block->prevBlock, previous->blockHash) || block->height != previous->height + 1) r = 0;
    if (r && (block->height % BLOCK_DIFFICULTY_INTERVAL) == 0 && transitionTime == 0) r = 0;
    return r;
}

static const BRChainParams BRMainNetParams = {
    BRMainNetDNSSeeds,
    4877,       // standardPort
    0xd3bbb0fd, // magicNumber
    0,          // services
    BRMerkleBlockVerifyDifficulty,
    BRMainNetCheckpoints,
    sizeof(BRMainNetCheckpoints)/sizeof(*BRMainNetCheckpoints)
};

static const BRChainParams BRTestNetParams = {
    BRTestNetDNSSeeds,
    7475,      // standardPort
    0xb350fcb1, // magicNumber
    0,          // services
    BRTestNetVerifyDifficulty,
    BRTestNetCheckpoints,
    sizeof(BRTestNetCheckpoints)/sizeof(*BRTestNetCheckpoints)
};

#endif // BRChainParams_h
