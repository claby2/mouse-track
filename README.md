# Mouse Track
Track, display, and graph the position of a mouse cursor.

The application produces a heatmap and plots the mouse positions. Mouse Track is only available for Windows OS as it requires functions from the Windows API.

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

1. Clone, compile, and run:
```
$ git clone https://github.com/claby2/mouse-track.git
$ cd mouse-track
$ cd src
$ make run
```
2. Move your mouse around your screen while the window is open.
3. To end the capture, close the window.
    - Positions of mouse are written into `data.csv` in root folder
    - BMP heatmap is written into `heatmap.bmp` in root folder
4. Plot mouse positions based on `data.csv`:
```
$ python graph.py
```

### Record heatmap without writing into data.csv
To run the application without writing into data.csv (for longer periods of tracking):
1. In src: `make main`
2. `source.exe --nosave`

### Playback mouse movements based on data.csv
1. In src: `make playback`

### Save graph as image
1. In src: `python graph.py -s`
    - Graph will be saved as `graph.png` in root folder