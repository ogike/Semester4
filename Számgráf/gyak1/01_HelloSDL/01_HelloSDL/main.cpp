#include <SDL.h>
#include <iostream>

int main( int argc, char* args[] )
{
	// állítsuk be, hogy kilépés előtt hívja meg a rendszer az alábbi függvényt, ami a lambda törzsében található
	atexit([] {
			std::cout << "Press a key to exit the application..." << std::endl;
			std::cin.get();
		});

	//
	// 1. lépés: inicializáljuk az SDL-t
	//

	// a grafikus alrendszert kapcsoljuk csak be, ha gond van, akkor jelezzük és lépjünk ki
	if ( SDL_Init( SDL_INIT_VIDEO ) == -1 )
	{
		// írjuk ki a hibát és termináljon a program
		std::cout << "[SDL initialization] Error during the SDL initialization: " << SDL_GetError() << std::endl;
		return 1;
	}

	/* 
	// Az SDL verziójáról az alábbi módon nyerhető ki információ:
	{
		SDL_version compiled;
		SDL_version linked;

		SDL_VERSION(&compiled);		// Makró a belefordított verzió lekéréséhez
		SDL_GetVersion(&linked);	// Függvény a linkelt verzió lekéréséhez
		std::cout << "[SDL verzió] Az SDL verziója: " << (int)compiled.major << "." << static_cast<int>(compiled.minor) << "." << compiled.patch << std::endl;
		std::cout << "[SDL verzió] Az SDL version: " << (int)linked.major << "." << static_cast<int>(linked.minor) << "." << linked.patch << std::endl;
	}
	*/	

	//
	// 2. lépés: hozzuk létre az ablakot, amire rajzolni fogunk
	//

	SDL_Window *win = nullptr;
	win = SDL_CreateWindow( "Hello SDL!",				// az ablak fejléce
							100,						// az ablak bal-felső sarkának kezdeti X koordinátája
							100,						// az ablak bal-felső sarkának kezdeti Y koordinátája
							800,						// ablak szélessége
							600,						// és magassága
							SDL_WINDOW_SHOWN);			// megjelenítési tulajdonságok

	// ha nem sikerült létrehozni az ablakot, akkor írjuk ki a hibát, amit kaptunk és lépjünk ki
	if (win == nullptr)
	{
		std::cout << "[Window creation] Error during the creation of an SDL window: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	//
	// 3. lépés: hozzunk létre egy renderelőt, rajzolót
	//

	SDL_Renderer *ren = nullptr;
	ren = SDL_CreateRenderer(	win, // melyik ablakhoz rendeljük hozzá a renderert
								-1,  // melyik indexű renderert inicializáljuk
									 // a -1 a harmadik paraméterben meghatározott igényeinknek megfelelő első renderelőt jelenti
								SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	// az igényeink, azaz
																						// hardveresen gyorsított és vsync-et beváró
	if (ren == nullptr)
	{
		std::cout << "[Renderer creation] Error during the creation of an SDL renderer: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	//
	// 3. lépés: töröljük az ablak háttérszínét, rajzoljunk egy vonalat és várjunk 2 másodpercet
	//

	// aktuális rajzolási szín legyen feket - 
	SDL_SetRenderDrawColor(	ren,	// melyik renderelőnek állítjuk be az aktuális rajzolási színét
							0,		// vörös intenzitás - 8 bites előjel nélküli egész szám
							0,		// zöld intenzitás - 8 bites előjel nélküli egész szám
							0,		// kék intenzitás - 8 bites előjel nélküli egész szám
							255);	// átlátszóság - 8 bites előjel nélküli egész szám

	// - és töröljük az aktuális rajzolási színnel az ablak kliensterületét
	SDL_RenderClear(ren);

	// aktuális rajzolási szín legyen zöld - 
	SDL_SetRenderDrawColor(	ren,	// renderer címe, aminek a rajzolási színét be akarjuk állítani
							0,		// piros
							255,	// zöld
							0,		// kék
							255);	// átlátszatlanság

	// - és rajzoljunk ki egy vonalat
	SDL_RenderDrawLine(	ren,	// renderer címe, amivel vonalat akarunk rajzolni
						10, 10, // vonal kezdőpontjának (x,y) koordinátái
						10, 60);// vonal végpontjának (x,y) koordinátái
	
	// ####################################
	//			FELADATOK
	// ####################################

	// 1. feladat: egészítsük ki a fenti vonalat egy H betűvé!

	SDL_RenderDrawLine(ren, 40, 10, 40, 60);
	SDL_RenderDrawLine(ren, 10, 35, 40, 35);

	// 2. beadható feladat: írjuk ki a "HELLO" szöveget a képernyőre!
    
	//E
	SDL_RenderDrawLine(ren, 50, 10, 50, 60);  //vert
	SDL_RenderDrawLine(ren, 50, 10, 80, 10); //horiz top
	SDL_RenderDrawLine(ren, 50, 35, 80, 35); //horiz mid
	SDL_RenderDrawLine(ren, 50, 60, 80, 60); //horiz bot

	//L
	SDL_RenderDrawLine(ren, 90, 10, 90, 60);  //vert
	SDL_RenderDrawLine(ren, 90, 60, 120, 60); //horiz top

	//L
	SDL_RenderDrawLine(ren, 130, 10, 130, 60);  //vert
	SDL_RenderDrawLine(ren, 130, 60, 150, 60); //horiz top





	//O az legyen ellipszis (magasabb mint szélesebb) (parametric representation függvény)
	// main.cpp-t kell elküldeni
	//sorrendben összeköti a pontokat:
		// SDL_RenderDrawLines( <renderer ptr>, <SDL_Point tömb>, <pontok száma>); parancs!

	//kör rajzolása
		//n egyenes szakaszra bontással
	const int numOfPoints = 35;
	SDL_Point points[numOfPoints+1];
	float r = 20;
	float scaleX = 1.0f, scaleY = 1.25f;
	int cx = 180, cy = 35;
	float alpha = 2 * M_PI / numOfPoints; //egy szakaszhoz tartozó körcikk foka radiánban

	for (int i = 0; i <= numOfPoints; i++)
	{
		//cpp inicializálós lista thingy:
			//points[i] = {1,2} ekvivalens = SDL_Point(1,2)

		//Fontos: cosf/sinf függvények radiánnal dolgoznak
		points[i] = { cx + (int)(scaleX * r * cosf(alpha * i)), 
					  cy + (int)(scaleY * r * sinf(alpha * i)) };
	}

	SDL_RenderDrawLines(ren, points, numOfPoints+1);

	// 3. feladat: 2 másodpercenként változzon a háttér színe! Először legyen piros, aztán zöld és végül kék,
	// majd lépjen ki a program!

	// jelenítsük meg a backbuffer tartalmát
	SDL_RenderPresent(ren);

	// várjunk 2 másodpercet
	SDL_Delay(3000);

	//piros háttér
	SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);	
	SDL_RenderClear(ren);
	SDL_RenderPresent(ren);

	SDL_Delay(2000);

	//zöld háttér
	SDL_SetRenderDrawColor(ren,	0, 255, 0, 255);	
	SDL_RenderClear(ren);
	SDL_RenderPresent(ren);

	SDL_Delay(2000);

	//kék háttér
	SDL_SetRenderDrawColor(ren,	0, 0, 255, 255);	
	SDL_RenderClear(ren);
	SDL_RenderPresent(ren);

	SDL_Delay(2000);


	//
	// 4. lépés: lépjünk ki
	// 

	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}
