//
//  KaraokeViewController.m
//  Karaoke-Example
//
//  Created by thealphanerd on 3/4/14.
//  Copyright (c) 2014 thealphanerd. All rights reserved.
//

#import "KaraokeViewController.h"
#import "Globals.h"

@interface KaraokeViewController ()
@property (weak, nonatomic) IBOutlet UIButton *RecordButton;
@property (weak, nonatomic) IBOutlet UIButton *PlayButton;
- (void) togglePlayLabel;
@end

@implementation KaraokeViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(togglePlay:)
                                                 name:@"TogglePlay"
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(toggleRecord:)
                                                 name:@"ToggleRecord"
                                               object:nil];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (IBAction)RecordPressed:(UIButton *)sender {
    if(!Globals::recording)
    {
        Globals::recordingLength = 0;
    }

    [self toggleRecordLabel];
}
- (IBAction)PlayPressed:(UIButton *)sender {
    if(!Globals::playing)
    {
        Globals::playHead = 0;
    }
    [self togglePlayLabel];
}

- (void) toggleRecord:(NSNotification *) notification
{
    if ([[notification name] isEqualToString:@"ToggleRecord"])
    {
        [self toggleRecordLabel];
    }
}

- (void) toggleRecordLabel
{
    if(!Globals::recording)
    {
        Globals::recording = true;
        [self.RecordButton setTitle:@"Recording..." forState:UIControlStateNormal];
    }
    else
    {
        Globals::recording = false;
        [self.RecordButton setTitle:@"Record" forState:UIControlStateNormal];
    }
}

-(void) togglePlay:(NSNotification *) notification
{
    if ([[notification name] isEqualToString:@"TogglePlay"])
        [self togglePlayLabel];
}

-(void) togglePlayLabel
{
    if(!Globals::playing)
    {
        Globals::playing = true;
        [self.PlayButton setTitle:@"Playing..." forState:UIControlStateNormal];
    }
    else
    {
        Globals::playing = false;
        [self.PlayButton setTitle:@"Play" forState:UIControlStateNormal];
    }
}

@end
