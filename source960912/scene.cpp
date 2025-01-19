/*==========================================================================
 *
 *  Copyright (C) 1995, 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: scene.c
 *
 ***************************************************************************/

#include "oddballs.h"

ACTOR	landscape;
ACTOR	ball;
ACTOR	hand;
ACTOR	camera;


BOOL
BuildScene(LPDIRECT3DRMDEVICE dev, LPDIRECT3DRMVIEWPORT view,
           LPDIRECT3DRMFRAME scene, LPDIRECT3DRMFRAME camera)
{
    LPDIRECT3DRMFRAME lights = NULL;
    LPDIRECT3DRMMESHBUILDER egg_builder = NULL;
    LPDIRECT3DRMLIGHT light1 = NULL;
    LPDIRECT3DRMLIGHT light2 = NULL;
    LPDIRECT3DRMLIGHT light3 = NULL;
    HRESULT rval;

    view = view;                /* not used */


    /*
     * initialize the lights in the scene
     */
    if (FAILED(lpD3DRM->CreateFrame(scene,&lights)))
        goto generic_error;
    if (FAILED(lights->SetPosition(scene,D3DVAL(0),D3DVAL(100),D3DVAL(0))))
		goto generic_error;

    if (FAILED(lpD3DRM->CreateLightRGB(D3DRMLIGHT_POINT,D3DVAL(0.9),D3DVAL(0.8), D3DVAL(0.7),&light1)))
		goto generic_error;
    if (FAILED(lights->AddLight(light1)))
		goto generic_error;

    if (FAILED(lpD3DRM->CreateLightRGB(D3DRMLIGHT_DIRECTIONAL, D3DVAL(0.5),D3DVAL(0.6),D3DVAL(0.8),&light3)))
		goto generic_error;
    if (FAILED(lights->AddLight(light3)))
		goto generic_error;



    if (FAILED(lpD3DRM->CreateLightRGB(D3DRMLIGHT_AMBIENT,D3DVAL(0.2),D3DVAL(0.2),D3DVAL(0.2),&light2)))
		goto generic_error;
    if (FAILED(scene->AddLight(light2)))
        goto generic_error;

    /*
     * load landscape
     */

	LoadMeshIntoActor(&landscape,scene,"\\pc\\oddballs\\graphics\\dock\\dock.x");
	LoadMeshIntoActor(&hand,scene,"\\pc\\oddballs\\graphics\\hand\\hand.x");
	LoadMeshIntoActor(&ball,scene,"\\pc\\oddballs\\graphics\\ball\\ball.x");
	
	ball.frame->SetPosition(scene, D3DVAL(0), D3DVAL(15), D3DVAL(13));


    /*
     * set up the frames position, orientation and rotation
     */

    if (FAILED(camera->SetPosition(scene,D3DVAL(0),D3DVAL(5),-D3DVAL(10))))
        goto generic_error;
    if (FAILED(camera->SetOrientation(scene,D3DVAL(0),D3DVAL(-0.4),D3DVAL(1),D3DVAL(0),D3DVAL(2),D3DVAL(0))))
		goto generic_error;
	
    RELEASE(lights);
    RELEASE(egg_builder);
    RELEASE(light1);
    RELEASE(light2);
    return TRUE;

generic_error:
    Msg("A failure occured while building the scene.\n");
ret_with_error:
    RELEASE(lights);
    RELEASE(egg_builder);
    RELEASE(light1);
    RELEASE(light2);
    return FALSE;
}

void
OverrideDefaults(Defaults *defaults)
{
    defaults->bNoTextures = FALSE;
    lstrcpy(defaults->Name, "OddBalls Direct3DRM Shell");
}



/*	--------------------------------------------------------------------------------
	Function 	: LoadMeshIntoActor
	Purpose 	: loads a .x file and adds it to the specified frame.
	Parameters 	: filename
	Returns 	: pointer to frame
	Info 		:
*/

BOOL LoadMeshIntoActor(ACTOR *actor,LPDIRECT3DRMFRAME parent,char *filename)
{

    LPDIRECT3DRMMESHBUILDER mesh_builder = NULL;
	int rval = 0;

	actor->parent = parent;
//	actor->frame = (LPDIRECT3DRMFRAME *)malloc(sizeof(LPDIRECT3DRMFRAME));
//	actor->frame = frame;

    if (FAILED(lpD3DRM->CreateMeshBuilder(&mesh_builder)))
        goto generic_error;

    rval = mesh_builder->Load(filename, NULL, D3DRMLOAD_FROMFILE, NULL, NULL);
    if (rval != D3DRM_OK) 
	{
        Msg("Failed to load file [%s]\n%s", filename, D3DRMErrorToString(rval));
        goto ret_with_error;
    }

	// create an child frame from the parent
    if (FAILED(lpD3DRM->CreateFrame(parent, &(actor->frame))))
        goto generic_error;

     // add the loaded mesh into the frame
    if (FAILED(actor->frame->AddVisual((LPDIRECT3DRMVISUAL)mesh_builder)))
        goto generic_error;

	actor->faceCount = mesh_builder->GetFaceCount();
	mesh_builder->GetFaces(&(actor->faceArray));

    RELEASE(mesh_builder);

	return TRUE;
generic_error:
    Msg("A failure occured while building the scene.\n");
ret_with_error:
    RELEASE(mesh_builder);
    return FALSE;

}