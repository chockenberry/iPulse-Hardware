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

@property (class, nonatomic, readonly, nullable) NSString *defaultBsdPath;

@property (nonatomic, strong, readonly, nullable) NSString *bsdPath;
@property (nonatomic, assign, readonly) BOOL isOpen;

- (_Nonnull instancetype)initWithBsdPath:(NSString * _Nonnull)bsdPath;

- (BOOL)open;
- (void)close;

- (void)send:(NSString * _Nonnull)message;
- (NSString * _Nullable)read;

@end

#endif /* Serial_h */
