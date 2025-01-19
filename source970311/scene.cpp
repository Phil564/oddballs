/*==========================================================================
 *
 *  Copyright (C) 1995, 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: scene.c
 *
 ***************************************************************************/

#include "oddballs.h"
#include "scene.h"
#include "personal.h"
#include "anim.h"
#include "collision.h"
#include "platforms.h"
#include "timegates.h"
#include "actors.h"
#include "wind.h"
#include "bullets.h"

LANDSCAPE	landscape;
ACTOR		ball;
ACTOR		hand;
ACTOR		camera;


LPDIRECT3DRMLIGHT dummyLight = NULL;
LPDIRECT3DRMFRAME dummyFrame = NULL;

SCIT	scenicItems[NUM_SCENIC_ITEMS];
int		numScenicItems = 0;

TEXTURELIST textureList;


//LPDIRECT3DRMFRAME scenicItems[NUM_SCENIC_ITEMS];

/*	--------------------------------------------------------------------------------
	Function 	: BuildScene()
	Purpose 	: sets up landscape and other basic scene elements
	Parameters 	: device, view, scene, camera
	Returns 	: success or failure
	Info 		:
*/
BOOL BuildScene(LPDIRECT3DRMDEVICE dev, LPDIRECT3DRMVIEWPORT view,LPDIRECT3DRMFRAME scene, LPDIRECT3DRMFRAME camera)
{
	LPDIRECT3DRMMESHBUILDER egg_builder = NULL;
	char texturePath[256];
	LPDIRECT3DRMFRAME lights = NULL;
	LPDIRECT3DRMLIGHT ambientLight = NULL;
	LPDIRECT3DRMLIGHT light1Light = NULL;
	LPDIRECT3DRMLIGHT light2Light = NULL;
	LPDIRECT3DRMLIGHT light3Light = NULL;
	LPDIRECT3DRMFRAME light1Frame = NULL;
	LPDIRECT3DRMFRAME light2Frame = NULL;
	LPDIRECT3DRMFRAME light3Frame = NULL;
    view = view;                /* not used */

	sprintf(texturePath, "%sgraphics\\bmps", rootPath, rootPath);
	if(detailFlags & DF_TEXTURED)
		g_lpD3DRM->SetSearchPath(texturePath);

	InitTextureList();
	InitPlatformIdList();
	InitBulletStructures();

    /*
     * initialize the lights in the scene
     */
    if (FAILED(g_lpD3DRM->CreateFrame(scene,&lights)))
        goto generic_error;
    if (FAILED(lights->SetPosition(scene,D3DVAL(0),D3DVAL(100),D3DVAL(0))))
		goto generic_error;

    if (FAILED(g_lpD3DRM->CreateFrame(lights,&light1Frame)))
        goto generic_error;
    if (FAILED(light1Frame->SetPosition(lights,D3DVAL(0),D3DVAL(0),D3DVAL(100))))
		goto generic_error;
	light1Frame->AddRotation(D3DRMCOMBINE_BEFORE, 1, 0, 0, PI - (PI / 5));

    if (FAILED(g_lpD3DRM->CreateFrame(lights,&light2Frame)))
        goto generic_error;
    if (FAILED(light2Frame->SetPosition(lights,D3DVAL(-100),D3DVAL(0),D3DVAL(-100))))
		goto generic_error;
	light2Frame->AddRotation(D3DRMCOMBINE_BEFORE, 1,0, 0, (PI / 5));
	light2Frame->AddRotation(D3DRMCOMBINE_AFTER, 0, 1, 0, PI/6);

    if (FAILED(g_lpD3DRM->CreateFrame(lights,&light3Frame)))
        goto generic_error;
    if (FAILED(light3Frame->SetPosition(lights,D3DVAL(100),D3DVAL(0),D3DVAL(-100))))
		goto generic_error;
	light3Frame->AddRotation(D3DRMCOMBINE_BEFORE, 1,0, 0, (PI / 5));
	light3Frame->AddRotation(D3DRMCOMBINE_AFTER, 0, 1, 0, -(PI/6));

//shadow
    if (FAILED(g_lpD3DRM->CreateFrame(lights,&dummyFrame)))
        goto generic_error;
    if (FAILED(dummyFrame->SetPosition(lights,D3DVAL(0),D3DVAL(0),D3DVAL(0))))
		goto generic_error;
	dummyFrame->AddRotation(D3DRMCOMBINE_BEFORE, 1,0, 0, (PI/2));


/*
		if (FAILED(g_lpD3DRM->CreateMeshBuilder(&egg_builder)))
			goto generic_error;

		egg_builder->Load("\\pc\\oddballs\\graphics\\arrow.x", NULL, D3DRMLOAD_FROMFILE, NULL, NULL);
		if (FAILED(light1Frame->AddVisual((LPDIRECT3DRMVISUAL)egg_builder)))
			goto generic_error;
		if (FAILED(light2Frame->AddVisual((LPDIRECT3DRMVISUAL)egg_builder)))
			goto generic_error;
		if (FAILED(light3Frame->AddVisual((LPDIRECT3DRMVISUAL)egg_builder)))
			goto generic_error;

		if (FAILED(dummyFrame->AddVisual((LPDIRECT3DRMVISUAL)egg_builder)))
			goto generic_error;
*/
    if (FAILED(g_lpD3DRM->CreateLightRGB(D3DRMLIGHT_DIRECTIONAL, D3DVAL(0.0),D3DVAL(0.0),D3DVAL(0.0),&dummyLight)))
		goto generic_error;
    if (FAILED(dummyFrame->AddLight(dummyLight)))
		goto generic_error;


    if (FAILED(g_lpD3DRM->CreateLightRGB(D3DRMLIGHT_DIRECTIONAL, D3DVAL(0.15),D3DVAL(0.2),D3DVAL(0.2),&light1Light)))
		goto generic_error;

    if (FAILED(light1Frame->AddLight(light1Light)))
		goto generic_error;

    if (FAILED(g_lpD3DRM->CreateLightRGB(D3DRMLIGHT_DIRECTIONAL, D3DVAL(0.4),D3DVAL(0.4),D3DVAL(0.4),&light2Light)))
		goto generic_error;
    if (FAILED(light2Frame->AddLight(light2Light)))
		goto generic_error;
    if (FAILED(g_lpD3DRM->CreateLightRGB(D3DRMLIGHT_DIRECTIONAL, D3DVAL(0.3),D3DVAL(0.3),D3DVAL(0.25),&light3Light)))
		goto generic_error;
    if (FAILED(light3Frame->AddLight(light3Light)))
		goto generic_error;

    if (FAILED(g_lpD3DRM->CreateLightRGB(D3DRMLIGHT_AMBIENT,D3DVAL(0.3),D3DVAL(0.3),D3DVAL(0.3),&ambientLight)))
		goto generic_error;
    if (FAILED(scene->AddLight(ambientLight)))
        goto generic_error;

    /*
     * load landscape
     */

	InitTimeGates();
	InitActorList();
	InitWindList();
	InitScene(scene);
	ParseLandscapeFile(levelToLoad);
	InitPlaneInfo(levelToLoad);
		
    /*
     * setup fog
     */

/*
	g_lpWorld->SetSceneFogEnable(TRUE);
	g_lpWorld->SetSceneFogMode(D3DRMFOG_EXPONENTIAL);
	g_lpWorld->SetSceneFogParams(D3DVALUE(60), D3DVALUE(140), D3DVALUE(0.5));
	g_lpWorld->SetSceneFogColor(RGB_MAKE(32, 16, 36));
*/
//	g_lpWorld->SetSceneBackground(RGB_MAKE(98/3, 50/3, 109/3));

    /*
     * set up the frames position, orientation and rotation
     */

    if (FAILED(camera->SetPosition(scene,D3DVAL(0),D3DVAL(5),-D3DVAL(10))))
        goto generic_error;
    if (FAILED(camera->SetOrientation(scene,D3DVAL(0),D3DVAL(-0.4),D3DVAL(1),D3DVAL(0),D3DVAL(2),D3DVAL(0))))
		goto generic_error;
	




    RELEASE(lights);
    RELEASE(egg_builder);
    RELEASE(light1Light);
    RELEASE(light2Light);
    RELEASE(light3Light);
    RELEASE(light1Frame);
    RELEASE(light2Frame);
    RELEASE(light3Frame);
    RELEASE(ambientLight);
    return TRUE;

generic_error:
//    Msg("A failure occured while building the scene.\n");
ret_with_error:
    RELEASE(lights);
    RELEASE(egg_builder);
    RELEASE(light1Light);
    RELEASE(light2Light);
    RELEASE(light3Light);
    RELEASE(light1Frame);
    RELEASE(light2Frame);
    RELEASE(light3Frame);
    RELEASE(ambientLight);
    return TRUE;

    return FALSE;
}
/*
void
OverrideDefaults(Defaults *defaults)
{
    defaults->bNoTextures = FALSE;
    lstrcpy(defaults->Name, "Whack 'N Roll Demo");
}
*/



