//
//  Encoder.m
//  Karaoke-Example
//
//  Created by thealphanerd on 3/6/14.
//  Copyright (c) 2014 thealphanerd. All rights reserved.
//

#import "Encoder.h"

@implementation Encoder

@synthesize someProperty;

+ (id)sharedEncoder:(NSData *)data
{
    static Encoder *sharedEncoder = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedEncoder = [[self alloc] init];
    });
    return sharedEncoder;
}

- (void)RetrieveWav
{
    //    https://stackoverflow.com/questions/6260160/how-can-i-save-array-of-samples-as-audio-file-in-iphone
    NSString *filePath = NSTemporaryDirectory();
    
    filePath = [filePath stringByAppendingPathComponent:@"audio_test.wav"];
    
    NSURL *fileURL = [NSURL fileURLWithPath:filePath];
    

}

@end
