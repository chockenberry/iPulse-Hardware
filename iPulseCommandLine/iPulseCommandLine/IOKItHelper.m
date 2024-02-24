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

#import <mach/mach.h>
#import <mach/mach_types.h>
#import <mach/mach_host.h>

#import "IOKitHelper.h"

double queryGraphicsUtilization(void) {
	mach_port_t port;
	IOMainPort(MACH_PORT_NULL, &port);
	
	double result = 0;
	
	io_iterator_t iterator;

	// NOTE: Query in shell with: ioreg -c "IOAccelerator" -l -r -w 0
	
	if (IOServiceGetMatchingServices(port, IOServiceMatching(kIOAcceleratorClassName), &iterator) == kIOReturnSuccess) {
		for (io_registry_entry_t regEntry = IOIteratorNext(iterator); regEntry; regEntry = IOIteratorNext(iterator)) {
			CFMutableDictionaryRef serviceDictionary;
			if (IORegistryEntryCreateCFProperties(regEntry, &serviceDictionary, kCFAllocatorDefault, kNilOptions) != kIOReturnSuccess) {
				IOObjectRelease(regEntry);
				continue;
			}
			else {
				NSDictionary *properties = (__bridge NSDictionary *)serviceDictionary;
				id property = properties[@"Device Utilization %"];
				if (property != nil && [property isKindOfClass:[NSNumber class]]) {
					NSNumber *gpuUtilization = (NSNumber *)property;
					result = gpuUtilization.doubleValue;
				}
				
				CFRelease(serviceDictionary);
				IOObjectRelease(regEntry);
				break;
			}

		}
		IOObjectRelease(iterator);
	}
	
	return result;
}
