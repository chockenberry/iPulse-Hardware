//
//  Serial.m
//  iPulseCommandLine
//
//  Created by Craig Hockenberry on 2/24/24.
//

#import <Foundation/Foundation.h>

// for serial port communication
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>

// for IOKit
#import <IOKit/IOKitLib.h>
#import <IOKit/IOKitKeys.h>
#import <IOKit/serial/IOSerialKeys.h>

#import <mach/mach.h>
#import <mach/mach_types.h>
#import <mach/mach_host.h>

#import "Serial.h"

//#import "IOKitHelper.h"

// NOTE: This code originally used IOPMAssertions to check for sleep/wake as described here:
// https://developer.apple.com/library/archive/qa/qa1340/_index.html
// There doesn't appear to be any benefit to using these instead of the normal NSWorkspace notifications
// (e.g. there is no information about Dark Wake/Power Nap).

NSNotificationName const SerialStateDidChangeNotification = @"SerialStateDidChangeNotification";

@interface Serial ()

@property (nonatomic, strong, readwrite) NSString *bsdPath;
@property (nonatomic, assign) int fileDescriptor;
@property (nonatomic, assign) struct termios originalAttributes;
@property (nonatomic, assign) BOOL needsReopen;

@end

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
	
	mach_port_deallocate(mach_task_self(), port);

	path[0] = 0;
}

@implementation Serial

// NOTE: ioctl and fnctl can't be used from Swift. And with this kind of legacy code, who the hell would want to?

+ (NSString *)defaultBsdPath
{
	char bsdPath[MAXPATHLEN];
	querySerialDevice(bsdPath);
	if (strlen(bsdPath) > 0) {
		NSString *result = [NSString stringWithUTF8String:bsdPath];
		return result;
	}
	return nil;
}

- (instancetype)initWithBsdPath:(NSString * _Nonnull)bsdPath
{
	NSLog(@"%s called", __PRETTY_FUNCTION__);
	
	if ((self = [super init])) {
		self.bsdPath = bsdPath;
		self.fileDescriptor = -1;
		self.needsReopen = NO;
	}
	
	return self;
}

- (void)dealloc
{
	NSLog(@"%s called", __PRETTY_FUNCTION__);
}

- (BOOL)isOpen
{
	return (self.fileDescriptor != -1);
}

- (void)handleError:(NSString *)message
{
	NSLog(@"error: %@ on %@ - %s (errno = %d)", message, self.bsdPath, strerror(errno), errno);

	if (self.fileDescriptor != -1) {
		close(self.fileDescriptor);
		self.fileDescriptor = -1;
		
		[NSNotificationCenter.defaultCenter postNotificationName:SerialStateDidChangeNotification object:self];
	}
}

