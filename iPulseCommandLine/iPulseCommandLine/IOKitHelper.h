//
//  IOKitHelper.h
//  iPulseMobile
//
//  Created by Craig Hockenberry on 12/15/23.
//

#import <Foundation/Foundation.h>

#ifndef IOKitHelper_h
#define IOKitHelper_h

void querySerialDevice(char *path);

void queryGraphicsUtilization(double *outPercentage, UInt64 *outMemorySize);
void queryDiskActivity(UInt64 *outReadCount, UInt64 *outReadBytes, UInt64 *outWriteCount, UInt64 *outWriteBytes);

#endif /* IOKitHelper_h */
