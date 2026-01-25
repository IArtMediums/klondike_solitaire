## Solitaire (Klondike)

A classic Klondike Solitaire game implementation in C using SDL2.

## Features

- **Classic Klondike Rules**: Traditional solitaire gameplay with enforced game rules
- **Undo System**: Press `Backspace` to undo your moves
- **Restart Game**: Press `R` to restart the current deal
- **New Game**: Press `N` to shuffle and deal a new game
- **Win Detection**: Automatically detects when you've won and displays a victory screen

## Controls

| Key           | Action                |
| ------------- | --------------------- |
| `Mouse Click` | Select and move cards |
| `Backspace`   | Undo last move        |
| `R`           | Restart current game  |
| `N`           | Start new game        |
| `ESC`         | Quit game             |

## Requirements

- SDL2
- SDL_Image
- C compiler (gcc, clang, etc.)
- Make

## Building

**Make sure that paths in Makefile command point to SDL2 and SDL_image on your system**

```bash
make
```

## Running

```bash
make run
```

## Cleaning

```bash
make clean
```

Or run the executable directly:

```bash
./game
```

## How to Play

The goal is to move all cards to the four foundation piles (top right), organized by suit from Ace to King.

### Rules:

- **Tableau**: Build down in alternating colors (red on black, black on red)
- **Foundation**: Build up by suit starting with Ace
- **Stock**: Click to draw cards from the stock pile
- Only Kings can be placed on empty tableau columns
- Only Aces can start foundation piles

## Technical Highlights

- **Command Pattern**: Implements undo/redo using a command stack
- **Stack-based Architecture**: Cards are managed in stacks (foundation, tableau, stock, waste)
- **Rule Validation**: Moves are validated before execution
- **Memory Management**: Proper allocation and deallocation of game resources

## Known Limitations

- Cards can go off bounds
- Selection is not persistent
- Can't double-click to move card to foundation
- Game is not guaranteed to be solvable

## Future Improvements

- [ ] Score tracking
- [ ] Timer
- [ ] Move counter
- [ ] Dynamic tableau resizing
- [ ] Move hints
- [ ] Difficulty selection
- [ ] Different card designs
- [ ] Sound effects
- [ ] Animation effects
- [ ] A lot of polish

## Credits

- Card assets: https://ivoryred.itch.io/pixel-poker-cards
- Developed as a learning project

## License

[GPL-3.0]

## Author

[IArtMediums] klondike_solitaire