- (BOOL)open
{
	//NSLog(@"open: %@", self.bsdPath);
	
	// Open the serial port read/write, with no controlling terminal, and don't wait for a connection.
	// The O_NONBLOCK flag also causes subsequent I/O on the device to be non-blocking.
	// See open(2) ("man 2 open") for details.

	self.fileDescriptor = open(self.bsdPath.UTF8String, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (self.fileDescriptor != -1) {
		// Note that open() follows POSIX semantics: multiple open() calls to the same file will succeed
		// unless the TIOCEXCL ioctl is issued. This will prevent additional opens except by root-owned
		// processes.
		// See tty(4) ("man 4 tty") and ioctl(2) ("man 2 ioctl") for details.
		
		if (ioctl(self.fileDescriptor, TIOCEXCL) != -1) {
			// setup non-blocking i/o -- so read() will return -1 when no data is available from the serial connection
			
			if (fcntl(self.fileDescriptor, F_SETFL, O_NONBLOCK) != -1) {
				// turn off caching
				
				if (fcntl(self.fileDescriptor, F_NOCACHE, 1) != -1) {
					// Get the current options and save them so we can restore the default settings later.
					
					struct termios attributes;
					if (tcgetattr(self.fileDescriptor, &attributes) != -1) {
						self.originalAttributes = attributes;
						
						// Print the current input and output baud rates.
						// See tcsetattr(4) ("man 4 tcsetattr") for details.
						
						//NSLog(@"open: current input baud rate is %d\n", (int) cfgetispeed(&attributes));
						//NSLog(@"open: current output baud rate is %d\n", (int) cfgetospeed(&attributes));
						
						// Set raw input (non-canonical) mode, with reads blocking until either a single character
						// has been received or a one second timeout expires.
						// See tcsetattr(4) ("man 4 tcsetattr") and termios(4) ("man 4 termios") for details.
						
						cfmakeraw(&attributes);
						attributes.c_cc[VMIN] = 1;
						attributes.c_cc[VTIME] = 10;
						
						// The baud rate, word length, and handshake options can be set as follows:
						
						cfsetspeed(&attributes, B115200);	// Set 9600 baud
						attributes.c_cflag |= (CS8 | 		// Use 8 bit words
											   PARENB);		// Parity enable (even parity if PARODD not also set)
						
						//attributes.c_lflag |= ICANON | ISIG;	// Canonical input
						//attributes.c_lflag &= ~(ECHO | ECHOE | ECHONL | IEXTEN);
						//attributes.c_lflag &= ~(ECHO);
						
						//attributes.c_oflag &= ~OPOST;

						// Print the new input and output baud rates. Note that the IOSSIOSPEED ioctl interacts with the serial driver
						// directly bypassing the termios struct. This means that the following two calls will not be able to read
						// the current baud rate if the IOSSIOSPEED ioctl was used but will instead return the speed set by the last call
						// to cfsetspeed.
						
						//NSLog(@"open: input baud rate changed to %d\n", (int) cfgetispeed(&attributes));
						//NSLog(@"open: output baud rate changed to %d\n", (int) cfgetospeed(&attributes));
						
						// Cause the new options to take effect immediately.
						if (tcsetattr(self.fileDescriptor, TCSANOW, &attributes) != -1) {
							// success
							[NSNotificationCenter.defaultCenter postNotificationName:SerialStateDidChangeNotification object:self];
							return YES;
						}
						else {
							[self handleError:@"setting tty attributes"];
						}
					}
					else {
						[self handleError:@"getting tty attributes"];
					}
				}
				else {
					[self handleError:@"setting F_NOCACHE"];
				}
			}
			else {
				[self handleError:@"setting O_NONBLOCK"];
			}
		}
		else {
			[self handleError:@"setting TIOCEXCL"];
		}
	}
	else {
		[self handleError:@"opening serial port"];
	}
	
	return NO;
}

- (void)send:(NSString * _Nonnull)message
{
	if (self.fileDescriptor != -1) {
		//NSLog(@"send: '%@' to %@", message, self.bsdPath);
		
		NSData *data = [[message stringByAppendingString:@"\r"] dataUsingEncoding:NSASCIIStringEncoding allowLossyConversion:YES];
		ssize_t bytesWritten = write(self.fileDescriptor, data.bytes, data.length);
		if (bytesWritten != -1) {
			//NSLog(@"send: wrote %ld bytes", bytesWritten);
		}
		else {
			[self handleError:@"sending message"];
		}
	}
}

- (NSString * _Nullable)read
{
	if (self.fileDescriptor != -1) {
		NSMutableString *output = [NSMutableString new];
		char buffer[256];
		
		BOOL done = NO;
		while (!done) {
			size_t bufferSize = sizeof(buffer);
			bzero(buffer, bufferSize);
			ssize_t bytesRead = read(self.fileDescriptor, buffer, bufferSize);
			if (bytesRead > 0) {
				NSString *string = [[NSString alloc] initWithBytes:buffer length:bytesRead encoding:NSUTF8StringEncoding];
				[output appendString:string];
			}
			else {
				done = true;
			}
			if (bytesRead == -1) {
				if (errno != EAGAIN) {
					NSLog(@"read: error = %d (%s)", errno, strerror(errno));
				}
			}
			else {
				//NSLog(@"read: bytesRead = %zd", bytesRead);
			}
		}
		
		if (output.length > 0) {
			return [output copy];
		}
		else {
			return nil;
		}
	}
	
	return nil;
}

- (void)close
{
	if (self.fileDescriptor != -1) {
		//NSLog(@"close %@", self.bsdPath);

		// Block until all written output has been sent from the device.
		// Note that this call is simply passed on to the serial device driver.
		// See tcsendbreak(3) ("man 3 tcsendbreak") for details.
		if (tcdrain(self.fileDescriptor) == -1) {
			NSLog(@"close: error waiting for drain on %@ - %s (%d)", self.bsdPath, strerror(errno), errno);
		}
		
		// Traditionally it is good practice to reset a serial port back to
		// the state in which you found it. This is why the original termios struct
		// was saved.
		
		struct termios attributes = self.originalAttributes;
		if (tcsetattr(self.fileDescriptor, TCSANOW, &attributes) == -1) {
			NSLog(@"close: error resetting tty attributes on %@ - %s (%d)", self.bsdPath, strerror(errno), errno);
		}

		close(self.fileDescriptor);
		self.fileDescriptor = -1;
		
		[NSNotificationCenter.defaultCenter postNotificationName:SerialStateDidChangeNotification object:self];
	}
}

@end
