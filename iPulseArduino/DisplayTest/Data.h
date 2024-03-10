// Data.h

#include <ArduinoJson.h>

#ifndef _Data_H_
#define _Data_H_

struct Data {
  float cpuUsage;  // value from 0.0 to 1.0 (inclusive)

  int64_t networkReceivedBps;
  int64_t networkSentBps;

  int64_t diskReadBytes;
  int64_t diskWriteBytes;

  int64_t memoryPhysicalSize;
  int64_t memoryWiredSize;
  int64_t memoryAppSize;
  int64_t memoryCompressedSize;

  float load;
  int32_t uptime;
};
typedef struct Data Data;
typedef struct Data *DataPtr;

void updateData(JsonDocument &document, DataPtr data) {
      data->cpuUsage = document["cpu_usage"];
      data->networkReceivedBps = document["network_received_bps"];
      data->networkSentBps = document["network_sent_bps"];
      data->diskReadBytes = document["disk_read_bytes"];
      data->diskWriteBytes = document["disk_write_bytes"];
      data->memoryPhysicalSize = document["memory_physical_size"];
      data->memoryWiredSize = document["memory_wired_size"];
      data->memoryAppSize = document["memory_app_size"];
      data->memoryCompressedSize = document["memory_compressed_size"];
      data->load = document["load_five_minutes"];
      data->uptime = document["uptime"];
}

#endif // _Data_H_