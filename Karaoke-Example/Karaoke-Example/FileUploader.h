//
//  FileUploader.h
//  Karaoke-Example
//
//  Created by thealphanerd on 3/6/14.
//  Copyright (c) 2014 thealphanerd. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface FileUploader : NSObject {
    NSString *someProperty;
}

@property (nonatomic, retain) NSString *someProperty;

+(id)sharedUploader;

-(void)uploadData:(NSData *)data;

@end
