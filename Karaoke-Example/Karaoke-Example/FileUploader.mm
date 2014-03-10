//
//  FileUploader.m
//  Karaoke-Example
//
//  Created by thealphanerd on 3/6/14.
//  Copyright (c) 2014 thealphanerd. All rights reserved.
//

#import "FileUploader.h"
#import "Globals.h"

@implementation FileUploader

@synthesize someProperty;

#pragma mark Singleton Methods

+ (id)sharedUploader {
    static FileUploader *shareduploader = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        shareduploader = [[self alloc] init];
    });
    return shareduploader;
}

- (void)uploadData:(NSData *)data
{
    NSLog(@"OMG");
}

- (id)init {
    if (self = [super init]) {
        someProperty = @"Default Property Value";
    }
    return self;
}

- (void)dealloc {
    // Should never be called, but just here for clarity really.
}


@end