/*	--------------------------------------------------------------------------------
	Function 	: InitTextureList()
	Purpose 	: initialises the linked list of textures
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void InitTextureList()
{
	textureList.numEntries = 0;
	textureList.head.next = textureList.head.prev = &textureList.head;
}

/*	--------------------------------------------------------------------------------
	Function 	: AddTexture()
	Purpose 	: adds a texture to the linked list
	Parameters 	: pointer to platform
	Returns 	: none
	Info 		:
*/
void AddTexture(TEXTURE *texture)
{
	int	j;

	if(texture->listPointer == 0)
	{
		textureList.numEntries++;
		texture->listPointer = (TEXTURELISTENTRY *)MyMalloc(sizeof(TEXTURELISTENTRY));
		texture->listPointer->prev = &textureList.head;
		texture->listPointer->next = textureList.head.next;
		textureList.head.next->prev = texture->listPointer;
		textureList.head.next = texture->listPointer;
		texture->listPointer->texture = texture;
	}
}


//------------------------------------------------------------------
// 
// Function     : LoadTextures
//
// Purpose      : Loads an individual texture
//
// NOTE         : Textures must have a size divisible by 2 (e.g. 128x64, 256x256)
//
//------------------------------------------------------------------

HRESULT LoadTextures(char *sName, void *pArg, LPDIRECT3DRMTEXTURE *hTexture)
{    
    char *sTmpName = sName;
	LPDIRECT3DRMTEXTURE tex;
	TEXTURE *texture;	            
    TEXTURELISTENTRY *texturePtr;
	// Load the texture

	for(texturePtr = textureList.head.next;texturePtr != &textureList.head;texturePtr = texturePtr->next)
	{
		if(strcmp(texturePtr->texture->name, sName) == 0)
		{
//			dprintf("LoadTexture: Texture already exists\n");
			*hTexture = texturePtr->texture->texture;
			return 0;
		}
	}
	

    if (FAILED(g_lpD3DRM->LoadTexture(sName, hTexture)))
	{
		dprintf("\n\nWARNING!! Failed to load texture %s\n\n",sName);
		return -1;
	}
	tex = *hTexture;
	texture = (TEXTURE *)MyCalloc(1, sizeof(TEXTURE));
	AddTexture(texture);
	sprintf(texture->name, "%s", sName);
	texture->texture = tex;
	if(strcmp(sName,"cannball.ppm") != 0)
	{
		TRY_D3DRM(tex->SetDecalTransparency(TRUE));
		TRY_D3DRM(tex->SetDecalTransparentColor(RGB_MAKE(0,0,0)));
	}
    return 0;
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

    if (FAILED(g_lpD3DRM->CreateMeshBuilder(&mesh_builder)))
        goto generic_error;

     TRY_D3DRM(mesh_builder->Load(filename, NULL, D3DRMLOAD_FROMFILE, LoadTextures, NULL))
//   rval = mesh_builder->Load(filename, NULL, D3DRMLOAD_FROMFILE, NULL, NULL);
    if (rval != D3DRM_OK) 
	{
//        Msg("Failed to load file [%s]\n%s", fName, D3DRMErrorToString(rval));
		dprintf("Failed to load file %s\n",filename);
        goto ret_with_error;
    }

	dprintf("Loading landscape object from file %s\n",filename);

	mesh_builder->SetPerspective(TRUE);
	if(actor->flags & BACKGROUND)	
	{
		mesh_builder->SetQuality(D3DRMRENDER_UNLITFLAT);
	}
	// create an child frame from the parent
    if (FAILED(g_lpD3DRM->CreateFrame(parent, &(actor->frame))))
        goto generic_error;

     // add the loaded mesh into the frame
    if (FAILED(actor->frame->AddVisual((LPDIRECT3DRMVISUAL)mesh_builder)))
        goto generic_error;

	actor->faceCount = mesh_builder->GetFaceCount();
	mesh_builder->GetFaces(&(actor->faceArray));

    RELEASE(mesh_builder);

	return TRUE;
generic_error:
//    Msg("A failure occured while building the scene.\n");
ret_with_error:
    RELEASE(mesh_builder);
    return FALSE;

}