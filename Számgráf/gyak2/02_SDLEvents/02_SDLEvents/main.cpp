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

	// a grafikus alrendszert kapcsoljuk csak be, ha gond van, akkor jelezzük és lépjün ki
	//	NOTE: ez automatikusan beindítja ez események alrendszerét
	if ( SDL_Init( SDL_INIT_VIDEO ) == -1 )
	{
		// irjuk ki a hibat es terminaljon a program
		std::cout << "[SDL initialization] Error during the SDL initialization: " << SDL_GetError() << std::endl;
		return 1;
	}
			
	//
	// 2. lépés: hozzuk létre az ablakot, amire rajzolni fogunk
	//

	SDL_Window *win = nullptr;
    win = SDL_CreateWindow( "Hello SDL!",				// az ablak fejléce
							100,						// az ablak bal-felsõ sarkának kezdeti X koordinátája
							100,						// az ablak bal-felsõ sarkának kezdeti Y koordinátája
							800,						// ablak szélessége
							600,						// és magassága
							SDL_WINDOW_SHOWN);			// megjelenítési tulajdonságok

	// ha nem sikerült létrehozni az ablakot, akkor írjuk ki a hibát, amit kaptunk és lépjünk ki
    if (win == nullptr)
	{
		std::cout << "[Window creation] Error during the creation of an SDL window: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 3. lépés: hozzunk létre egy renderelõt, rajzolót
	//

    SDL_Renderer *ren = nullptr;
    ren = SDL_CreateRenderer(	win, // melyik ablakhoz rendeljük hozzá a renderert
								-1,  // melyik indexú renderert inicializáljuka
									 // a -1 a harmadik paraméterben meghatározott igényeinknek megfelelõ elsõ renderelõt jelenti
								SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	// az igényeink, azaz
																						// hardveresen gyorsított és vsync-et beváró
    if (ren == nullptr)
	{
        std::cout << "[Renderer creation] Error during the creation of an SDL renderer: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 4. lépés: indítsuk el a fõ üzenetfeldolgozó ciklust
	// 

	// véget kell-e érjen a program futása?
	bool quit = false;
	// feldolgozandó üzenet ide kerül
	SDL_Event ev;
	// egér X és Y koordinátái
	Sint32 mouseX = 0, mouseY = 0;

	int rectColR = 255, rectColG = 0, rectColB = 0;

	while (!quit)
	{
		//Note: SDL-ben események konkurrensen dolgozódnak fel
			//sorrendben bekerülnek egy listában, abban tudunk végighaladni rajtuk gyakorlatilag

		// amíg van feldolgozandó üzenet dolgozzuk fel mindet: (legutóbbi rajzolás óta
		while ( SDL_PollEvent(&ev) )
		{
			switch (ev.type)
			{
			case SDL_QUIT: //ablak jobb felső sarkában x-et megnyomtuk
				quit = true;
				break;
			//   SDL_KEYUP:  ha elengedem
			case SDL_KEYDOWN: //ha megnyomom a gombot
				// gyakorlatilag egy enum jelzi hogy milyen gomb
				if ( ev.key.keysym.sym == SDLK_ESCAPE )
					quit = true;
				break;
			case SDL_MOUSEMOTION: //egér mozgás
				mouseX = ev.motion.x;
				mouseY = ev.motion.y;
				break;
			case SDL_MOUSEBUTTONUP:
				// egérgomb felengedésének eseménye; a felengedett gomb a ev.button.button -ban található
				// a lehetséges gombok a következõek: SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, 
				//		SDL_BUTTON_RIGHT, SDL_BUTTON_WHEELUP, SDL_BUTTON_WHEELDOWN

				if (ev.button.button == SDL_BUTTON_LEFT) {
					rectColR = 255, rectColG = 0, rectColB = 0;
				} else if (ev.button.button == SDL_BUTTON_RIGHT) {
					rectColR = 0, rectColG = 0, rectColB = 255;
				}

				break;
			}
		}

		// töröljük a hátteret fehérre
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		// aktuális rajzolási szín legyen zöld és rajzoljunk ki egy vonalat
		SDL_SetRenderDrawColor(	ren,	// renderer címe, aminek a rajzolási színét be akarjuk állítani
								0,		// piros
								255,	// zöld
								0,		// kék
								255);	// átlátszatlanság

		SDL_RenderDrawLine(	ren,	// renderer címe, ahová vonalat akarunk rajzolni
							0, 0, // vonal kezdõpontjának (x,y) koordinátái
							mouseX, mouseY);// vonal végpontjának (x,y) koordinátái
		
		//1. feladat:
		//SDL_GetTicks();

		//sin periódusának számítása: ez lenne 2pi: sinf(SDL_GetTicks() / 1000.0)
		//így végül 20 és 50 között fog "pattogni" (abs miatt), 2 másodperces periódussal
		int size = abs(sinf(SDL_GetTicks() / 1000.0 * 2 * M_PI / 2)) * 30 + 20;
	

		// definiáljunk egy (mouseX, mouseY) középpontó, tengelyekkel párhuzamos oldalú
		// 20x20-as négyzetet:
		SDL_Rect cursor_rect; //SDL rect struktúra
		cursor_rect.x = mouseX - size/2;
		cursor_rect.y = mouseY - size/2;
		cursor_rect.w = size;
		cursor_rect.h = size;

		// legyen a négyzet kitöltési színe piros
		SDL_SetRenderDrawColor( ren, rectColR, rectColG, rectColB, 255 );
		SDL_RenderFillRect( ren, &cursor_rect);

		// 1. feladat: az eltelt idõ függvényében periodikusan nõjjön és csökkenjen
		//    az egérmutató középpontjával kirajzolt négyszög

		// 2. feladat: ha a user a bal egérgombot nyomja meg akkor a téglalap színe váltson pirosra,
		//    ha a jobb egérgombot, akkor kékre

		// 3. beadható feladat: rajzolj ki egy 50 sugarú körvonalat az egérmutató köré!
		// segítség: használd a SDL_RenderDrawLines()-t
			//Változtatás: legyen "fogaskerék szerű"


		const int numOfPoints = 30;
		SDL_Point points[numOfPoints + 1];
		float innerR = 35, outerR = 50;
		float alpha = 2 * M_PI / numOfPoints; //egy szakaszhoz tartozó körcikk foka radiánban
		bool inner = false;
		int curR = 35;

		for (int i = 0; i <= numOfPoints; i++)
		{
			//cpp inicializálós lista thingy:
				//points[i] = {1,2} ekvivalens = SDL_Point(1,2)

			curR = inner ? innerR : outerR;
			inner = !inner;

			//Fontos: cosf/sinf függvények radiánnal dolgoznak
			points[i] = { mouseX + (int)(curR * cosf(alpha * i)),
						  mouseY + (int)(curR * sinf(alpha * i)) };
		}


		SDL_RenderDrawLines(ren, points, numOfPoints + 1);

		// jelenítsük meg a backbuffer tartalmát
		SDL_RenderPresent(ren);
	}

	//
	// 4. lépés: lépjünk ki
	// 

	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}