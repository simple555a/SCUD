//
//  Linkable.h
//  test_Scud_objc
//
//  Created by bv on 25/07/2017.
//  Copyright © 2017 bv. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SCUDLinkable : NSObject
@property (readonly) char priority;
@property (readonly) float weight;
-(void)pull;
-(void)push;
-(void)pullAndPush;
-(void)linkPredecessor:(SCUDLinkable*) link;
-(void)linkSuccessor:(SCUDLinkable*) link;
-(void)setPriority:(char) priority;
-(void)setWeight:(float)weight;
@end