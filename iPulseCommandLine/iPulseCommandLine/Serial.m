//
//  Serial.m
//  iPulseCommandLine
//
//  Created by Craig Hockenberry on 2/24/24.
//

#import <Foundation/Foundation.h>

// for serial port communication
#include <IOKit/serial/IOSerialKeys.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>

@interface Serial : NSObject

@property (nonatomic, strong, readwrite) NSString *bsdPath;
@property (nonatomic, assign) int fileDescriptor;
@property (nonatomic, assign) struct termios originalAttributes;

@end

@implementation Serial

// NOTE: ioctl and fnctl can't be used from Swift. And with this kind of legacy code, who the hell would want to?

- (instancetype)initWithBsdPath:(NSString *)bsdPath
{
	if ((self = [super init])) {
		self.bsdPath = bsdPath;
		self.fileDescriptor = -1;
	}
	
	return self;
}

- (void)handleError:(NSString *)message
{
	if (self.fileDescriptor != -1) {
		close(self.fileDescriptor);
		self.fileDescriptor = -1;
	}
	
	NSLog(@"error: %@ on %@ - %s (%d)", message, self.bsdPath, strerror(errno), errno);
}

- (void)open
{
	NSLog(@"open: %@", self.bsdPath);
	
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
						
						NSLog(@"open: current input baud rate is %d\n", (int) cfgetispeed(&attributes));
						NSLog(@"open: current output baud rate is %d\n", (int) cfgetospeed(&attributes));
						
						// Set raw input (non-canonical) mode, with reads blocking until either a single character
						// has been received or a one second timeout expires.
						// See tcsetattr(4) ("man 4 tcsetattr") and termios(4) ("man 4 termios") for details.
						
						cfmakeraw(&attributes);
						attributes.c_cc[VMIN] = 1;
						attributes.c_cc[VTIME] = 10;
						
						// The baud rate, word length, and handshake options can be set as follows:
						
						cfsetspeed(&attributes, B9600);		// Set 9600 baud
						attributes.c_cflag |= (CS8 | 		// Use 8 bit words
											   PARENB);		// Parity enable (even parity if PARODD not also set)
						
						
						// Print the new input and output baud rates. Note that the IOSSIOSPEED ioctl interacts with the serial driver
						// directly bypassing the termios struct. This means that the following two calls will not be able to read
						// the current baud rate if the IOSSIOSPEED ioctl was used but will instead return the speed set by the last call
						// to cfsetspeed.
						
						NSLog(@"open: input baud rate changed to %d\n", (int) cfgetispeed(&attributes));
						NSLog(@"open: output baud rate changed to %d\n", (int) cfgetospeed(&attributes));
						
						// Cause the new options to take effect immediately.
						if (tcsetattr(self.fileDescriptor, TCSANOW, &attributes) != -1) {
							// success
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
}

- (void)send:(NSString *)message
{
	if (self.fileDescriptor != -1) {
		NSLog(@"send: '%@' to %@", message, self.bsdPath);
		
		NSData *data = [[message stringByAppendingString:@"\r"] dataUsingEncoding:NSASCIIStringEncoding allowLossyConversion:YES];
		ssize_t bytesWritten = write(self.fileDescriptor, data.bytes, data.length);
		if (bytesWritten != -1) {
			NSLog(@"send: wrote %ld bytes", bytesWritten);
		}
		else {
			[self handleError:@"sending message"];
		}
	}
}

- (void)close
{
	NSLog(@"close %@", self.bsdPath);
}

@end
