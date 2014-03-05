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

@end

@implementation KaraokeViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
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
        [self.RecordButton setTitle:@"Recording..." forState:UIControlStateNormal];
        Globals::recording = true;
        Globals::recordingLength = 0;
    }
    else
    {
        [self.RecordButton setTitle:@"Record" forState:UIControlStateNormal];
        Globals::recording = false;
    }
}
- (IBAction)PlayPressed:(UIButton *)sender {
    if(!Globals::playing)
    {
        [self.PlayButton setTitle:@"Playing..." forState:UIControlStateNormal];
        Globals::playing = true;
        Globals::playHead = 0;
    }
    else
    {
        [self.PlayButton setTitle:@"Play" forState:UIControlStateNormal];
        Globals::playing = false;
    }
}

@end
