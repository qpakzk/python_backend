// Copyright (c) 2021, NVIDIA CORPORATION. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "triton/core/tritonserver.h"

#include <string>
#include <memory>
#include <vector>
#include <pthread.h>

namespace triton { namespace backend { namespace python {

// Create a conditional variable.
void CreateIPCCondVariable(pthread_cond_t** cv);

// Create a mutex that is shared between different processes.
void CreateIPCMutex(pthread_mutex_t** mutex);

//
// Represents a raw data 
//
struct RawData {
    off_t memory_ptr;
    TRITONSERVER_MemoryType memory_type;
    int memory_type_id;
    uint64_t byte_size;
};

//
// Represents a Tensor object that will be passed to Python code.
//
struct Tensor {
    off_t raw_data; // Offset for raw data field.
    off_t name; // Offset for name field.
    TRITONSERVER_DataType dtype;
    off_t dims; // Shared memory offset for the dimensions.
    size_t dims_count;
};

struct String {
    off_t data;
    size_t length;
};

//
// Inference Request
//
struct Request {
    off_t id; // Offset for the id field.
    uint64_t correlation_id;
    off_t inputs; // Offset for input field.
    uint32_t requested_input_count;
    off_t requested_output_names; // Offset for the requested output names
    uint32_t requested_output_count;
};

struct Response {
    off_t outputs; // Offset for Tensor output.
    uint32_t outputs_size;
};

struct ResponseBatch {
    off_t reponses; // Offset for response object.
    uint32_t batch_size;
};

struct RequestBatch {
    off_t requests; // Offset for request object.
    uint32_t batch_size;
};

struct IPCMessage {
    // request points to a RequestBatch struct.
    off_t request_batch;

    // response points to a ResponseBatch struct.
    off_t response_batch;
};

}}}  // namespace triton::backend::python
