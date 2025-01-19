/*
 *	Project:	oddballs
 *	File:		anim.cpp
 *	Description:choney
 */

#include "oddballs.h"
#include "anim.h"
#include "maths.h"
#include "personal.h"
#include "graphics.h"
#include "actors.h"

void ACB(LPDIRECT3DRMFRAME obj, void* arg, D3DVALUE delta)
{
	static double time = 0;

	hand.anims[hand.currentAnimation].animationSet->SetTime(time);
	time += 0.5;
}


void AnimationCallback(LPDIRECT3DRMFRAME obj,void *myActor, D3DVALUE delta)
{
	ACTOR *temp = (ACTOR*)myActor;
    obj = obj;
	D3DVALUE time;

	temp->reachedEndOfAnimation = FALSE;

	temp->callBack->animset->SetTime(temp->callBack->time);

    temp->callBack->time += (temp->speedAnimation);
//    temp->callBack->time += delta*(temp->speedAnimation / 50);

	if(temp->callBack->time == temp->anims[temp->currentAnimation].numberOfFrames)
	{
		temp->reachedEndOfAnimation = TRUE;
		temp->callBack->time -= 0.1;
	}

	if ((temp->callBack->time > temp->anims[temp->currentAnimation].numberOfFrames)&&
		(temp->loopAnimation==NO))
	{
		
		temp->callBack->time = temp->anims[temp->currentAnimation].numberOfFrames - 0.1;
		temp->reachedEndOfAnimation = TRUE;
		if (temp->queueAnimation[0] != -1)
		{
			StopAnimationCallBack(temp);
	   
			SetAnimCallBack(temp, temp->anims[temp->queueAnimation[0]].animationSet);
			temp->currentAnimation = temp->queueAnimation[0];//anim;
			temp->loopAnimation = temp->queueLoopAnimation[0];//loop;
			temp->speedAnimation = temp->queueSpeedAnimation[0];//speed;
	   
			temp->queueAnimation[0] = temp->queueAnimation[1];
			temp->queueAnimation[1] = temp->queueAnimation[2];
			temp->queueAnimation[2] = temp->queueAnimation[3];
			temp->queueAnimation[3] = temp->queueAnimation[4];
			temp->queueAnimation[4] = -1;
			temp->queueLoopAnimation[0] = temp->queueLoopAnimation[1];
			temp->queueLoopAnimation[1] = temp->queueLoopAnimation[2];
			temp->queueLoopAnimation[2] = temp->queueLoopAnimation[3];
			temp->queueLoopAnimation[3] = temp->queueLoopAnimation[4];
			temp->queueLoopAnimation[4] = -1;
			temp->queueSpeedAnimation[0] = temp->queueSpeedAnimation[1];
			temp->queueSpeedAnimation[1] = temp->queueSpeedAnimation[2];
			temp->queueSpeedAnimation[2] = temp->queueSpeedAnimation[3];
			temp->queueSpeedAnimation[3] = temp->queueSpeedAnimation[4];
			temp->queueSpeedAnimation[4] = -1;
			if (temp->numberQueued >0) temp->numberQueued--;
		}
	}
	else
	{
		if (temp->callBack->time > temp->anims[temp->currentAnimation].numberOfFrames)
		{
			temp->callBack->time -= temp->anims[temp->currentAnimation].numberOfFrames;
		}
	}

}

/*	--------------------------------------------------------------------------------
	Function 	: SetAnimCallBack(ACTOR *myActor, LPDIRECT3DRMANIMATIONSET animset)
	Purpose 	: Sets the animation callback routine for an actor
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
BOOL SetAnimCallBack(ACTOR *myActor, LPDIRECT3DRMANIMATIONSET animset)
{
	myActor->callBack->animset = animset;
	myActor->callBack->time = D3DVAL(0);
//	myActor->reachedEndOfAnimation = FALSE;
	myActor->callBack->animset->SetTime(myActor->callBack->time);
    if (FAILED(myActor->frame->AddMoveCallback(AnimationCallback, myActor)))
        return FALSE;
    return TRUE;
}


/*	--------------------------------------------------------------------------------
	Function 	: StopAnimationCallBack()
	Purpose 	: Removes the animation callback routine for a actor
	Parameters 	: actor pointer
	Returns 	: none
	Info 		:
*/
void StopAnimationCallBack(ACTOR *myActor)
{
	if (myActor->currentAnimation!=-1)
	{
		myActor->frame->DeleteMoveCallback(AnimationCallback,myActor);
		myActor->currentAnimation = -1;
	}
	myActor->callBack->time = D3DVAL(0);
//	myActor->reachedEndOfAnimation = FALSE;
}

