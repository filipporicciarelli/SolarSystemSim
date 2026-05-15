# Solar System Simulator (SFML)

An interactive Solar System simulator developed in C++ using the SFML library. The project features dynamic planetary orbit visualization with an advanced camera system.

## 🚀 Key Features
- **Dynamic Camera:** Click on a planet to follow it and automatically zoom in.
- **Interactive HUD:** A toggleable help panel ("?" button) that displays commands without obstructing the view.
- **Dual-View System:** The UI remains fixed while the deep space can be explored and zoomed freely.
- **Time Management:** Pause or adjust the simulation speed in real-time.
- **Astronomical Details:** Includes major planets with relative sizes and orbital speeds, plus the Moon with conditional visibility.
- **Visual Trails:** Smooth planetary trails to visualize orbital paths.

## 🎮 Controls
- **Left Click:** Select a planet to follow / Toggle the "?" help button.
- **Space:** Pause/Resume the simulation.
- **Right Arrow:** Increase time speed.
- **Left Arrow:** Decrease time speed.
- **Backspace:** Reset the camera (return to system overview).
- **Esc:** Exit the application.

## 🛠 Tech Stack
- **Language:** C++
- **Graphics Library:** SFML 3.0
- **Font:** Comic Sans MS (for that friendly look!)

## 🔧 Installation
1. Ensure you have **SFML 3.0** installed and configured in your environment.
2. Compile the `main.cpp` and `Planet.cpp` files.
3. Make sure `comic.ttf` is in the same directory as the executable.
4. Run the application and enjoy the space exploration!
