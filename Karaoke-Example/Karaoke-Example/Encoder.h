//
//  Encoder.h
//  Karaoke-Example
//
//  Created by thealphanerd on 3/6/14.
//  Copyright (c) 2014 thealphanerd. All rights reserved.
//

#import <Foundation/Foundation.h>
//https://stackoverflow.com/questions/6260160/how-can-i-save-array-of-samples-as-audio-file-in-iphone
@interface Encoder : NSObject {
    NSString *someProperty;
}

@property (nonatomic, retain) NSString *someProperty;

+(id)sharedEncoder:(NSData *)data;

-(void)RetrieveWav;

@end
