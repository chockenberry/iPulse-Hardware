//
//  IOKItHelper.m
//  iPulseMobile
//
//  Created by Craig Hockenberry on 12/15/23.
//

#import <Foundation/Foundation.h>

#import <IOKit/IOKitLib.h>
#import <IOKit/IOKitKeys.h>
#import <IOKit/graphics/IOGraphicsLib.h>
#import <IOKit/storage/IOBlockStorageDriver.h>
#import <IOKit/serial/IOSerialKeys.h>

#import <mach/mach.h>
#import <mach/mach_types.h>
#import <mach/mach_host.h>

#import "IOKitHelper.h"

// NOTE: IOKit is not supported by Swift (understandable!). These functions provide some basic information to the Collector
// and Serial classes.

// NOTE: It's very cool that IOMasterPort was deprecated and replaced with IOMainPort - it's never easy to do the right thing,
// especially when it's embedded in layers and layers of legacy code.

void querySerialDevice(char *path) {
	mach_port_t port;
	IOMainPort(MACH_PORT_NULL, &port);

	// NOTE: Query in shell with: ioreg -c "IOSerialBSDClient" -l -r -w 0
	
	// find the first "usbmodem" device path and return it
	
	io_iterator_t iterator;
	if (IOServiceGetMatchingServices(port, IOServiceMatching(kIOSerialBSDServiceValue), &iterator) == kIOReturnSuccess) {
		for (io_registry_entry_t entry = IOIteratorNext(iterator); entry; entry = IOIteratorNext(iterator)) {
			CFTypeRef calloutDeviceRef = IORegistryEntryCreateCFProperty(entry, CFSTR(kIOCalloutDeviceKey), kCFAllocatorDefault, 0);
			if (calloutDeviceRef) {
				const char *devicePath = CFStringGetCStringPtr(calloutDeviceRef, kCFStringEncodingUTF8);
				if (devicePath) {
					if (strstr(devicePath, "usbmodem") != NULL) {
						strncpy(path, devicePath, MAXPATHLEN);

						CFRelease(calloutDeviceRef);
						IOObjectRelease(entry);
						IOObjectRelease(iterator);

						return;
					}
				}
				CFRelease(calloutDeviceRef);
			}
			
			IOObjectRelease(entry);
		}
	}
	IOObjectRelease(iterator);
	
	path[0] = 0;
}

void queryGraphicsUtilization(double *outPercentage, UInt64 *outMemorySize) {
	mach_port_t port;
	IOMainPort(MACH_PORT_NULL, &port);

	// NOTE: Query in shell with: ioreg -c "IOAccelerator" -l -r -w 0

	double percentage = 0;	
	UInt64 memorySize = 0;

	io_iterator_t iterator;
	if (IOServiceGetMatchingServices(port, IOServiceMatching(kIOAcceleratorClassName), &iterator) == kIOReturnSuccess) {
		for (io_registry_entry_t entry = IOIteratorNext(iterator); entry; entry = IOIteratorNext(iterator)) {
			CFMutableDictionaryRef entryProperties;
			if (IORegistryEntryCreateCFProperties(entry, &entryProperties, kCFAllocatorDefault, kNilOptions) != kIOReturnSuccess) {
				IOObjectRelease(entry);
				continue;
			}
			else {
				NSDictionary *properties = (__bridge NSDictionary *)entryProperties;
				{
					id property = properties[@"Device Utilization %"];
					if (property != nil && [property isKindOfClass:[NSNumber class]]) {
						NSNumber *deviceUtilization = (NSNumber *)property;
						percentage = deviceUtilization.doubleValue;
					}
				}
				{
					id property = properties[@"In use system memory"];
					if (property != nil && [property isKindOfClass:[NSNumber class]]) {
						NSNumber *memoryUsage = (NSNumber *)property;
						memorySize = memoryUsage.unsignedLongLongValue;
					}
				}
				
				CFRelease(entryProperties);
				IOObjectRelease(entry);
				break;
			}
		}
		IOObjectRelease(iterator);
	}
	
	mach_port_deallocate(mach_task_self(), port);

	if (outPercentage != nil) {
		*outPercentage = percentage;
	}
	if (outMemorySize != nil) {
		*outMemorySize = memorySize;
	}
}

void queryDiskActivity(UInt64 *outReadCount, UInt64 *outReadBytes, UInt64 *outWriteCount, UInt64 *outWriteBytes) {
	mach_port_t port;
	IOMainPort(MACH_PORT_NULL, &port);

	// NOTE: Query in shell with: ioreg -c "IOBlockStorageDriver" -l -r -w 0
	
	UInt64 readCount = 0;
	UInt64 readBytes = 0;
	UInt64 writeCount = 0;
	UInt64 writeBytes = 0;
	
	io_iterator_t iterator;
	
	if (IOServiceGetMatchingServices(port, IOServiceMatching(kIOBlockStorageDriverClass), &iterator) == kIOReturnSuccess) {
		io_registry_entry_t entry;
		while ((entry = IOIteratorNext(iterator))) {
			CFMutableDictionaryRef entryProperties;
			if (IORegistryEntryCreateCFProperties(entry, &entryProperties, kCFAllocatorDefault, kNilOptions) == kIOReturnSuccess) {
				CFDictionaryRef statisticsProperties = (CFDictionaryRef)CFDictionaryGetValue(entryProperties, CFSTR(kIOBlockStorageDriverStatisticsKey));
				if (statisticsProperties != NULL) {
					NSNumber *reads = (__bridge NSNumber *)CFDictionaryGetValue(statisticsProperties, CFSTR(kIOBlockStorageDriverStatisticsReadsKey));
					if (reads != nil) {
						readCount += reads.unsignedLongLongValue;
					}
					NSNumber *bytesRead = (__bridge NSNumber *)CFDictionaryGetValue(statisticsProperties, CFSTR(kIOBlockStorageDriverStatisticsBytesReadKey));
					if (bytesRead != nil) {
						readBytes += bytesRead.unsignedLongLongValue;
					}
					NSNumber *writes = (__bridge NSNumber *)CFDictionaryGetValue(statisticsProperties, CFSTR(kIOBlockStorageDriverStatisticsWritesKey));
					if (writes != nil) {
						writeCount += writes.unsignedLongLongValue;
					}
					NSNumber *bytesWritten = (__bridge NSNumber *)CFDictionaryGetValue(statisticsProperties, CFSTR(kIOBlockStorageDriverStatisticsBytesWrittenKey));
					if (bytesWritten != nil) {
						writeBytes += bytesWritten.unsignedLongLongValue;
					}
					CFRelease(entryProperties);
				}
			}
			IOObjectRelease(entry);
		}
		IOObjectRelease(iterator);
	}

	mach_port_deallocate(mach_task_self(), port);
	
	if (outReadCount != nil) {
		*outReadCount = readCount;
	}
	if (outReadBytes != nil) {
		*outReadBytes = readBytes;
	}
	if (outWriteCount != nil) {
		*outWriteCount = writeCount;
	}
	if (outWriteBytes != nil) {
		*outWriteBytes = writeBytes;
	}
}
