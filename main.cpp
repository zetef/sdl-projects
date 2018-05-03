#include <SDL.h>
#include <iostream>
#include <string>

//key press surfaces constants
enum gKeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT = 0,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

//screen properties
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

//window to render
SDL_Window* gWindow = NULL;

//window's surface
SDL_Surface* gScreenSurface = NULL;

//images that correspond to a keypress
SDL_Surface* gKeyPressSurfaces [ KEY_PRESS_SURFACE_TOTAL ];

//image about to be stretched
SDL_Surface* gStretchedSurface = NULL;

//current displayed image
SDL_Surface* gCurrentSurface = NULL;

//starts SDL and creates window
bool init();

//load media
bool loadMedia();

//frees media and shuts SDL
void close();

//load individual images
SDL_Surface* loadSurface( std::string path );

//create a stretched image
void createStretchedImage( int x, int y, int w, int h, SDL_Surface* src, SDL_Surface* dst );

SDL_Surface* loadSurface( std::string path )
{
    //the final optimized image
    SDL_Surface* optimizedSurface = NULL;

    //load image at specified path
    SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
    if( loadedSurface == NULL )
    {
        std::cout << "could not load image at " << path << "\nerror: " << SDL_GetError( ) << std::endl;
    }
    else
    {
        //convert image to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, NULL );
        if( optimizedSurface == NULL )
        {
            std::cout << "could not optimize image " << path.c_str() << "\nerror: " << SDL_GetError( ) << std::endl;
        }

        //get rid of loaded surface
        SDL_FreeSurface( loadedSurface );

    }

    return optimizedSurface;
}

void createStretchedImage( int x, int y, int w, int h, SDL_Surface* src, SDL_Surface* dst )
{
    SDL_Rect stretchRect;
    stretchRect.x = x;
    stretchRect.y = y;
    stretchRect.w = w;
    stretchRect.h = h;
    SDL_BlitScaled( src, NULL, dst, &stretchRect );
}

bool init()
{

    //initialization flag
    bool succes = true;

    //initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0)
    {
        std::cout << "could not initialize. error: " << SDL_GetError() << std::endl;
        succes = false;
    }
    else
    {
        //create window
        gWindow = SDL_CreateWindow( "title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if(gWindow == NULL)
        {
            std::cout << "could not create window. error: " << SDL_GetError() << std::endl;
            succes = false;
        }
        else
        {
            //get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }

    return succes;
}

bool loadMedia()
{
    //loading succes flag
    bool succes = true;

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface( "key_presses/default.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL )
    {
        std::cout << "failed to load default image!" << std::endl;
        succes = false;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = loadSurface( "key_presses/up.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL )
    {
        std::cout << "failed to load up image!" << std::endl;
        succes = false;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = loadSurface( "key_presses/down.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL )
    {
        std::cout << "failed to load down image!" << std::endl;
        succes = false;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = loadSurface( "key_presses/left.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL )
    {
        std::cout << "failed to load left image!" << std::endl;
        succes = false;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = loadSurface( "key_presses/right.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL )
    {
        std::cout << "failed to load right image!" << std::endl;
        succes = false;
    }

    gStretchedSurface = loadSurface( "stretch_image/stretchable_image.bmp" );

    return succes;
}

void close()
{
    //deallocate surfaces
    SDL_FreeSurface( gCurrentSurface );
    gCurrentSurface = NULL;

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //quit SDL subsytem
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    //start up SDL and create window
    if( !init() )
    {
        std::cout << "failed to initialize!\n";
    }
    else
    {
        //laod media
        if( !loadMedia() )
        {
            std::cout << "failed to load media\n";
        }
        else
        {
            //main loop flag
            bool quit = false;

            //event handler
            SDL_Event e;

            //set default current surface
            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];

            int w = SCREEN_WIDTH, h = SCREEN_HEIGHT;

            while( !quit )
            {
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //user requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }

                    //user presses a key
                    else if( e.type == SDL_KEYDOWN )
                    {
                        switch( e.key.keysym.sym )
                        {
                        case SDLK_UP:
                            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
                            h -= 10;
                            break;

                        case SDLK_DOWN:
                            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
                            h += 10;
                            break;

                        case SDLK_LEFT:
                            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
                            w -= 10;
                            break;

                        case SDLK_RIGHT:
                            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
                            w += 10;
                            break;

                        case SDLK_ESCAPE:
                            quit = true;
                            break;

                        default:
                            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
                            break;
                        }
                    }
                }

                //apply the image stretched
                createStretchedImage(0, 0, /*x, y*/
                                     w, h, /*w, h*/
                                     gStretchedSurface, gScreenSurface); /*source, destination*/

                //apply the current image
                SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, NULL );

                //update
                SDL_UpdateWindowSurface( gWindow );
            }

            //delay 2 seconds
            //SDL_Delay( 2000 );
        }
    }

    //free sources and stop SDL
    close();

    return 0;
}

   
