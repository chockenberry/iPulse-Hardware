//
//  Serial.h
//  iPulseCommandLine
//
//  Created by Craig Hockenberry on 2/24/24.
//

#ifndef Serial_h
#define Serial_h

@import Foundation;

extern NSNotificationName const _Nonnull SerialStateDidChangeNotification;

@interface Serial : NSObject

@property (nonatomic, strong, readonly) NSString *bsdPath;
@property (nonatomic, assign, readonly) BOOL isOpen;

- (instancetype)initWithBsdPath:(NSString *)bsdPath;

- (void)open;
- (void)close;

- (void)send:(NSString *)message;
- (NSString *)read;

@end

#endif /* Serial_h */
