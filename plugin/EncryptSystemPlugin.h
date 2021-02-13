//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// Copyright (C) 2014-2021 Simdsoft Limited.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions
// of the accompanying license agreement.
//*******************************************************************************
// Version: 10.0.9000.21
/*
The x-sutdio Pro EncryptSystemPlugin interface
The plugin should export API: "XSPESSIoControl"
*/
#pragma once

/// <summary>
/// The input control code
/// </summary>
enum {
    ES_ICC_PREENCRYPT = 1,
    ES_ICC_COMPUTE_FILE_CONTENT_HASH,
    ES_ICC_COMPUTE_FILE_NAME_HASH,
};

// The output control code, returned by the XSPESSIoControl
// when <= 0
// since: 10.0.9000.21
enum
{
    ES_OCC_DEFAULT = 0,
    ES_OCC_SKIP = -1001, // only works for ES_ICC_PREENCRYPT
};

/// <summary>
/// @returns:
///    > 0: the actual bytes have been write to outputBuffer
///   <= 0: Output Control Code: ES_OCC_DEFAULT or ES_OCC_SKIP
/// </summary>
typedef int (*PFNXSPESSIOCONTROL)(int controlCode, const void* lpInBuffer, int nInBufferSize, void* lpOutBuffer, int nOutBufferSize);
