# Mouse Track
Track, display, and graph the position of a mouse cursor.

The application produces a heatmap and plots the mouse positions .

## Getting Started

### Prerequisites

Python:
```
numpy==1.16.2
matplotlib==3.0.3
```

C++:
```
SDL2
```

### Installing

* Install NumPy: `pip install numpy`
* Install Matplotlib: `pip install matplotlib`
* Install [SDL2](http://libsdl.org/download-2.0.php)

## Usage

1. `git clone https://github.com/claby2/mouse-track.git`
2. `cd mouse-track`
3. `cd src`
4. `make run`
5. Move your mouse around your screen while the window is open.
6. To end the capture, close the window.
- Positions of mouse are written into `data.csv` in root folder
- BMP heatmap is written into `heatmap.bmp` in root folder
7. To plot mouse positions based on `data.csv`, run: `python graph.py`

### Record heatmap without writing into data.csv
To run the application without writing into data.csv (for longer periods of tracking):
1. In src: `make main`
2. `source.exe --nosave`