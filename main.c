#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <stdbool.h>
#include <time.h>

#define GRID_SIZE 200

int main(int argc, char* argv[]) {
	// Setting up SDL
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Init(SDL_INIT_VIDEO);
	
	// Sets the seed to time
	srand(time(NULL));
	
	// Create a window
	window = SDL_CreateWindow(
		"Cellular Automaton",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		800,
		SDL_WINDOW_OPENGL);
	// Check if we were able to make the window
	if (window == NULL) printf("Could not create window.");
	// Create the renderer
	renderer = SDL_CreateRenderer(window, -1, NULL);
	// Check to see if the renderer was made
	if (renderer == NULL) printf("Could not create renderer.");
	// Set the scale of the renderer dependent on the window and grid size
	SDL_RenderSetScale(renderer, 800.0f / (float)GRID_SIZE, 800.0f / (float)GRID_SIZE);

	// Set up two grids
	bool grid[GRID_SIZE][GRID_SIZE][2];
	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			// Initialize the first grid to randomly true or false
			grid[y][x][0] = rand() % 2;
			// Initialize the second grid to false
			grid[y][x][1] = false;
		}
	}

	// gridSelect will be used to tell which grid the screen is showing
	bool gridSelect = false;
	// generation is for counting the generations
	int generation = 0;

	bool running = true;
	while (running) {
		// Check to see if we exit the window
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				running = false;
				break;
			}
		}
		
		// Setting the background to white
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		// Black will be the color of the live cells
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		
		int cellsAlive = 0;
		for (int y = 0; y < GRID_SIZE; y++) {
			for (int x = 0; x < GRID_SIZE; x++) {
				// Count number of alive neigbors
				int aliveNeighbors = 0;
				// Start at the row above the current cell 
				// End at the row below the current cell
				int startX = x - 1, startY = y - 1;
				// Start at the column to the left of the current cell 
				// End at the column to the right of the current cell 
				int endX = x + 1, endY = y + 1;
				// Not allowing it to check out of bounds
				if (startX < 0) startX = x;
				if (startY < 0) startY = y;
				if (endX >= GRID_SIZE) endX = x;
				if (endY >= GRID_SIZE) endY = y;
				for (int yy = startY; yy <= endY; yy++) {
					for (int xx = startX; xx <= endX; xx++) {
						// If the current cell is alive and the cell 
						// isn't one we're checking for alive neighbors
						// then we add one to aliveNeighbors
						if (grid[yy][xx][gridSelect] && !(xx == x && yy == y)) {
							aliveNeighbors++;
						}
					}
				}

				// Cell is currently alive
				if (grid[y][x][gridSelect]) {
					cellsAlive++;
					// Draw it to the screen
					SDL_RenderDrawPoint(renderer, x, y);
					// Does it survive?
					if (aliveNeighbors == 2 || aliveNeighbors == 3)
						// It survived
						grid[y][x][!gridSelect] = true;
					else
						// It died
						grid[y][x][!gridSelect] = false;
				}
				// Cell is currently dead
				else {
					// Does it revive?
					if (aliveNeighbors == 3)
						// It revived
						grid[y][x][!gridSelect] = true;
					else
						// It's still dead
						grid[y][x][!gridSelect] = false;
				}
			}
		}
		// Swap to the other grid
		gridSelect = !gridSelect;
		printf("Generation: %d, Cells Alive: %d\n", ++generation, cellsAlive);
		// Delay so it goes at a watchable speed
		SDL_Delay(100);

		// Render everything to the screen
		SDL_RenderPresent(renderer);
	}

	// Clean up
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}