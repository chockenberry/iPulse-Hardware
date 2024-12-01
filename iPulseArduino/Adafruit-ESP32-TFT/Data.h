// Data.h

#include <ArduinoJson.h>

#ifndef _Data_H_
#define _Data_H_

const int volumeCount = 6;
const int volumeNameLength = 16;

struct Volume {
  bool inUse;
  char name[volumeNameLength + 1];

  int64_t totalBytes;
  int64_t availableBytes;
};
typedef struct Volume Volume;
typedef struct Volume *VolumePtr;

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

  Volume volumes[volumeCount];

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

  for (int i = 0; i < volumeCount; i++) {
    data->volumes[i].inUse = false;
    data->volumes[i].name[0] = 0;
    data->volumes[i].totalBytes = 0;
    data->volumes[i].availableBytes = 0;
  }

  JsonArray array = document["volumes"].as<JsonArray>();
  int volumeIndex = 0;
  for (JsonVariant item : array) {
    if (volumeIndex < volumeCount) {
      data->volumes[volumeIndex].inUse = true;
      strncpy(data->volumes[volumeIndex].name, item["name"], volumeNameLength);
      //strncpy(data->volumes[volumeIndex].name, item["name"], volumeNameLength);
      data->volumes[volumeIndex].totalBytes = item["total_bytes"];
      data->volumes[volumeIndex].availableBytes = item["available_bytes"];
    }
    volumeIndex += 1;
  }
}

#endif // _Data_H_