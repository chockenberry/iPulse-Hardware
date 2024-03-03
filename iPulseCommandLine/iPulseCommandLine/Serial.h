//
//  Serial.h
//  iPulseCommandLine
//
//  Created by Craig Hockenberry on 2/24/24.
//

#ifndef Serial_h
#define Serial_h

@import Foundation;

@interface Serial : NSObject

@property (nonatomic, strong, readonly) NSString *bsdPath;

- (instancetype)initWithBsdPath:(NSString *)bsdPath;

- (void)open;
- (void)send:(NSString *)message;
- (NSString *)read;
- (void)close;

@end

#endif /* Serial_h */