/*	--------------------------------------------------------------------------------
	Function 	: InitActor()
	Purpose 	: Initialises an actor structure
	Parameters 	: actor pointer
	Returns 	: none
	Info 		:
*/
void InitActor(ACTOR *myActor)
{
	myActor->frame = NULL;
	myActor->currentAnimation = -1;
	myActor->queueAnimation[0] = -1;
	myActor->queueAnimation[1] = -1;
	myActor->queueAnimation[2] = -1;
	myActor->queueAnimation[3] = -1;
	myActor->queueAnimation[4] = -1;
	myActor->loopAnimation = NO;
	myActor->numberQueued = 0;
//	myActor->processScript = NO;
	myActor->reachedEndOfAnimation = FALSE;
	myActor->callBack = (animationCallbackArgs*)MyMalloc(sizeof(animationCallbackArgs));
	myActor->status = ACTIVE;

	myActor->enemyNum = -1;
	myActor->scale.v[X] = myActor->scale.v[Y] = myActor->scale.v[Z] = 1;
}

/*	--------------------------------------------------------------------------------
	Function 	: LoadFrame()
	Purpose 	: Loads an x file into a frame
	Parameters 	: pointer to frame, and relative parent, and filename
	Returns 	: success or failure
	Info 		:
*/
LPDIRECT3DRMFRAME LoadFrame(LPDIRECT3DRMFRAME scene,char *filename)
{
	char fName[256];
	LPDIRECT3DRMFRAME temp = NULL;

	sprintf(fName, "%s%s", rootPath, filename);
	if (FAILED(g_lpD3DRM->CreateFrame(scene,&temp)))
	{
		dprintf("Error creating frame for %s\n",filename);
		return NULL;
	}

	if (FAILED(temp->Load(fName, NULL,D3DRMLOAD_FROMFILE, NULL, NULL)))
	{
		dprintf("Error loading %s\n",filename);
		return NULL;
	}
	dprintf("LoadFrame:Loaded %s\n", filename);
	return temp;
}

/*	--------------------------------------------------------------------------------
	Function 	: LoadMesh()
	Purpose 	: Loads an x file into a frame
	Parameters 	: pointer to frame, and relative parent, and filename
	Returns 	: success or failure
	Info 		:
*/
LPDIRECT3DRMFRAME LoadMesh(LPDIRECT3DRMFRAME scene,char *filename)
{
	char fName[256];
	LPDIRECT3DRMFRAME temp = NULL;
    LPDIRECT3DRMMESHBUILDER mesh_builder = NULL;
	int rval = 0;

	sprintf(fName, "%s%s", rootPath, filename);

    if (FAILED(g_lpD3DRM->CreateMeshBuilder(&mesh_builder)))
        goto generic_error;

    rval = mesh_builder->Load(fName, NULL, D3DRMLOAD_FROMFILE, LoadTextures, NULL);
    if (rval != D3DRM_OK) 
	{
		dprintf("Failed to load file %s\n",fName);
        goto ret_with_error;
    }
	mesh_builder->SetPerspective(TRUE);


	// create an child frame from the parent
    if (FAILED(g_lpD3DRM->CreateFrame(scene, &(temp))))
        goto generic_error;

     // add the loaded mesh into the frame
    if (FAILED(temp->AddVisual((LPDIRECT3DRMVISUAL)mesh_builder)))
        goto generic_error;


    RELEASE(mesh_builder);
	return temp;

generic_error:
//    Msg("A failure occured while building the scene.\n");
ret_with_error:
    RELEASE(mesh_builder);
    return NULL;

}



/*	--------------------------------------------------------------------------------
	Function 	: LoadActor()
	Purpose 	: Load an actor into an ACTOR structure
	Parameters 	: pointer to structure,parent,filename
	Returns 	: success or failure
	Info 		:
*/
BOOL LoadActor(ACTOR *myActor,LPDIRECT3DRMFRAME scene,char *filename)
{
	char fName[256];
    LPDIRECT3DRMMESHBUILDER mesh_builder = NULL;
	int rval = 0;


	sprintf(fName, "%s%s", rootPath, filename);
/*	if (FAILED(g_lpD3DRM->CreateFrame(scene,&myActor->frame)))
	{
		dprintf("Error creating frame for %s\n",filename);
		return FALSE;
	}

	if (FAILED(myActor->frame->Load(fName, NULL,
						D3DRMLOAD_FROMFILE, NULL, NULL)))
	{
		dprintf("Error loading %s\n",filename);
		return FALSE;
	}
	dprintf("LoadActor:Loaded %s\n", filename);
	if(myActor->flags & SHADOW)
		InitShadow(myActor);

	return TRUE;
*/

    if (FAILED(g_lpD3DRM->CreateMeshBuilder(&mesh_builder)))
        return FALSE;
	mesh_builder->SetPerspective(TRUE);
    if (FAILED(g_lpD3DRM->CreateFrame(scene, &(myActor->frame))))
	{
		dprintf("cant create frame");
		return FALSE;
	}
    if (FAILED(myActor->frame->AddVisual((LPDIRECT3DRMVISUAL)mesh_builder)))
		return FALSE;

	if(myActor->flags & SHADOW)
		InitShadow(myActor);

    RELEASE(mesh_builder);
	return TRUE;

}

