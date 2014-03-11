//
//  KaraokeViewController.m
//  Karaoke-Example
//
//  Created by thealphanerd on 3/4/14.
//  Copyright (c) 2014 thealphanerd. All rights reserved.
//

#import "KaraokeViewController.h"
#import "Globals.h"
#import "FileUploader.h"
#import "Renderer.h"

@interface KaraokeViewController ()

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

@property (weak, nonatomic) IBOutlet UIButton *RecordButton;
@property (weak, nonatomic) IBOutlet UIButton *PlayButton;
@property (weak, nonatomic) IBOutlet UIButton *UploadButton;

@property (weak, nonatomic) FileUploader *uploader;

- (void)setupGL;
- (void)tearDownGL;

- (void) togglePlayLabel;
@end

@implementation KaraokeViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];

    KaraokeInit();
    _uploader = [FileUploader sharedUploader];
    
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

- (void)viewDidLayoutSubviews
{
    KaraokeSetDims( self.view.bounds.size.width, self.view.bounds.size.height );
//    Globals::waveform->setWidth(self.view.bounds.size.width);
//    Globals::waveform->setHeight(self.view.bounds.size.height / 5);
}

- (void)dealloc
{
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    
    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }
    
    // Dispose of any resources that can be recreated.
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
    // glEnable( GL_DEPTH_TEST );
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    self.effect = nil;
}

- (IBAction)settingsDismissed:(UIStoryboardSegue *)segue
{
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    //    glClearColor( 1.0f, 1.0f, 0.0f, 1.0f);
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    KaraokeRender();
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

- (IBAction)UploadPressed:(UIButton *)sender {
    [_uploader uploadData:nullptr];
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
