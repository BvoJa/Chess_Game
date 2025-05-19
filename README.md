## Guide to use 
Clone the repo locally and then click ChessGame.exe to run

## Project Structure

```
chess-game/
├── assets/               # Images, fonts, sound and game history
│   ├── images/           # images of pieces, boards, background and figure
│   ├── sound/            # Sound effects and music files
│   ├── fonts/            # fonts
│   ├── GameHistory/      # the saved game states
├── src/                  # Source code files
│   ├── main.cpp          # Entry point to manage overall game flow
│   ├── Menu.cpp          # Handles the main menu interface (Play, Settings, Exit)
│   ├── Mode.cpp          # Displays options for game modes (Player vs Player, Player vs AI)
│   ├── OnePlayerMode.cpp # Manages AI difficulty selection and player side (white or black)
│   ├── Playaswhite.cpp   # Handles game logic when the player is playing as white
│   ├── Playasblack.cpp   # Handles game logic when the player is playing as black
│   ├── Twoplayermode.cpp # Manages logic and interface for two-player games
│   ├── Settings.cpp      # Handles customization options for board, pieces, and sound effects
│   ├── SoundEffect.cpp   # Manages volume and settings for sound effects
│   ├── Backgroundmusic.cpp # Manages background music settings and volume
├── include/              # Header files
│   ├── Constant.h        # Constants and utility functions (e.g., `random`, `scalepicture`)
│   ├── Menu.h            # Definitions for the main menu interface
│   ├── Mode.h            # Definitions for game mode options
│   ├── OnePlayerMode.h   # Definitions for AI difficulty selection and player side
│   ├── Playaswhite.h     # Definitions for game logic when playing as white
│   ├── Playasblack.h     # Definitions for game logic when playing as black
│   ├── TwoPlayerMode.h   # Definitions for two-player game logic
│   ├── Settings.h        # Definitions for customization options
│   ├── SoundEffect.h     # Definitions for sound effect management
│   ├── Backgroundmusic.h # Definitions for background music management
├── README.md             # Project documentation
```