/*	--------------------------------------------------------------------------------
	Function 	: LoadAnimation()
	Purpose 	: Load an animation into an animation structure
	Parameters 	: animation pointer, number of frames, filename
	Returns 	: success or failure
	Info 		:
*/
BOOL LoadAnimation(animation *anim,int noFrames, double speed, char *filename, ACTOR *actor)
{
	char fName[256];
	static int objNum = 0;
	UUID *animUuid[50];
	char tname[60];


//	if(UuidCreate(animUuid[objNum]) != RPC_S_OK)
//		return FALSE;


	sprintf(fName, "%s%s", rootPath, filename);

	if (FAILED(g_lpD3DRM->CreateAnimationSet(&anim->animationSet)))
		return FALSE;
		
	if (dxerror =(anim->animationSet->Load(fName, NULL, D3DRMLOAD_FROMFILE, LoadTextures, NULL, actor->frame)))
	{
		TraceErrorD3DRM(dxerror, NULL, 0);
		return FALSE;

	}
	anim->numberOfFrames = noFrames;
	anim->speed = speed;
	objNum++;
	return TRUE;
}

/*	--------------------------------------------------------------------------------
	Function 	: AttachAnimations(ACTOR *myActor,animation *anim)
	Purpose 	: Attaches a set of animations to a actor.
	Parameters 	: actor pointer, animation pointer
	Returns 	: none
	Info 		:
*/
void AttachAnimations(ACTOR *myActor,animation *anim)
{
	myActor->anims = anim;
}

/*	--------------------------------------------------------------------------------
	Function 	: SetAnimationSpeed	
	Purpose 	: Sets the speed of an actor
	Parameters 	: actor, speed
	Returns 	: none
	Info 		:
*/
void SetAnimationSpeed(ACTOR *myActor, double speed)
{
	myActor->speedAnimation = speed;

}

/*	--------------------------------------------------------------------------------
	Function 	: AnimateActor()
	Purpose 	: Sets the animation for a actor.
	Parameters 	: actor pointer, animation number, loop:animation should loop, queue:queue the animation
	Returns 	: none
	Info 		:
*/
void AnimateActor(ACTOR *myActor,int anim,BOOL loop,BOOL queue,double speed)
{
	if(myActor->anims == NULL)
		return;

	if(myActor->anims[anim].animationSet == NULL)
		return;

	if(speed == 0)
	{
		speed = myActor->anims[anim].speed;
	}
 
	if (queue==NO)
	{
		StopAnimationCallBack(myActor);
//		if (myActor->currentAnimation!=-1) StopAnimationCallBack(myActor);
		SetAnimCallBack(myActor, myActor->anims[anim].animationSet);
		myActor->currentAnimation = anim;
		myActor->loopAnimation = loop;
		myActor->speedAnimation = speed;
		myActor->reachedEndOfAnimation = FALSE;
		FlushQueue(myActor);

	}
	else
	{
		if (myActor->currentAnimation == -1)
		{
			SetAnimCallBack(myActor, myActor->anims[anim].animationSet);
			myActor->currentAnimation = anim;
			myActor->loopAnimation = loop;
			myActor->speedAnimation = speed;
		}
		else
		{
			if (myActor->numberQueued == 5) return;
			myActor->queueAnimation[myActor->numberQueued] = anim;
			myActor->queueLoopAnimation[myActor->numberQueued] = loop;
			myActor->queueSpeedAnimation[myActor->numberQueued] = speed;
			myActor->numberQueued++;
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: FlushQueue()
	Purpose 	: Flush the animation queue of the given actor
	Parameters 	: actor pointer
	Returns 	: none
	Info 		:
*/
void FlushQueue(ACTOR *myActor)
{
	myActor->numberQueued = 0;
	myActor->queueAnimation[0] = -1;
	myActor->queueAnimation[1] = -1;
	myActor->queueAnimation[2] = -1;
	myActor->queueAnimation[3] = -1;
	myActor->queueAnimation[4] = -1;
}

/*	--------------------------------------------------------------------------------
	Function 	: StopAnimateActor()
	Purpose 	: Stops the animation for an actor
	Parameters 	: actor pointer
	Returns 	: none
	Info 		:
*/
void StopAnimateActor(ACTOR *myActor,int mode)
{
	if (mode==RESET)
	{
		if (myActor->currentAnimation!=-1)
			myActor->anims[myActor->currentAnimation].animationSet->SetTime(D3DVAL(0.0));
	}
	if (myActor->currentAnimation!=-1) StopAnimationCallBack(myActor);
}

/*	--------------------------------------------------------------------------------
	Function 	: EndAnimationLoop()
	Purpose 	: Stops the animation looping
	Parameters 	: actor pointer
	Returns 	: none
	Info 		:
*/
void EndAnimationLoop(ACTOR *myActor)
{
	myActor->loopAnimation = NO;
}

